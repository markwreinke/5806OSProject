//
// Created by csis on 4/30/20.
//

#ifndef INC_5806OSPROJECT_COPYFILE_H
#define INC_5806OSPROJECT_COPYFILE_H


#include <stdio.h>
#include "FilePaths.h"

class copyFile {
    static ssize_t copy(char* vdiName, char* src, char* dest);
};


#endif //INC_5806OSPROJECT_COPYFILE_H