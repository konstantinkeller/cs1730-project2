#include "Editor.h"
#include <unistd.h>
#include <fcntl.h>

using namespace std;

/**
 * Editor constructor for existing file
 */
Editor::Editor(char * file) {
    const int BUFF_SIZE = 1024;
    char buffer [BUFF_SIZE];
    int n;

    Editor::fname = file;
    
    if ((Editor::fd = open(Editor::fname, O_RDWR)) != -1) { // attempts to open file
        n = 0;
        while ((n = read(Editor::fd, buffer, BUFF_SIZE)) > 0) {
            Editor::contents += buffer; // appends buffer contents to file content string
        }
    } else {
        exit(EXIT_FAILURE);
    }
}

/**
 * Returns string variable containing file contents
 */
string Editor::getContents() {
    string contents = Editor::contents;
    return contents;
}
