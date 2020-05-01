//
// Created by csis on 2/8/20.
//

#include "../include/ExEmulationTests.h"
#include "../include/Directories.h"


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
                } else if(example == 2) {
                step2Ex2();
                break;
                } else if(example == 3) {
                step2Ex3();
                break;
                }
        case 3: if(example == 1) {
                step3Ex1();
                break;
                } else if(example == 2) {
                step3Ex2();
                break;
                } else if(example == 3) {
                step3Ex3();
                break;
                }
        case 4: if(example == 1) {
                step4Ex1();
                break;
                } else if(example == 2) {
                step4Ex2();
                break;
                }else if(example == 3){
                step4Ex3();
                break;
        }
        case 5: if(example == 1){
                step5Ex1();
                break;
                }else if (example == 2){
                step5Ex2();
                break;
                }
        case 6: if(example == 1){
            step6Ex1();
            break;
        }
        case 7: if(example == 1){
            step7Ex1();
            break;
                }
        case 8: if(example == 1){
            step8Ex1();
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
    VDIFile testVDIfile;
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
    VDIFile testVDIfile;
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
    VDIFile f;
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
    VDIFile f;
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
    VDIFile f;
    f.vdiOpen(filename);
    f.vdiSeek(446, SEEK_SET);
    f.vdiRead(buffer, 64);
    StepZDebug::displayBuffer(reinterpret_cast<uint8_t *>(&f.headerInfo), 400, 0);
    f.vdiClose();
    delete[] buffer;
}

void ExEmulationTests::step1Ex3() {
    cout << "Displaying Step 1, Example 3" << endl;
    char *filename = "../testFiles/Test-fixed-1k.vdi";

    uint8_t *buffer = new uint8_t[64];
    VDIFile f;
    f.vdiOpen(filename);
    f.vdiSeek(446, SEEK_SET);
    f.vdiRead(buffer, 64);
    StepZDebug::displayBufferPage(buffer, 64, 190, 256);
    f.vdiClose();
    delete[] buffer;
}

void ExEmulationTests::step1Ex4() {
    cout << "Displaying Step 1, Example 4" << endl;

    char *filename = "../testFiles/Test-dynamic-1k.vdi";

    uint8_t *buffer = new uint8_t[64];
    VDIFile f;
    f.vdiOpen(filename);
    f.vdiSeek(446, SEEK_SET);
    f.vdiRead(buffer, 64);
    StepZDebug::dumpVDIHeader(&f.headerInfo);
    f.vdiClose();
    delete[] buffer;
}

void ExEmulationTests::step2Ex1() {
    cout << "Displaying Step 2, Example 1 (It's Borked)" << endl;

    char *filename = "../testFiles/Test-dynamic-1k.vdi";
    uint8_t *buffer = new uint8_t[1024];
    VDIFile f;
    f.vdiOpen(filename);

    Partition p;
    p.partitionOpen(&f);
    StepZDebug::dumpPartitionTable(&f,&p.partEntry);
    p.partitionSeek(1024,SEEK_SET);
    p.partitionRead(buffer,1024);
    StepZDebug::displayBuffer(buffer,1024,1024);
    p.partitionClose();
    delete[] buffer;
}

void ExEmulationTests::step2Ex2() {
    cout << "Displaying Step 2, Example 2" << endl;

    char *filename = "../testFiles/Test-fixed-1k.vdi";
    uint8_t *buffer = new uint8_t[1024];
    VDIFile f;
    f.vdiOpen(filename);

    Partition p;
    p.partitionOpen(&f);
    StepZDebug::dumpPartitionTable(&f,&p.partEntry);
    p.partitionSeek(1024,SEEK_SET);
    p.partitionRead(buffer,1024);
    StepZDebug::displayBuffer(buffer,1024,1024);
    p.partitionClose();
    delete[] buffer;
}

void ExEmulationTests::step2Ex3() {
    cout << "Displaying Step 2, Example 3" << endl;

    char *filename = "../testFiles/Test-fixed-4k.vdi";
    uint8_t *buffer = new uint8_t[1024];
    VDIFile f;
    f.vdiOpen(filename);

    Partition p;
    p.partitionOpen(&f);
    StepZDebug::dumpPartitionTable(&f,&p.partEntry);
    p.partitionSeek(1024,SEEK_SET);
    p.partitionRead(buffer,1024);
    StepZDebug::displayBuffer(buffer,1024,1024);
    p.partitionClose();
    delete[] buffer;
}

