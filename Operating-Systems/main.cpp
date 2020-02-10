#include <iostream>
#include <iomanip>
#include "src/VDIfile.cpp"
#include "include/VDIfile.h"
#include "include/StepZDebug.h"
#include "include/ExEmulationTests.h"""
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;



int main() {
    //ExEmulationTests::step0Ex2();
    int SizeOfBlock = 16 ;///this->headerInfo.cbBlock;
    int bytesJustRead = 0;
    int bytesRead = 0;
    void *buf = new char [16];
    for(int x = 0; x < SizeOfBlock; x++) {
        *(static_cast<char *>(buf) + (bytesRead + bytesJustRead)) ='0';
        cout << *(static_cast<char *>(buf) + (bytesRead + bytesJustRead)) << endl;
        cout << "BytesJustRead: " << bytesJustRead << endl;
        bytesJustRead++;
    }
    /* uint8_t buffer[64];
 dedZeros.str() << end;

     VDIfile f;
     bool i = f.vdiOpen(filename);

     StepZDebug::dumpVDIHeader(&f.headerInfo);
     cout << i << endl;
     f.vdiSeek(446, SEEK_SET);
     f.vdiRead(buffer, 64);

     for (int x = 0; x < 64; x++) {
         printf("%02x", buffer[x]);
         cout << endl;
     }
   StepZDebug::displayBufferPage(buffer, 64, 190, 256);*/





}
