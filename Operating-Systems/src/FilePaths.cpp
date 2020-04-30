//
// Created by csis on 4/30/20.
//

#include <cstring>
#include "../include/FilePaths.h"
uint32_t FilePaths::searchDirectory(Ext2File *ext2, uint32_t iNum, char* targetFile){

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