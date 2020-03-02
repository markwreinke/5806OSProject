#include <fcntl.h>
#include "../include/VDIfile.h"
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <string>

/* Declaration of constructor with transMapSize input. Because it is explicit, it must be called VDIfile(int 3)*/
VDIfile::VDIfile(int transSize) {
    transMapSize = transSize;
    VDITransMapPointer = new int{transMapSize};
    cursor = 0;
}

/* This loads the header information and such from the given vdi file. Because VDIfile is a class, returns a boolean instead of a pointer. */
bool VDIfile::vdiOpen(char *fn) {
    fileDescriptor = open(fn, 2);

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
void VDIfile::vdiClose() {
    close(this->fileDescriptor);
    delete[] VDITransMapPointer;
}


ssize_t VDIfile::vdiRead(void *buf, size_t count) {
  size_t bytesRemaining = count;
  size_t bytesRead = 0;


  while(bytesRemaining > 0){

      /* Translate the cursor to the real location according to the translation map */
      size_t virtualPage = cursor/this->headerInfo.cbBlock;
      size_t offset = cursor%this->headerInfo.cbBlock;
      size_t physicalPage = this->VDITransMapPointer[virtualPage];
      size_t realLocation = physicalPage*this->headerInfo.cbBlock + offset;
      lseek(fileDescriptor, realLocation, SEEK_SET);


      size_t bytesJustRead = 0;

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

      bytesRead += bytesJustRead;
      bytesRemaining -= bytesJustRead;
      vdiSeek(bytesJustRead, SEEK_CUR);
  }
}


ssize_t VDIfile::vdiWrite(void *buf, size_t count) {
    size_t bytesRemaining = count;
    size_t bytesWritten = 0;


    while(bytesRemaining > 0) {
        /* Translate the cursor to the real location according to the translation map */
        size_t virtualPage = cursor / this->headerInfo.cbBlock;
        size_t offset = cursor % this->headerInfo.cbBlock;
        size_t physicalPage = this->VDITransMapPointer[virtualPage];
        size_t realLocation = physicalPage * this->headerInfo.cbBlock + offset;
        lseek(fileDescriptor, realLocation, SEEK_SET);

        size_t bytesJustWritten = 0;

        /* If the block wasn't initialized */
        if (realLocation < 0) {
            size_t newLocation =
                    this->headerInfo.offData + this->headerInfo.cBlocksAllocated * this->headerInfo.cbBlock;
            lseek(fileDescriptor, newLocation, SEEK_SET);
            this->VDITransMapPointer[virtualPage] = newLocation;
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
    /*
    lseek(this->fileDescriptor, this->cursor, 0);//will move the cursor of the file with the given descriptor to the location of our cursor
    size_t bytesWritten = 0;
    uint8_t buffer[count];
    while(count != 0)
    {
        if (count >= 256) {
            for(int x = bytesWritten; x < (bytesWritten + 256); x++)
            {
                buffer[x] = ((uint8_t *)buf)[x];
            }
            write(fileDescriptor, buffer, 256);
            bytesWritten += 256;
            cursor += 256;
            count -= 256;
        } else {
            for(int x = bytesWritten; x < (bytesWritten + count); x++)
            {
                buffer[x] = ((uint8_t *)buf)[x];
            }
            write(fileDescriptor, buffer, count);
            cursor += count;
            count -= count;
            bytesWritten += count;
        }
        return bytesWritten;
    }
    */
}


/* I feel like this is a better situation for a switch, but that is kinda arbitrary */
off_t VDIfile::vdiSeek(off_t offset, int anchor) {
    if(anchor == SEEK_SET) {
        if(offset < this->headerInfo.cbDisk && offset >= 0)
            this->cursor = headerInfo.offData + offset;
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

