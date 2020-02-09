//
// Created by csis on 2/8/20.
//

#include "../include/ExEmulationTests.h"

void ExEmulationTests::step0Ex1() {
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
}

void ExEmulationTests::step0Ex2() {
    uint8_t *FileInformation;
    size_t BytesToCount = 472;

    char *filename = "../testFiles/Test-fixed-1k.vdi";
    int FileDescriptor = open(filename, O_RDWR);
    VDIfile testVDIfile;
    testVDIfile.vdiOpen(filename);

    FileInformation = (uint8_t*) malloc(100 * sizeof(uint8_t));
    read(FileDescriptor, FileInformation, BytesToCount);

    cout << "test: " << testVDIfile.headerInfo.offBlocks << endl;

    StepZDebug::displayBufferPage(FileInformation, sizeof(FileInformation), 0, testVDIfile.headerInfo.offBlocks);
}
