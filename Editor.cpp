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
