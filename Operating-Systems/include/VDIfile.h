#ifndef VDIFILE_H
#define VDIFILE_H
#include <stdint.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
using namespace std;

class VDIfile {
    public:
        int ArraySize=1;


        VDIfile();


        VDIfile(int NewSize);
        virtual ~VDIfile();

        struct VDIHeader
        {
            uint16_t preheader;
            uint64_t Descriptor;

        };

        struct VDIFileX *vdiOpen(char *fn)
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
        void vdiClose(struct VDIfileX *f);
        ssize_t vdiRead(struct VDIfileX *f, void *buf, size_t count);
        ssize_t vdiWrite(struct VDIFileX *f, void *buf, size_t count);
        off_t vdiSeek(VDIFileX *F, off_t offset, int anchor);

        int Capacity(){return ArraySize;}
        void SetCapacity(int NewSize){ArraySize = NewSize;}

    protected:

    private:
        int Descriptor;
        size_t Cursor;
        //int VDITransMapPointer;

        int *VDITransMapPointer = new int[ArraySize];

};

#endif // VDIFILE_H
