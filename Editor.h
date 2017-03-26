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
        bool saveFileAs(char * filename);
        bool openFile(char * filename); // I changed these from string to char * but both worked
};

#endif
