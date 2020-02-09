#include <fcntl.h>
#include "../include/VDIfile.h"
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
/* Declaration of constructor with transMapSize input. Because it is explicit, it must be called VDIfile(int 3)*/
VDIfile::VDIfile(int transSize) {
    transMapSize = transSize;
    VDITransMapPointer = new int{transMapSize};
    cursor = 0;
}
//comment here to test git
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
        cout << transMapSize << endl;
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

// #todo might need to update buf
ssize_t VDIfile::vdiRead(void *buf, size_t count) {
  size_t bytesRemaining = count;
  size_t bytesRead = 0;


  while(bytesRemaining > 0){
      size_t virtualPage = cursor/this->headerInfo.cbBlock;
      size_t offset = cursor%this->headerInfo.cbBlock;
      size_t physicalPage = this->VDITransMapPointer[virtualPage];

      size_t realLocation = physicalPage*this->headerInfo.cbBlock + offset;

      size_t bytesJustRead;
      if(realLocation >= 0) {
          lseek(fileDescriptor, realLocation + this->headerInfo.offData, SEEK_SET);
          bytesJustRead = read(fileDescriptor, buf, this->headerInfo.cbBlock);
      } else{
          stringstream addedZeros;
          addedZeros  << setfill('0') << setw(this->headerInfo.cbBlock);
          // #todo add addedZeros to buf,
          bytesJustRead = this->headerInfo.cbBlock;

      }

      bytesRead += bytesJustRead;
      bytesRemaining -= bytesJustRead;
      vdiSeek(bytesJustRead, SEEK_CUR);






  }




























   /*{ lseek(this->fileDescriptor, cursor, SEEK_SET);
    //cout << dec << "Curser: " << cursor << endl;
    size_t bytesRead = 0;
    uint8_t FullBuffer[count];

    //cout << "Count: " << count << endl;
    while(count != 0) {
        if(count > 256) {
            read(this->fileDescriptor, buf, 256);
            vdiSeek(256, SEEK_CUR);
            //cout << dec << "Curser: " << cursor << endl;

            cout << "bytesRead: " << bytesRead << endl;
            for (int x = bytesRead; x < (bytesRead + 256); x++) {
                FullBuffer[x] = ((uint8_t*)buf)[x];
            }
            bytesRead += 256;
            count -= 256;
        }
        else{
            read(this->fileDescriptor, buf, count);
            vdiSeek(count, SEEK_CUR);
            ssize_t position = lseek(fileDescriptor, 0,  SEEK_CUR);
            //cout << dec << "Cursor2: " <<  position << endl;
            for (int x = bytesRead; x < (bytesRead + count); x++) {
                FullBuffer[x] = ((uint8_t*)buf)[x];
               /*
                * DEBUG STUFF*
                printf("%02x", FullBuffer[x]);
                cout << endl;
            }
            bytesRead += count;
            count -= count;
        }

    }
    for(int x = 0; x < sizeof(FullBuffer); x++)
    {
        ((uint8_t *) buf)[x] = FullBuffer[x];
    }
    return bytesRead }*/
}
ssize_t VDIfile::vdiWrite(void *buf, size_t count) {
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

}
/* I feel like this is a better situation for a switch, but that is kinda arbitrary */
off_t VDIfile::vdiSeek(off_t offset, int anchor) {
    if(anchor == SEEK_SET) {
        if(offset < this->headerInfo.cbDisk && offset >= 0)
            this->cursor = headerInfo.offData;
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

