#include "Editor.h"
#include "Buffer.h"
#include <ncurses.h>

using namespace std;

void init_ncurses();
void updateText();
void input(int);
void showMenu();
static void quit();

char * fname;
int trow, tcol; // rows and cols in screen
int crow, ccol; // coordinates of cursor
int ppos; // pad position
int psize;
int lines;
bool doExit = false;
Editor ed;
Buffer * cont;
WINDOW * filePad; // file content pad
WINDOW * menu;

// TODO: argument/error handling
int main(const int argc, char * argv[]) {
    if (argc > 1) {
        fname = argv[1];
        ed = Editor(fname); // new editor object using existing file
    } else {
        ed = Editor(); // new editor object using new file
    }

    init_ncurses();

    ppos = 0;
    crow = 0;
    ccol = 0;
    updateText();
    // main program loop
    while (!doExit) {
    	updateText();
    	int ch = wgetch(filePad);
    	input(ch);
    }

    quit();
}

void init_ncurses() {
    char header[] = "CSCI 1730 Text Editor";

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    getmaxyx(stdscr, trow, tcol);

    mvaddstr(0, 0, "F1: MENU");
    mvaddstr(0, (tcol-strlen(header))/2, header);
    const char * filename = ed.getFilename().c_str(); // convert filename string to const char *
    mvaddstr(trow-1, 0, filename);


    filePad = newpad(10000, tcol-2);
    keypad(filePad, true);
    nodelay(filePad, true);
    psize = trow-5;

    refresh();
    
    WINDOW * text_border; // new window on top of the stdscr
    text_border = newwin(LINES-2,COLS,1,0); // setting dimensions of text_area window
    box(text_border, 0, 0); // window border
    wrefresh(text_border);
    delwin(text_border); // deallocate

    curs_set(1);
}

void updateText() {
    cont = ed.getBuffer();
    lines = cont->text.size()-1;

    for (int i = 0; i < lines; i++) {
        if (i == lines-1) {
            wmove(filePad, i+1, 0);
            wclrtoeol(filePad);
        }
        wmove(filePad, i, 0);
        wclrtoeol(filePad);
        mvwprintw(filePad, i, 0, cont->text[i].c_str());
    }
    wmove(filePad, crow, ccol);
    prefresh(filePad, ppos, 0, 2, 1, trow-3, tcol-2);
}

void input(int ch) {
    switch(ch) {
        case KEY_F(1):
            showMenu();
            break;
        case KEY_UP:
            if (crow > 0) {
                if (crow == ppos) {
                    ppos--;
                }
                crow--;
                if (ccol >= cont->text[crow].length()) {
                    ccol = cont->text[crow].length();
                }
            }
            break;
        case KEY_DOWN:
            if (crow < lines-1) {
                if (crow == ppos+psize) {
                    ppos++;
                }
                crow++;
                if (ccol >= cont->text[crow].length()) {
                    ccol = cont->text[crow].length();
                }
            }
            break;
        case KEY_LEFT:
            if (ccol > 0) {
                ccol--;
            }
            break;
        case KEY_RIGHT:
            if ((ccol < tcol) && (ccol < cont->text[crow].length())) {
                ccol++;
            }
            break;
        case 127:
        case KEY_BACKSPACE: // Backspace
            if (crow > 0 && ccol == 0) { // if at beginning of line
                ccol = cont->text[crow-1].length(); // set cursor position to end of previous line
                cont->text[crow-1] += cont->text[crow]; // move line up
                cont->delLine(crow); // delete line
                crow--; // move cursor one row up
            } else if (crow > 0 || ccol > 0) {
                ccol--; // move cursor one character to left
                cont->text[crow].erase(ccol, 1); // delete character
            }
            break;
        case KEY_DC: // Delete
            if (crow < lines-1 && ccol == cont->text[crow].length()) {
                cont->text[crow] += cont->text[crow+1];
                cont->delLine(crow+1);
            } else if (!(crow == lines && ccol == cont->text[crow].length())) {
                cont->text[crow].erase(ccol, 1);
            }
            break;
        case 10:
        case KEY_ENTER: // Enter
            if (ccol < cont->text[crow].length()) {
                string substring = cont->text[crow].substr(ccol, cont->text[crow].length()-ccol);
                cont->text[crow].erase(ccol, cont->text[crow].length()-ccol);
                cont->insLine(substring, crow+1);
            } else {
                cont->insLine("", crow+1);
            }
            ccol = 0;
            crow++;
            break;
        case 9: // Tab
            cont->text[crow].insert(ccol, "\t"); // insert 4 spaces
            ccol = ccol+8;
            break;
    }
}

void showMenu() {
    int choice;
    int highlight = 0;
    string choices[4] = {"Open", "Save", "Save As", "Exit"};

    menu = newwin(10, 40, (trow-10)/2, (tcol-40)/2);
    box(menu, 0, 0);
    mvwhline(menu, 2, 1, 0, 38);
    touchwin(menu);
    curs_set(0);
    refresh();
    mvwaddstr(menu, 1, 7, "Menu");
    keypad(menu, true);
    wrefresh(menu);


    while (true) {
        for (int i = 0; i < 4; i++) {
            if (i == highlight) wattron(menu, A_REVERSE);
            mvwprintw(menu, i+3, 1, choices[i].c_str());
            wattroff(menu, A_REVERSE);
        }
        choice = wgetch(menu);

        switch (choice) {
            case KEY_UP:
                if (highlight > 0) {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight < 4) {
                    highlight++;
                }
                break;
            default:
                break;
        }

        if (choice == 10) {
            break;
        }
    }

    switch (highlight) {
        char str[80];
        case 0: // Open
            mvwprintw(menu,8,2,"Enter file name: ");
            wgetstr(menu,str);
            wrefresh(menu);
            ed.openFile(str);
            break;
        case 1: // Save
            ed.saveFile();
            break;
        case 2: // Save As
            mvwprintw(menu,8,2,"Save file as: ")
            wgetstr(menu,str);
            wrefresh(menu);
            ed.saveFileAs(str);
            break;
        case 3: // Quit
            doExit = true;
            break;
    }

    delwin(menu);
    curs_set(1);
}

static void quit() {
    endwin();
    exit(0);
}

