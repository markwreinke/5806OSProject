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


        explicit VDIfile(int = 1);

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

        bool vdiOpen(char *fn);

/* todo
        void vdiClose(struct VDIfile *f);
        ssize_t vdiRead(struct VDIfile *f, void *buf, size_t count);
        ssize_t vdiWrite(struct VDIFile *f, void *buf, size_t count);
        off_t vdiSeek(VDIFile *F, off_t offset, int anchor);
*/
        int Capacity(){return transMapSize;}
        void SetCapacity(int NewSize){transMapSize = NewSize;}

    protected:

    private:
        /* This is to set the size of the VDI translation map array */
        int transMapSize;

        int Descriptor{};
        size_t cursor;
        //int VDITransMapPointer;

        int *VDITransMapPointer = new int[transMapSize];

};

#endif // VDIFILE_H
