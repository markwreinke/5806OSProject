//
// Created by csis on 4/30/20.
//

#ifndef INC_5806OSPROJECT_COPYFILE_H
#define INC_5806OSPROJECT_COPYFILE_H


#include <stdio.h>
#include "FilePaths.h"

class copyFile {
public:
    static ssize_t copyFileToHost(char* vdiName, char* src, char* dest);
    static ssize_t copyFileToVDI(char* vdiName, char* src);
    static void viewVDIDirectories(char* filename);

    static char *traverseGivenFileName(char *fileName);
};


#endif //INC_5806OSPROJECT_COPYFILE_H
