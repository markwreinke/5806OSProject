#include <fcntl.h>
#include "../include/VDIFile.h"
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <string>

/* Declaration of constructor with transMapSize input. Because it is explicit, it must be called VDIFile(int 3)*/
VDIFile::VDIFile(int transSize) {
    transMapSize = transSize;
    VDITransMapPointer = new int[transMapSize];
    cursor = 0;
}

/* This loads the header information and such from the given vdi file. Because VDIFile is a class, returns a boolean instead of a pointer. */
bool VDIFile::vdiOpen(char *fn) {
    this->fileDescriptor = open(fn, 2);

    /* the file failed to open, return false */
    if(this->fileDescriptor == -1) {
        return false;
    } else {
        /* Sets the header into structure */
        read(fileDescriptor, &headerInfo, sizeof(headerInfo));

        /* Initialize the translation map */
        this-> transMapSize = headerInfo.cBlocks; // The number of blocks in the image
        this-> VDITransMapPointer = new int[transMapSize];
        lseek(fileDescriptor, headerInfo.offBlocks, SEEK_SET);
        read(fileDescriptor, VDITransMapPointer, transMapSize* sizeof(int));

        this->cursor = 0;

    }
};
/* Closes the vdi file while destroying dynamically created stuff */
void VDIFile::vdiClose() {
    close(fileDescriptor);
    delete[] VDITransMapPointer;
}


ssize_t VDIFile::vdiRead(void *buf, size_t count) {
  size_t bytesRemaining = count;
  size_t bytesRead = 0;


  while(bytesRemaining > 0){

      /* Translate the cursor to the real location according to the translation map */
      size_t virtualPage = cursor/this->headerInfo.cbBlock;
      size_t offset = cursor%this->headerInfo.cbBlock;
      size_t physicalPage = this->VDITransMapPointer[virtualPage];
      size_t realLocation = physicalPage*this->headerInfo.cbBlock + offset;
      lseek(fileDescriptor, realLocation + this->headerInfo.offData, SEEK_SET);

      ssize_t bytesJustRead = 0;

      /* If the location has been initialized */
      if(realLocation >= 0) {
          size_t bytesToRead = 0;
          if(count < this-> headerInfo.cbBlock){
              bytesToRead = count;
          }else{
              bytesToRead = this->headerInfo.cbBlock;
          }
          bytesJustRead = read(fileDescriptor, static_cast<uint8_t *>(buf) + bytesRead, bytesToRead);
      } else{ /* if the block wasn't initialized, it reads out a block of Os */
          int SizeOfBlock = this->headerInfo.cbBlock;
          for(int x = 0; x > SizeOfBlock; x++){
              *(static_cast<uint8_t *>(buf) + (bytesRead + bytesJustRead)) = '0';
              bytesJustRead++;
          }
      }
      if(bytesJustRead < 0) {
          return -1;
      }
      bytesRead += bytesJustRead;
      bytesRemaining -= bytesJustRead;
      vdiSeek(bytesJustRead, SEEK_CUR);
  }

  return bytesRead;
}


ssize_t VDIFile::vdiWrite(void *buf, size_t count) {
    size_t bytesRemaining = count;
    size_t bytesWritten = 0;


    while(bytesRemaining > 0) {
        /* Translate the cursor to the real location according to the translation map */
        size_t virtualPage = cursor / this->headerInfo.cbBlock;
        size_t offset = cursor % this->headerInfo.cbBlock;
        size_t physicalPage = this->VDITransMapPointer[virtualPage];
        size_t realLocation = physicalPage * this->headerInfo.cbBlock + offset;
        lseek(fileDescriptor, realLocation + this->headerInfo.offData, SEEK_SET);

        size_t bytesJustWritten = 0;

        /* If the block wasn't initialized */
        if(realLocation < 0) {
            lseek(fileDescriptor, 0, SEEK_END);
            uint8_t *pageBuffer = new uint8_t[this->headerInfo.cbBlock];
            write(fileDescriptor, pageBuffer, this->headerInfo.cbBlock);
            delete[] pageBuffer;

            physicalPage = this->headerInfo.cBlocksAllocated;
            this->headerInfo.cBlocksAllocated++;

            this->VDITransMapPointer[virtualPage] = physicalPage;

            lseek(fileDescriptor, 0, SEEK_SET);
            write(fileDescriptor, &this->headerInfo, sizeof(this->headerInfo));

            lseek(fileDescriptor, this->headerInfo.offBlocks, SEEK_SET);
            write(fileDescriptor, this->VDITransMapPointer, sizeof(uint32_t) * this->headerInfo.cBlocks);
        }

        size_t bytesToWrite = 0;

        /* This is so the method writes one block at a time */
        if (count < this->headerInfo.cbBlock) {
            bytesToWrite = count;
        } else {
            bytesToWrite = this->headerInfo.cbBlock;
        }

        bytesJustWritten = write(fileDescriptor, static_cast<uint8_t *>(buf) + bytesWritten, bytesToWrite);

        bytesWritten += bytesJustWritten;
        bytesRemaining -= bytesJustWritten;
        vdiSeek(bytesJustWritten, SEEK_CUR);
    }

    return bytesWritten;
}

off_t VDIFile::vdiSeek(off_t offset, int anchor) {
    if(anchor == SEEK_SET) {
        if(offset < this->headerInfo.cbDisk && offset >= 0)
            this->cursor = offset;
        else
            return -1;
    }
    else if(anchor == SEEK_CUR) {
        if((this->cursor + offset) < this->headerInfo.cbDisk && (this->cursor + offset) >= 0)
            this->cursor = this->cursor + offset;
        else
            return -1;
    }
    else if (anchor == SEEK_END) {
        if(offset < 0){
            this->cursor = this->headerInfo.cbDisk + offset;
        } else{
            return -1;
        }
    }
    else {
        return -1;
    }
    return cursor;
}

