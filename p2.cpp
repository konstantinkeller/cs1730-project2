#include "Editor.h"
#include "Buffer.h"
#include <ncurses.h>
#include <cstring>
#include <cstdlib>

using namespace std;

void init_ncurses();
static void quit();
void showmenu(void);

char * fname;
int trow, tcol; // rows and cols in screen
int crow, ccol; // coordinates of cursor
int ppos; // pad position
int psize;
int lines;
int ch;
Editor ed;
Buffer * cont;
WINDOW * filePad; // file content pad
WINDOW * text_border;
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

  //main program loop(press F2 to exit,F1 for menu)
  while (ch != KEY_F(2)) {
   ch = wgetch(text_border);
   wrefresh(text_border);
   switch(ch) {
   case KEY_LEFT:
     getyx(text_border,ccol,crow);
     crow--;
     wmove(text_border,ccol,crow);
     break;
   case KEY_RIGHT:
     getyx(text_border,ccol,crow);
     crow++;
     wmove(text_border,ccol,crow);
     break;
   case KEY_UP:
     getyx(text_border,ccol,crow);
     ccol--;
     wmove(text_border,ccol,crow);
     break;
   case KEY_DOWN:
     getyx(text_border,ccol,crow);
     ccol++;
     wmove(text_border,ccol,crow);
     break;
   case KEY_F(1):
     showmenu();
     break;
   default:
   waddch(text_border,ch);
   break;
   }
  }
 quit();
}

void init_ncurses() {
  char header[] = "CSCI 1730 Text Editor";

  initscr();
  noecho();
  cbreak();
  keypad(stdscr,true);

  getmaxyx(stdscr, trow, tcol);

  mvaddstr(0, 0, "F1: MENU  F2: QUIT");
  mvaddstr(0, (tcol-strlen(header))/2, header);
  const char * filename = ed.getFilename().c_str(); // convert filename string to const char
  mvaddstr(trow-1, 0, filename);


  filePad = newpad(10000, tcol-2);
  keypad(filePad, true);
  nodelay(filePad, true);
  psize = trow-5;
  refresh();


  text_border = newwin(LINES-2,COLS,1,0); // setting dimensions of text_area
  keypad(text_border,TRUE);
  cbreak();
  box(text_border, 0, 0); // window border
 wborder(text_border,'|','|','-','-','-','-','-','-');
 wmove(text_border,1,1);
 wrefresh(text_border);


 //making the menu window
 menu = newwin(10,40,4,4);
 box(menu,0,0);
 wborder(menu,'|','|','-','-','-','-','-','-');
 mvwhline(menu,2,1,'-',38);
 refresh();
 mvwaddstr(menu,1,7,"My Menu");
 keypad(menu,true);


}


static void quit() {
  delwin(text_border);
  delwin(menu);
  endwin();
  exit(0);
}
//fills the menu window with options, and allows navigation/selection
void showmenu(void) {
  string choices[4] = {"Open", "Save", "Save As", "Exit"};
  int choice;
  int highlight = 0;
  touchwin(menu);
 wrefresh(menu);

  while (1) {
    for (int i = 0; i < 4; i++) {
      if (i == highlight)
        wattron(menu, A_REVERSE);
      mvwprintw(menu, i+3, 1, choices[i].c_str());
      wattroff(menu, A_REVERSE);
    }
    choice = wgetch(menu);

    switch (choice) {
    case KEY_UP:
      highlight--;
      if (highlight == -1)
        highlight= 0;
      break;
    case KEY_DOWN:
      highlight++;
      if (highlight == 4)
        highlight = 3;
      break;
    default:
      break;
    }
    if (choice == 10)
      break;
  }

  switch(highlight) {
  case 3:
    quit();
    break;
  case 2:
    //save as
    break;
  case 1:
    //save
    break;
  case 0:
    //open
    break;
  }

  touchwin(text_border);
  refresh();
}
