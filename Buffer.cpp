#include "Buffer.h"

/**
 * Buffer constructor
 */
Buffer::Buffer() {}

/**
 * Returns string containing line with all tabs replaced by four spaces
 */
string Buffer::replaceTabs(string line) {
    size_t i;
    // loop through all tab characters
    while (true) {
        i = line.find("\t");
        if (i != line.npos) {
            line.replace(i, 1, "    "); // replace tab with four spaces
        } else {
            break;
        }
    }

   return line;
} 

string Buffer::replaceSpaces(string line) {
    size_t i;

    while (true) {
        i = line.find("    ");
        if (i != line.npos) {
            line.replace(i, 4, "\t");
        } else {
            break;
        }
    }

    return line;
}

/**
 * Adds line to the end of the buffer vector
 */
void Buffer::addLine(string line) {
    line = replaceTabs(line); // replace tabs with spaces
    text.push_back(line);
}

/**
 * Inserts line into buffer vector at position n
 */
void Buffer::insLine(string line, int n) {
    line = replaceTabs(line); // replace tabs with spaces
    text.insert(text.begin()+n, line);
}

/**
 * Deletes line from buffer vector
 */
void Buffer::delLine(int n) {
    text.erase(text.begin()+n);
}
