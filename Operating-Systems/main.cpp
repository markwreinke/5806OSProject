#include <iostream>
#include <iomanip>
#include "src/VDIFile.cpp"
#include "include/VDIFile.h"
#include "include/StepZDebug.h"
#include "include/ExEmulationTests.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;



int main() {
   ExEmulationTests::runEmTest(8,4);
    //char* fileName = "../testFiles/Write_Test-fixed-1k.vdi";
    //copyFile::viewVDIDirectories(fileName);
}
