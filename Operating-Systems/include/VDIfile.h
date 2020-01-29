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

        /* Declaration of constructor with transMapSize input. Because it is explicit, it must be called VDIfile(int 3)*/
        explicit VDIfile(int = 1);

        virtual ~VDIfile();


        /* This struct holds all of the VDI file header information */
        struct VDIHeaderInfo {
            char imageName[64];

            /* This is the preheader information */
            char szFileInfo[64];
            uint32_t u32Signature;
            uint32_t u32Version;

            /* This is the rest of the file header info - for VDIHEADER1PLUS */
            uint32_t cbHeader; // size of this header in bytes
            uint32_t imageType; // line 132 "u32Type"
            uint32_t fFlags; // line 139
            char szComment[256]; //Image comment UTF-8



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


        /* This loads the header information and such from the given vdi file. Because VDIfile is a class, returns a boolean instead of a pointer. */
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
