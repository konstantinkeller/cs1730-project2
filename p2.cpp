#include "Editor.h"
#include <ncurses.h>

using namespace std;

void init_ncurses();
static void quit();

char * fname;
int trow, tcol; // rows and cols in screen
int crow, ccol; // coordinates of cursor
int lines; // lines in file
int ppos; // pad position
int psize;
string cont; // file contents
WINDOW * filePad; // file content pad

// TODO: argument/error handling
int main(const int argc, char * argv[]) {
    fname = argv[1];
    Editor te(fname); // new editor object using existing file
    cont = te.getContents(); // fills content string with file contents
    lines = te.countLines();

    init_ncurses();

    wprintw(filePad, cont.c_str()); // prints file contents in pad
    wmove(filePad, 0, 0);

    ppos = 0;
    prefresh(filePad, ppos, 0, 2, 0, trow-2, tcol);

    crow = 0;
    ccol = 0;

    int ch;
    while ((ch = wgetch(filePad)) != KEY_F(2)) {
        switch(ch) {
            case KEY_UP:
                if (crow > 0) {
                    if (crow == ppos) {
                        ppos--;
                    }
                    crow--;
                }
                wmove(filePad, crow, ccol);
                break;
            case KEY_DOWN:
                if (crow < lines-1) {
                    if (crow == ppos+psize) {
                        ppos++;
                    }
                    crow++;
                }
                wmove(filePad, crow, ccol);
                break;
            case KEY_LEFT:
                if (ccol > 0) {
                    ccol--;
                }
                wmove(filePad, crow, ccol);
                break;
            case KEY_RIGHT:
                if (ccol < tcol) {
                    ccol++;
                }
                wmove(filePad, crow, ccol);
                break;
        }
        prefresh(filePad, ppos, 0, 2, 0, trow-2, tcol);
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

    mvaddstr(0, 0, "F1: MENU  F2: QUIT");
    mvaddstr(0, (tcol-strlen(header))/2, header);
    mvaddstr(trow-1, 0, fname);

    filePad = newpad(lines, tcol-2);
    keypad(filePad, true);
    nodelay(filePad, true);
    psize = trow-4;

    refresh();
    
  WINDOW * text_area; //new window on top of the stdscr
  text_area = newwin(LINES-2,COLS-2,1,1); //setting dimensions of text_area window
  box(text_area, '|', '-'); //border characters(i still get letters on the corners :|)
  wmove(text_area,1,1); //move cursor to text area window
  wrefresh(text_area);
  getch();
  delwin(text_area); //deallocate
  endwin();

}

static void quit() {
    endwin();
    exit(0);
}
