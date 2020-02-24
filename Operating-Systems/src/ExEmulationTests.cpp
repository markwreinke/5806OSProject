//
// Created by csis on 2/8/20.
//

#include "../include/ExEmulationTests.h"


void ExEmulationTests::runEmTest(int step, int example){
    switch(step){
        case 0: if(example == 1){
                    step0Ex1();
                    break;
                } else if(example == 2){
                    step0Ex2();
                    break;
                } else if(example == 3){
                    step0Ex3();
                    break;
                }

        case 1:if(example == 1) {
                step1Ex1();
                break;
                } else if(example == 2){
                step1Ex2();
                break;
                } else if(example == 3){
                step1Ex3();
                break;
                } else if(example == 4){
                step1Ex4();
                break;
                } else if(example == 5){

                }
        case 2:if(example == 1){
                step2Ex1();
                break;
                }
        default:
            cout << "These are not the  step " << step << " example " << example << " emulation tests you are looking for." << endl;
    }
}





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

void ExEmulationTests::step1Ex1() {
    cout << "Displaying Step 1, Example 1" << endl;

    char *filename = "../testFiles/Test-fixed-1k.vdi";

    uint8_t *buffer = new uint8_t[64];
    VDIfile f;
    f.vdiOpen(filename);
    f.vdiSeek(446, SEEK_SET);
    f.vdiRead(buffer, 64);
    StepZDebug::dumpVDIHeader(&f.headerInfo);
    f.vdiClose();
    delete[] buffer;
}

void ExEmulationTests::step1Ex2() {
    cout << "Displaying Step 1, Example 2" << endl;

    char *filename = "../testFiles/Test-fixed-1k.vdi";

    uint8_t *buffer = new uint8_t[64];
    VDIfile f;
    f.vdiOpen(filename);
    f.vdiSeek(446, SEEK_SET);
    f.vdiRead(buffer, 64);
    StepZDebug::displayBuffer(reinterpret_cast<uint8_t *>(&f.headerInfo), 400, 0);
    f.vdiClose();
    delete[] buffer;
}

void ExEmulationTests::step1Ex3(){
    cout << "Displaying Step 1, Example 3" << endl;
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

void ExEmulationTests::step1Ex4(){
    cout << "Displaying Step 1, Example 4" << endl;

    char *filename = "../testFiles/Test-dynamic-1k.vdi";

    uint8_t *buffer = new uint8_t[64];
    VDIfile f;
    f.vdiOpen(filename);
    f.vdiSeek(446, SEEK_SET);
    f.vdiRead(buffer, 64);
    StepZDebug::dumpVDIHeader(&f.headerInfo);
    f.vdiClose();
    delete[] buffer;
}
void ExEmulationTests::step2Ex1(){
    cout << "Displaying Step 2, Example 1" << endl;

    char *filename = "../testFiles/Test-dynamic-1k.vdi";

    VDIfile f;
    f.vdiOpen(filename);

    Partition p;
    p.partitionOpen(&f);
    StepZDebug::dumpPartitionTable(&p.partEntry);
    f.vdiClose();



    /*

    PartitionEntry* PE = new PartitionEntry;
    *PE->partitionEntry[0]+1 = 20;
    PE->partitionEntry[0]+2 = hex(21);
    PE->partitionEntry[0]+4 = hex(83);
    PE->partitionEntry[0]+5 = hex(51);
    PE->partitionEntry[0]+6 = hex(01);
    PE->partitionEntry[0]+7 = hex(10);
    PE->partitionEntry[0]+9 = hex(8);
    PE->partitionEntry[0]+13 = hex(8);
    PE->partitionEntry[0]+14 = hex(03);
    StepZDebug::dumpPartitionTable(PE);

    delete[] PE; */
}