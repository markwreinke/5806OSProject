#include <fcntl.h>
#include "../include/VDIfile.h"
#include <unistd.h>


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


        /* This loop populates the file info/image type from the preheader */
        for(int i = 0; i < 64; i++){
            uint8_t temp = read(fileDescriptor, tempBuffer, 1);
            VDIHeaderInfo.szFileInfo[i] = (char) temp;
        }
        VDIHeaderInfo.u32Signature = read(fileDescriptor, tempBuffer, 4);
        VDIHeaderInfo.u32Version = read(fileDescriptor, tempBuffer, 4);




        /*
         * todo Continue filling values
         */

        delete[] tempBuffer; // Deallocate memory used by tempBuffer.





    }
    /* todo
        int errorcheck = read(fileDescriptor, VDIHeaderInfo, 400);
        vdiheaderinfo isnt an acceptable replacement for the void*
        unsure how the read would work
    */
};

/*

// todo
void VDIfile::vdiClose()
{
    close(this->descriptor);
    delete[] vdipointertransmap;
}

// todo
ssize_t VDIfile::vdiRead(void *buf, size_t count)
{

}
*/


ssize_t VDIfile::vdiWrite(void *buf, size_t count)
{
   lseek(this->fileDescriptor, this->cursor, 0);//will move the cursor of the file with the given descriptor to the location of our cursor
    write(this->fileDescriptor, buf, count);
}



off_t VDIfile::vdiSeek(off_t offset, int anchor)
{
    if(anchor == 0)
    {
        if(offset < this->VDIHeaderInfo.cbDisk && offset >= 0)
            this->cursor = offset;
        else
            return -1;
    }
    else if(anchor == 1)
    {
        if(this->cursor + offset < this->VDIHeaderInfo.cbDisk && this->cursor + offset >= 0)
            this->cursor = this->cursor + offset;
        else
            return -1;
    }
    else if (anchor == 2)
    {
        /*todo
            find location of end of file + offset and ensure its no larger than the disk
            if conditions are met move cursor to the location, else return -1
            Not sure how to find the end of file location
        */
    }
    else
    {
        return -1;
    }
}


//todo
VDIfile::~VDIfile()
{
    delete[] VDITransMapPointer;
}


