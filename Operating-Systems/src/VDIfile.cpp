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

    cout << "I'm doing someting" << endl;


    fileDescriptor = open(fn, 2);

    cout << "I'm still doing someting" << endl;


    /* the file failed to open, return false */
    if(this->fileDescriptor == -1) {
        return false;
    } else {
        /* A temporary tempBuffer to use for reading commands */

        read(fileDescriptor, &headerInfo, sizeof(headerInfo));

        /* Initialize the translation map */
        this-> transMapSize = headerInfo.cBlocks; // The number of blocks in the image
        cout << transMapSize << endl;
        this-> VDITransMapPointer = new int[transMapSize];
        lseek(fileDescriptor, headerInfo.offBlocks, SEEK_SET);
        read(fileDescriptor, VDITransMapPointer, transMapSize* sizeof(int));
        this->cursor = 0; /// This sets the start of the curser to just after the header, accounting for the header + preheader size.
        cout << "signature: 0x" << hex << headerInfo.u32Signature << endl;
        cout << "offblocks: " << hex << headerInfo.offBlocks << endl;
    }
};
/* Closes the vdi file while destroying dynamically created stuff */
void VDIfile::vdiClose() {
    close(this->fileDescriptor);
    delete[] VDITransMapPointer;
}


ssize_t VDIfile::vdiRead(void *buf, size_t count) {
    lseek(this->fileDescriptor, cursor, SEEK_SET);
    size_t BytesRead = 0;
    uint8_t *tempBuffer;
    uint8_t FullBuffer[count];
    while(count != 0) {
        if(count > 256) {
            read(this->fileDescriptor, buf, 256);
            cursor += 256;
            tempBuffer = static_cast<uint8_t *>(buf);
            for (int x = BytesRead; x < (BytesRead + 256); x++) {
                FullBuffer[x] = tempBuffer[x];
            }
            BytesRead += 256;
            count -= 256;
        }
        else{
            read(this->fileDescriptor,buf, count);
            cursor+= count;
            tempBuffer = static_cast<uint8_t *>(buf);
            for (int x = BytesRead; x < (BytesRead + count); x++) {
                FullBuffer[x] = tempBuffer[x];
            }
            BytesRead += count;
            count -= count;
        }
        return BytesRead;
    }
}
ssize_t VDIfile::vdiWrite(void *buf, size_t count) {
    lseek(this->fileDescriptor, this->cursor, 0);//will move the cursor of the file with the given descriptor to the location of our cursor
    write(this->fileDescriptor, buf, count);
    cursor += count; // must move our cursor according to how many bytes we counted.
}
/* I feel like this is a better situation for a switch, but that is kinda arbitrary */
off_t VDIfile::vdiSeek(off_t offset, int anchor) {
    if(anchor == 0) {
        if(offset < this->headerInfo.cbDisk && offset >= 0)
            this->cursor = offset;
        else
            return -1;
    }
    else if(anchor == 1) {
        if((this->cursor + offset) < this->headerInfo.cbDisk && (this->cursor + offset) >= 0)
            this->cursor = this->cursor + offset;
        else
            return -1;
    }
    else if (anchor == 2) {
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