void ExEmulationTests::step3Ex1() {
    cout << "Displaying Step 3, Example 1" << endl;

    char *filename = "../testFiles/Test-fixed-1k.vdi";
    uint8_t *buffer = new uint8_t[1024];
    Ext2File *ext2File = new Ext2File();
    bool success = ext2File->ext2Open(filename);
    StepZDebug::displayBuffer(reinterpret_cast<uint8_t*>(&ext2File->superBlock), 1024, 1024);
    StepZDebug::dumpSuperBlock(ext2File);
    StepZDebug::dumpBGDT(ext2File);
    ext2File->ext2Close();
    delete[] ext2File;
    delete[] buffer;
}

void ExEmulationTests::step3Ex2() {
    cout << "Displaying Step 3, Example 2" << endl;

    char *filename = "../testFiles/Test-dynamic-1k.vdi";
    uint8_t *buffer = new uint8_t[1024];
    Ext2File *ext2File = new Ext2File();
    bool success = ext2File->ext2Open(filename);
    StepZDebug::displayBuffer(reinterpret_cast<uint8_t*>(&ext2File->superBlock), 1024, 1024);
    StepZDebug::dumpSuperBlock(ext2File);
    StepZDebug::dumpBGDT(ext2File);
    ext2File->ext2Close();
    delete[] ext2File;
    delete[] buffer;
}

void ExEmulationTests::step3Ex3() {
    cout << "Displaying Step 3, Example 3" << endl;

    char *filename = "../testFiles/Test-fixed-4k.vdi";
    uint8_t *buffer = new uint8_t[1024];
    Ext2File *ext2File = new Ext2File();
    bool success = ext2File->ext2Open(filename);
    StepZDebug::displayBuffer(reinterpret_cast<uint8_t*>(&ext2File->superBlock), 1024, 1024);
    StepZDebug::dumpSuperBlock(ext2File);
    StepZDebug::dumpBGDT(ext2File);
    ext2File->ext2Close();
    delete[] ext2File;
    delete[] buffer;
}

void ExEmulationTests::step4Ex1() {
    cout << "Displaying Step 4, Example 1" << endl << endl;

    char *filename = "../testFiles/Test-fixed-4k.vdi";

    Ext2File *ext2File = new Ext2File();
    bool success = ext2File->ext2Open(filename);
    struct InodeStruct *inodeStruct = new InodeStruct;
    Inode::fetchInode(ext2File, 2, inodeStruct);
    StepZDebug::dumpInode(ext2File, *inodeStruct, 2);
    cout << endl;
    Inode::fetchInode(ext2File, 11, inodeStruct);
    StepZDebug::dumpInode(ext2File, *inodeStruct, 11);
    Inode::inodeInUse(ext2File,2);
    Inode::inodeInUse(ext2File,11);
    ext2File->ext2Close();
    delete[] inodeStruct;
    delete[] ext2File;
}

