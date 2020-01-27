#include "../include/VDIfile.h"

/* Declaration of constructor with no input */
VDIfile::VDIfile()
{
    VDITransMapPointer = new int{ArraySize};
    Cursor = 0;

}

/* Declaration of constructor with newsize input */
VDIfile::VDIfile(int NewSize)
{
    ArraySize = NewSize;
    VDITransMapPointer = new int{ArraySize};
    Cursor = 0;
}






 VDIfile::VDIHeaderInfo VDIfile::*vdiOpen(char *fn) {
    ofstream OpenFile;
    OpenFile.open(fn);
    if(OpenFile.is_open())
    {
        cout << "The file is open" << endl;

    }
    else
        return nullptr;


};



// todo
void VDIfile::vdiClose(struct VDIfileX *f)
{

}

// todo
ssize_t VDIfile::vdiRead(struct VDIfileX *f, void *buf, size_t count)
{

}


//todo
ssize_t VDIfile::vdiWrite(struct VDIFileX *f, void *buf, size_t count)
{

}


//todo
off_t VDIfile::vdiSeek(VDIFileX *F, off_t offset, int anchor)
{

}


//todo
VDIfile::~VDIfile()
{
    delete[] VDITransMapPointer;
}
