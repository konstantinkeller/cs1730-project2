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
