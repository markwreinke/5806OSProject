#include "../include/VDIfile.h"


VDIfile::VDIfile()
{
    VDITransMapPointer = new int{ArraySize};
    Cursor = 0;

}
VDIfile::VDIfile(int NewSize)
{
    ArraySize = NewSize;
    VDITransMapPointer = new int{ArraySize};
    Cursor = 0;
}
struct VDIFileX VDIfile::*vdiOpen(char *fn)
{
    ofstream OpenFile;
    OpenFile.open(fn);
    if(OpenFile.is_open())
    {
        cout << "The file is open" << endl;

    }
    else
        return nullptr;


};
void VDIfile::vdiClose(struct VDIfileX *f)
{

}
ssize_t VDIfile::vdiRead(struct VDIfileX *f, void *buf, size_t count)
{

}
ssize_t VDIfile::vdiWrite(struct VDIFileX *f, void *buf, size_t count)
{

}
off_t VDIfile::vdiSeek(VDIFileX *F, off_t offset, int anchor)
{

}

VDIfile::~VDIfile()
{
    delete[] VDITransMapPointer;
}
