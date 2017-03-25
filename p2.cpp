#include "Editor.h"
#include "Buffer.h"
#include <ncurses.h>

using namespace std;

void init_ncurses();
void updateText();
void input(int);
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

    mvaddstr(0, 0, "F1: MENU  F2: QUIT");
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
    	case KEY_F(2):
    		doExit = true;
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
            cont->text[crow].insert(ccol, "    "); // insert 4 spaces
            ccol = ccol+4;
            break;
    }
}

static void quit() {
    endwin();
    exit(0);
}

//I commented this out because i'm still working on implementing it differently,
//but the code should be mostly correct

/*
void create_menu() {

  // Menu choices
  const char *choices[] = {
    "Open",
    "Save",
    "Save As",
    "Exit",
  };

  ITEM **my_items;
  int c;
  MENU *my_menu;
  WINDOW *my_menu_win;
  int n_choices, i;

  //creates/allocates choices to the menu
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
    my_items[i] = new_item(choices[i]);

  //creates the actual menu
  my_menu = new_menu((ITEM **)my_items);

  //creates the window for the menu
  my_menu_win = newwin(10,40,4,4);
  keypad(my_menu_win,TRUE);


  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

  //Menu mark is *(aka * will appear next to the selected menu option)
  set_menu_mark(my_menu, " * ");

  //Print a border around the menu window
  box(my_menu_win, 0, 0);
  print_in_middle(my_menu_win, 1, 0, 40, "My Menu");
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
  refresh();

  //post menu to screen
  post_menu(my_menu);
  wrefresh(my_menu_win);


  while((c = wgetch(my_menu_win)) != KEY_F(1))
    {       switch(c)
        {case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
  case KEY_UP:
          menu_driver(my_menu, REQ_UP_ITEM);
          break;
        }
      wrefresh(my_menu_win);
    }

  //free previously allocated memory
  unpost_menu(my_menu);
  free_menu(my_menu);
  for (i =0; i < n_choices; i++)
    free_item(my_items[i]);
  endwin();
}


void print_menu() {
  //to do
}
*/
