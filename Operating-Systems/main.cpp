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
    ExEmulationTests::step0Ex2();

   /* uint8_t buffer[64];

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
