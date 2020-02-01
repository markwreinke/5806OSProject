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
    this->fileDescriptor = open(fn, O_RDWR);

    /* the file failed to open, return false */
    if(this->fileDescriptor == -1) {
        return false;
    } else {
        /* A temporary tempBuffer to use for reading commands */
        char *tempBuffer  = new char[64];



        *VDIHeaderInfo.szFileInfo = read(fileDescriptor, tempBuffer, 64);
        VDIHeaderInfo.u32Signature = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.u32Version = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.cbHeader = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.imageType = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.fFlags = read(fileDescriptor, tempBuffer,4);

        *VDIHeaderInfo.szComment = read(fileDescriptor, tempBuffer, 256);


        VDIHeaderInfo.offBlocks = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.offData = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.legacyCCylinders = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.legacyCHeads = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.legacyCSectors = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.legacyCbSector = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.u32Dummy = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.cbDisk = read(fileDescriptor, tempBuffer, 8);
        VDIHeaderInfo.cbBlock = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.cbBlockExtra = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.cBlocks = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.cBlocksAllocated = read(fileDescriptor, tempBuffer, 4);


        /* Filling the UUID for uuidCreate */
        VDIHeaderInfo.uuidCreate.timeLow = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.uuidCreate.timeMid = read(fileDescriptor, tempBuffer, 2);
        VDIHeaderInfo.uuidCreate.timeHigh = read(fileDescriptor, tempBuffer, 2);
        VDIHeaderInfo.uuidCreate.clock = read(fileDescriptor, tempBuffer, 2);
        *VDIHeaderInfo.uuidCreate.node = read(fileDescriptor, tempBuffer, 6);

        /* Filling the UUID for uuidModify */
        VDIHeaderInfo.uuidModify.timeLow = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.uuidModify.timeMid = read(fileDescriptor, tempBuffer, 2);
        VDIHeaderInfo.uuidModify.timeHigh = read(fileDescriptor, tempBuffer, 2);
        VDIHeaderInfo.uuidModify.clock = read(fileDescriptor, tempBuffer, 2);
        *VDIHeaderInfo.uuidModify.node = read(fileDescriptor, tempBuffer, 6);

        /* Filling the UUID for uuidLinkage */
        VDIHeaderInfo.uuidLinkage.timeLow = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.uuidLinkage.timeMid = read(fileDescriptor, tempBuffer, 2);
        VDIHeaderInfo.uuidLinkage.timeHigh = read(fileDescriptor, tempBuffer, 2);
        VDIHeaderInfo.uuidLinkage.clock = read(fileDescriptor, tempBuffer, 2);
        *VDIHeaderInfo.uuidLinkage.node = read(fileDescriptor, tempBuffer, 6);

        /* Filling the UUID for uuidParentModify */
        VDIHeaderInfo.uuidParentModify.timeLow = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.uuidParentModify.timeMid = read(fileDescriptor, tempBuffer, 2);
        VDIHeaderInfo.uuidParentModify.timeHigh = read(fileDescriptor, tempBuffer, 2);
        VDIHeaderInfo.uuidParentModify.clock = read(fileDescriptor, tempBuffer, 2);
        *VDIHeaderInfo.uuidParentModify.node = read(fileDescriptor, tempBuffer, 6);

        VDIHeaderInfo.LCHSCCylinders = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.LCHSCHeads = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.LCHSCSectors = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.LCHSCbSector = read(fileDescriptor, tempBuffer, 4);


        delete[] tempBuffer; // Deallocate memory used by tempBuffer.

        this->cursor = VDIHeaderInfo.cbHeader + 72; /// This sets the start of the curser to just after the header, accounting for the header + preheader size.

    }
};

/*

// todo
void VDIfile::vdiClose()
{
    close(this->descriptor);
    delete[] vdipointertransmap;
}
*/

ssize_t VDIfile::vdiRead(void *buf, size_t count) {
    lseek(this->fileDescriptor, cursor, 0);
    read(this->fileDescriptor, buf, count);
    cursor+= count;
}



ssize_t VDIfile::vdiWrite(void *buf, size_t count) {
   lseek(this->fileDescriptor, this->cursor, 0);//will move the cursor of the file with the given descriptor to the location of our cursor
   write(this->fileDescriptor, buf, count);
   cursor += count; // must move our cursor according to how many bytes we counted.
}



/* I feel like this is a better situation for a switch, but that is kinda arbitrary */
off_t VDIfile::vdiSeek(off_t offset, int anchor) {
    if(anchor == 0) {
        if(offset < this->VDIHeaderInfo.cbDisk && offset >= 0)
            this->cursor = offset;
        else
            return -1;
    }
    else if(anchor == 1) {
        if(this->cursor + offset < this->VDIHeaderInfo.cbDisk && this->cursor + offset >= 0)
            this->cursor = this->cursor + offset;
        else
            return -1;
    }
    else if (anchor == 2) {
        struct stat Temp; // temp structure that holds a variable for the size of the file
        if(fstat(this->fileDescriptor, &Temp) == -1) { // ensuring this worked
            cout << strerror(errno) << endl;/// if it failed print the error
            return -1;
        }
         cursor = Temp.st_size + offset;// set cursor to the size of the file and of the offset
    }
    else {
        return -1;
    }
    return offset;
}


//todo
VDIfile::~VDIfile() {
    delete[] VDITransMapPointer;
}


