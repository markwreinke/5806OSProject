#ifndef VDIFILE_H
#define VDIFILE_H
#include <stdint.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <string>
#include "utilityUUID.h"

using namespace std;

class VDIfile {
    public:
        int ArraySize = 1;
        VDIfile();
        VDIfile(int NewSize);

        virtual ~VDIfile();


        /* This struct holds all of the VDI file header information */
        struct VDIHeaderInfo {
            string imageName;
            uint32_t  u32Signature;
            float version; //todo Idk if float is what we want here or not? I'm not sure if this is the same as line 231 in the virtualbox.org/browser/vbox/trunk/src/VBox/Storage/VDICore.h
            uint32_t cbHeaderSize; // header structure in bytes.
            uint32_t imageType; // line 132 "u32Type"
            uint32_t fFlags; // line 139

            /* todo not sure what this stuff is
             * Virtual CHS
             * Sector size
             * Logical chs
             * sector size
             * map offset
             * frame offset
             * frame size
             * extra frame size
             * total frames
             * frames allocated
             *
             */


            uint64_t diskSize; //disk size in bytes - line 152
            struct utilityUUID::UUID imageUUID;
            struct utilityUUID::UUID lastSnapUUID;
            struct utilityUUID::UUID linkUUID;
            struct utilityUUID::UUID parentUUID;

            /*
             * Image comment
             */



        };

        struct VDIFile *vdiOpen(char *fn)
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

/* todo
        void vdiClose(struct VDIfile *f);
        ssize_t vdiRead(struct VDIfile *f, void *buf, size_t count);
        ssize_t vdiWrite(struct VDIFile *f, void *buf, size_t count);
        off_t vdiSeek(VDIFile *F, off_t offset, int anchor);
*/
        int Capacity(){return ArraySize;}
        void SetCapacity(int NewSize){ArraySize = NewSize;}

    protected:

    private:
        int Descriptor{};
        size_t Cursor;
        //int VDITransMapPointer;

        int *VDITransMapPointer = new int[ArraySize];

};

#endif // VDIFILE_H
