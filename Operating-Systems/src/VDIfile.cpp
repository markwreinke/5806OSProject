#include "VDIfile.h"

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
/*struct VDIFileX *vdiOpen(char *fn)
{
    ofstream OpenFile;
    OpenFile.open(fn);
    if(OpenFile.is_open())
    {
        cout << "The file is open" << endl;

    }
    else
        return nullptr;


};*/
void vdiClose(struct VDIfileX *f)
{
    ofstream CloseFile = f->OpenFile;
    CloseFile.close();
}
ssize_t vdiRead(struct VDIfileX *f, void *buf, size_t count)
{

}
ssize_t vdiWrite(struct VDIFileX *f, void *buf, size_t count)
{

}
off_t vdiSeek(VDIFileX *F, off_t offset, int anchor)
{

}

VDIfile::~VDIfile()
{
    delete[] VDITransMapPointer;
}
