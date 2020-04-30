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
    //where to start in the path, we start at 1 because every path begins with a '/'
    //we need to ignore this so skip the first character
    ///length is the full length of the path including '/'
    ///iNum is the inode to search for the name of the directory in, this is 2 by default because thats the inode we know has good information
    int start = 1;
    uint32_t length = strlen(path);
    uint32_t iNum = 2;
    ///while not at the end of the filePath and search directory doesn't fail traverse through directories
    while(start < length && iNum != 0){
        ///set to start +1 because there must always be 1 more space before the end otherwise start == length and we dont enter here
        int end = start + 1;
        //while the path hasn't had a terminator increase end until it does, then set that symbol to 0;
        while(path[end] != 0 && path[end]  != '/')
            end++;
        path[end] = '0';
        //search for the directory in ext2 at iNum with the fileName being whatever is path+start until a 0 appears
        iNum = searchDirectory(ext2,iNum,path + start);
        //increment start = end+1 so that we skip the null terminator
        start = end + 1;
    }
    ///return the final value of iNum
    return iNum;
}