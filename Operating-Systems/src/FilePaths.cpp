//
// Created by csis on 4/30/20.
//

#include <cstring>
#include "../include/FilePaths.h"

/* Searches for a given file name in a directory (the iNum of which is given). Returns the inode number of the file if found, else returns 0 */
uint32_t FilePaths::searchDirectory(Ext2File *ext2, uint32_t iNum, char* targetFile){
    /* Pointer to given directory */
    Directory *searchDirectory;

    /* Open given directory */
    searchDirectory = Directories::openDirectory(ext2, iNum);

    /* A variable to hold the iNum and name of files as we loop through the different files of the directory */
    uint32_t indexINum;
    char* indexName = new char[255];

    /* Flag when we reach the end of the directory */
    bool notEndofFile = true;

    /* Loop through the given directory contents */
    while(notEndofFile) {
        notEndofFile = Directories::getNextDirent(searchDirectory, indexINum, indexName);

        /* Compare the target name with the indexName. End function if found */
        if(strcmp(indexName, targetFile) == 0) {
            delete[] indexName;
            Directories::closeDir(searchDirectory);
            return indexINum;
        }
    }

    /* File not found */
    Directories::closeDir(searchDirectory);
    delete[] indexName;
    return 0;
}

uint32_t FilePaths::traversePath(Ext2File *ext2, char* path){
    int start = 1;
    uint32_t length = strlen(path);
    uint32_t iNum = 2;
    while(start < length && iNum != 0){
        uint32_t end = start + 1;
        while(path[end] != 0 && path[end]  != '/')
            end++;
        path[end] = 0;
        iNum = searchDirectory(ext2,iNum,path + start);
        start = end + 1;
    }
    return iNum;
}