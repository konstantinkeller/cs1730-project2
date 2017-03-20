#include <iostream>
#include <fstream>

#ifndef EDITOR_H
#define EDITOR_H

using std::string;

class Editor {
    char * fname;
    string contents;
    int fd;

    public: 
        Editor();
        Editor(char * file);
        string getContents();
};

#endif