void ExEmulationTests::step4Ex2() {
    cout << "Displaying Step 4 Testing freeInode" << endl << endl;
    char *filename = "../testFiles/Write_Test-fixed-1k.vdi";
    char *reallocationFile = "../testFiles/Test-fixed-1k.vdi";

    Ext2File *ext2File = new Ext2File();
    Ext2File *reExt2File = new Ext2File();

    bool success = ext2File->ext2Open(filename);
    reExt2File->ext2Open(reallocationFile);

    struct InodeStruct *inodeStruct = new InodeStruct;
    struct InodeStruct *reallocationInodeStruct = new InodeStruct;

    /* Display before */
    cout << "**********Showing before freeInode**********" << endl;
    Inode::fetchInode(ext2File, 2, inodeStruct);
    StepZDebug::dumpInode(ext2File, *inodeStruct, 2);
    cout << endl;
    Inode::fetchInode(ext2File, 11, inodeStruct);
    StepZDebug::dumpInode(ext2File, *inodeStruct, 11);

    /* Free inodes 2 and 11 */
    Inode::freeInode(ext2File,2);
    Inode::freeInode(ext2File, 11);

    /* Display after */
    cout << endl << "**********Showing after freeInode*********** " << endl;
    Inode::fetchInode(ext2File, 2, inodeStruct);
    StepZDebug::dumpInode(ext2File, *inodeStruct, 2);
    cout << endl;
    Inode::fetchInode(ext2File, 11, inodeStruct);
    StepZDebug::dumpInode(ext2File, *inodeStruct, 11);

    /* Return the possibly changed file to what it was */
    Inode::fetchInode(reExt2File, 2, reallocationInodeStruct);
    Inode::writeInode(ext2File, 2, reallocationInodeStruct);

    Inode::fetchInode(reExt2File, 11, reallocationInodeStruct);
    Inode::writeInode(ext2File, 11, reallocationInodeStruct);

    reExt2File->ext2Close();
    delete[] reallocationInodeStruct;
    delete[] reExt2File;
    ext2File->ext2Close();
    delete[] inodeStruct;
    delete[] ext2File;
}
void ExEmulationTests::step4Ex3() {
    cout << "Displaying Step 4 Testing inodeInUse" << endl << endl;
    char *filename = "../testFiles/Write_Test-fixed-1k.vdi";
    char *reallocationFile = "../testFiles/Test-fixed-1k.vdi";

    Ext2File *ext2File = new Ext2File();
    Ext2File *reExt2File = new Ext2File();

    bool success = ext2File->ext2Open(filename);
    reExt2File->ext2Open(reallocationFile);

    struct InodeStruct *inodeStruct = new InodeStruct;
    struct InodeStruct *reallocationInodeStruct = new InodeStruct;

    /* Display before */
    cout << "*********First look at inodes, Both are set by default**********" << endl;
    Inode::fetchInode(ext2File, 2, inodeStruct);
    Inode::fetchInode(ext2File, 11, inodeStruct);
    /* Call inodeInUse */
    Inode::inodeInUse(ext2File,2);
    Inode::inodeInUse(ext2File,11);
    cout << endl;
    //test to see if inode 2 and 11 is being freed
    cout << "**********Freeing Inodes, Should return UNSET for both**********" << endl;
    Inode::freeInode(ext2File,2);
    Inode::freeInode(ext2File, 11);
    Inode::inodeInUse(ext2File,2);
    Inode::inodeInUse(ext2File,11);
    cout << endl;

    cout << "**********Resetting inodes (should return to set)********** " << endl;
    Inode::fetchInode(ext2File, 11, inodeStruct);
    Inode::fetchInode(ext2File, 2, inodeStruct);



    /* Return the possibly changed file to what it was */
    Inode::fetchInode(reExt2File, 2, reallocationInodeStruct);
    Inode::writeInode(ext2File, 2, reallocationInodeStruct);

    Inode::fetchInode(reExt2File, 11, reallocationInodeStruct);
    Inode::writeInode(ext2File, 11, reallocationInodeStruct);
    Inode::setInodeToUsed(ext2File,2);
    Inode::setInodeToUsed(ext2File,11);
    Inode::inodeInUse(ext2File,2);
    Inode::inodeInUse(ext2File,11);
    reExt2File->ext2Close();
    delete[] reallocationInodeStruct;
    delete[] reExt2File;
    ext2File->ext2Close();
    delete[] inodeStruct;
    delete[] ext2File;
}
void ExEmulationTests::step5Ex1(){
    cout << "Displaying Step 5, Example 1" << endl;

    char *filename = "../testFiles/Write_Test-fixed-1k.vdi";
    ///open the ext2file with the filename given above
    Ext2File *ext2File = new Ext2File();
    ext2File->ext2Open(filename);
    ///Each of the inodes being displayed given seperate structures
    InodeStruct *inodeTwo = new InodeStruct;
    InodeStruct *inodeEleven = new InodeStruct;
    InodeStruct *inodeTwelve = new InodeStruct;

    ///fill those structures
    Inode::fetchInode(ext2File,2,inodeTwo);
    Inode::fetchInode(ext2File,11,inodeEleven);
    Inode::fetchInode(ext2File,12,inodeTwelve);

    uint8_t *tempBuffer = new uint8_t[ext2File->getBlockSize()];//will display information the size of the block

    StepZDebug::dumpInode(ext2File,*inodeTwo,2);
    cout << endl;
    FileAccess::fetchBlockFromFile(ext2File,0,tempBuffer,2);
    StepZDebug::displayBuffer(tempBuffer,ext2File->getBlockSize(),0);
    cout << endl << endl << endl;

    StepZDebug::dumpInode(ext2File,*inodeEleven,11);
    cout << endl;
    FileAccess::fetchBlockFromFile(ext2File,0,tempBuffer,11);
    StepZDebug::displayBuffer(tempBuffer,ext2File->getBlockSize(),0);
    cout << endl << endl << endl;

    StepZDebug::dumpInode(ext2File,*inodeTwelve,12);
    cout << endl;
    //FileAccess::fetchBlockFromFile(ext2File,0,tempBuffer,12);
    //StepZDebug::displayBuffer(tempBuffer,ext2File->getBlockSize(),0);
    delete[]tempBuffer;
    delete inodeTwo;
    delete inodeTwelve;
    delete inodeEleven;
    delete ext2File;
}
///used to test writing a block to a file, in this case were going to write over inum 11 with inum 2 and then rewrite inum 11
void ExEmulationTests::step5Ex2() {
    cout << "Displaying Step 5, Example 2" << endl;
    char *filename = "../testFiles/Write_Test-fixed-1k.vdi";
    ///open the ext2file with the filename given above
    Ext2File *ext2File = new Ext2File();
    ext2File->ext2Open(filename);
    ///Each of the inodes being displayed given seperate structures
    InodeStruct *inodeTwo = new InodeStruct;
    InodeStruct *inodeEleven = new InodeStruct;
    InodeStruct *inodeTwelve = new InodeStruct;

    ///fill those structures
    Inode::fetchInode(ext2File,2,inodeTwo);
    Inode::fetchInode(ext2File,11,inodeEleven);
    Inode::fetchInode(ext2File,12,inodeTwelve);

    uint8_t *bufferHoldingBlockTwo = new uint8_t[ext2File->getBlockSize()];
    uint8_t *bufferHoldingBlockEleven = new uint8_t[ext2File->getBlockSize()];
    uint8_t *tempBuffer = new uint8_t[ext2File->getBlockSize()];
    ///read those two blocks
    FileAccess::fetchBlockFromFile(ext2File,0,bufferHoldingBlockTwo,2);
    FileAccess::fetchBlockFromFile(ext2File,0,bufferHoldingBlockEleven,11);
    ///display what is in them originally
    cout << "Block Two: " << endl;
    StepZDebug::displayBuffer(bufferHoldingBlockTwo,ext2File->getBlockSize(),0);
    cout << endl << endl << endl;
    cout << "Block Eleven: " << endl;
    StepZDebug::displayBuffer(bufferHoldingBlockEleven,ext2File->getBlockSize(),0);

    ///overwrite block 0 inode 11 and then fetch into the temp buffer, display the temp buffer;
    FileAccess::writeBlockToFile(ext2File,0,bufferHoldingBlockTwo,11);
    FileAccess::fetchBlockFromFile(ext2File,0,tempBuffer,11);
    cout << endl << endl << endl;
    cout << " Updated Block Eleven: (should look like block two) " << endl;
    StepZDebug::displayBuffer(tempBuffer,ext2File->getBlockSize(),0);
    ///rewrite original blockEleven into block eleven, read into the temp buffer and then display once more
    FileAccess::writeBlockToFile(ext2File,0,bufferHoldingBlockEleven,11);
    FileAccess::fetchBlockFromFile(ext2File,0,tempBuffer,11);
    cout << endl << endl << endl;
    cout << " Updated Block Eleven: (should look like original block elevel) " << endl;
    StepZDebug::displayBuffer(tempBuffer,ext2File->getBlockSize(),0);
}
void ExEmulationTests::step6Ex1() {
    cout << "Displaying Step 6, Example 1" << endl;

    char *filename = "../testFiles/Test-fixed-1k.vdi";
    ///open the ext2file with the filename given above
    Ext2File *ext2File = new Ext2File();
    ext2File->ext2Open(filename);

    char name[256];
    uint32_t iNum;
    Directory *d;

    cout << "Root directory contents" << endl;
    d = Directories::openDirectory(ext2File,2);
    iNum = 2;
    while(Directories::getNextDirent(d,iNum,name)){
        cout << "Inode: " << iNum << "     name: [" << name << ']' << endl;
    }
    Directories::closeDir(d);

    cout << "\n\nlast+found directory contents" << endl;
    d = Directories::openDirectory(ext2File,11);
    iNum = 11;
    while(Directories::getNextDirent(d,iNum,name)){
        if(iNum != 0)
            cout << "Inode: " << iNum << "     name: [" << name << ']' << endl;
    }
    Directories::closeDir(d);
}
void ExEmulationTests::step7Ex1() {
    cout << "Displaying Step 7, Example 1" << endl;

    char *filename = "../testFiles/Test-fixed-1k.vdi";
    ///open the ext2file with the filename given above
    Ext2File *ext2File = new Ext2File();
    ext2File->ext2Open(filename);
    char *filepathWanted = "/examples/02.Digital/toneMelody/toneMelody.ino";

    char name[256];
    uint32_t iNum = 2;
    Directory *d;

    d = Directories::openDirectory(ext2File, 2);
    uint32_t returnedINum = FilePaths::traversePath(ext2File, filepathWanted);
    StepZDebug::dumpInode(ext2File, d->iS, returnedINum);
}

///used to print all the VDIDirectories in filename
void ExEmulationTests::step8Ex1(){
    cout << "Displaying all inode information" << endl;
    char *filename = "../testFiles/Test-fixed-1k.vdi";
    copyFile::viewVDIDirectories(filename);
}