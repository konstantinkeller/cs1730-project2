#include <string>
#include <vector>
#include <algorithm>

#ifndef BUFFER_H
#define BUFFER_H

using std::string;
using std::vector;

class Buffer {
    public:
        vector<string> text;

        Buffer();

        void addLine(string);
        void insLine(string, int);
        void delLine(int);

        string replaceTabs(string);
        string replaceSpaces(string);
};

#endif
