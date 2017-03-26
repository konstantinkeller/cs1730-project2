#include <iostream>
#include <fstream>
#include "Buffer.h"

#ifndef EDITOR_H
#define EDITOR_H

using std::string;

class Editor {
    string fname;
    Buffer * contents;

    public: 
        Editor();
        Editor(char * file);
        Buffer * getBuffer();
        string getFilename();

        bool saveFile();
        bool saveFileAs(string filename);
        bool openFile();
};

#endif
