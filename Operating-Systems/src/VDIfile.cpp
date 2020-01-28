#include "../include/VDIfile.h"



/* Declaration of constructor with transMapSize input. Because it is explicit, it must be called VDIfile(int 3)*/
VDIfile::VDIfile(int transSize) {
    transMapSize = transSize;
    VDITransMapPointer = new int{transMapSize};
    cursor = 0;
}


/* This loads the header information and such from the given vdi file. Because VDIfile is a class, returns a boolean instead of a pointer. */
bool VDIfile::vdiOpen(char *fn) {
    ofstream OpenFile;
    OpenFile.open(fn);
    if(OpenFile.is_open()) {
        cout << "The file is open" << endl; // todo remove when done

    }
    else {
        return false;
    }

};

/*

// todo
void VDIfile::vdiClose(struct VDIfile *f)
{

}

// todo
ssize_t VDIfile::vdiRead(struct VDIfile *f, void *buf, size_t count)
{

}


//todo
ssize_t VDIfile::vdiWrite(struct VDIFile *f, void *buf, size_t count)
{

}


//todo
off_t VDIfile::vdiSeek(VDIFile *F, off_t offset, int anchor)
{

}
*/

//todo
VDIfile::~VDIfile()
{
    delete[] VDITransMapPointer;
}


