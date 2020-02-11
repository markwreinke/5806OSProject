//
// Created by csis on 2/8/20.
//

#include "../include/ExEmulationTests.h"

void ExEmulationTests::step0Ex1() {
    cout << "Displaying Step 0, Example 1" << endl;

    uint8_t *FileInformation;
    size_t BytesToCount = 500;
    int BytesRead;

    char *filename = "../testFiles/Test-fixed-1k.vdi";

    cout << filename << endl;
    int FileDescriptor = open(filename, O_RDWR);
    /*cout << FileDescriptor << endl;
    cout << strerror(errno);
    */

    FileInformation = (uint8_t*) malloc(100 * sizeof(uint8_t));
    read(FileDescriptor, FileInformation, BytesToCount);


    StepZDebug::displayBuffer(FileInformation, 400, 0);
    delete[] FileInformation;
    close(FileDescriptor);
}

void ExEmulationTests::step0Ex2() {
    cout << "Displaying Step 0, Example 2" << endl;
    uint8_t *FileInformation;

    char *filename = "../testFiles/Test-fixed-1k.vdi";
    int FileDescriptor = open(filename, O_RDWR);
    VDIfile testVDIfile;
    testVDIfile.vdiOpen(filename);

    size_t BytesToCount = 256; // #todo figure out how tp dynamically?
    FileInformation = (uint8_t*) malloc(100 * sizeof(uint8_t));
    lseek(FileDescriptor,testVDIfile.headerInfo.offBlocks,SEEK_SET);
    read(FileDescriptor, FileInformation, BytesToCount);


    StepZDebug::displayBufferPage(FileInformation, 256, 0, testVDIfile.headerInfo.offBlocks);
    delete[] FileInformation;
    close(FileDescriptor);
}

void ExEmulationTests::step0Ex3() {
    cout << "Displaying Step 0, Example 3" << endl;


    /// Cursed Fuckery
    /*uint8_t *buffer = new uint8_t[64];
    char *filename = "../testFiles/Test-fixed-1k.vdi";

    int FileDescripter = open(filename, O_RDWR);
    VDIfile testVDIfile;
    testVDIfile.vdiOpen(filename);

    uint8_t *FileInformation;
    FileInformation = (uint8_t*)malloc(100 * sizeof(uint8_t));
    lseek(FileDescripter, 446, SEEK_SET);
    read(FileDescripter, buffer, 64);




    StepZDebug::displayBufferPage(buffer, 64, 190,256);
    delete[] FileInformation;
    close(FileDescripter);
    delete[] buffer;*/

    char *filename = "../testFiles/Test-fixed-1k.vdi";

    uint8_t *buffer = new uint8_t[64];
    VDIfile f;
    f.vdiOpen(filename);
    f.vdiSeek(446, SEEK_SET);
    f.vdiRead(buffer, 64);
    StepZDebug::displayBufferPage(buffer, 64, 190, 256);
    f.vdiClose();
    delete[] buffer;








}