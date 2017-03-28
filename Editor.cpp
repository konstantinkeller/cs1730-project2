#include "Editor.h"
#include <fstream>
#include <iostream>

using namespace std;

/**
 * Editor constructor for new file
 */
Editor::Editor() {
    fname = "*NEW FILE*";
    
    contents = new Buffer();
}

/**
 * Editor constructor for existing file
 */
Editor::Editor(char * file) {
    fname = file;

    contents = new Buffer();

    ifstream f(file);
    if (f.is_open()) { // if file is open
        while (!f.eof()) {
            string line;
            getline(f, line);
            contents->addLine(line);
        }
    } else {
        exit(EXIT_FAILURE);
    }
}

/**
 * Returns pointer to  buffer
 */
Buffer * Editor::getBuffer() {
    return contents;
}

/**
 * Returns name of file currently open
 */
string Editor::getFilename() {
    return fname;
}

bool Editor::openFile(char * filename) {
  fname = filename;
  bool result;
  ifstream f(fname);
  contents = new Buffer();

  if (f.is_open()) {
    while(!f.eof()) {
      string line;
      getline(f, line);
      contents->addLine(line);
      result = true;
    }
  } else {
    result = false;
    exit(EXIT_FAILURE);
  }
  return result;

}

bool Editor::saveFile() {
    bool result;
    ofstream f(fname);

    if (f.is_open()) {
        for (int i = 0; i < contents->text.size()-1; i++) {
            f << contents->replaceSpaces(contents->text[i]) << endl;
        }
        result = true;
    } else {
        result = false;
    }

    return result;
}

bool Editor::saveFileAs(char * filename) {
    bool result;
    ofstream f(filename);

    if (f.is_open()) {
        for (int i = 0; i < contents->text.size()-1; i++) {
            f << contents->replaceSpaces(contents->text[i]) << endl;
        }
        result = true;
    } else {
        result = false;
    }

    return result;
}

bool Editor::if_file_exist(char * filename)  {
    ifstream f(filename);
    return f.good();
}
