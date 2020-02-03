#include <iostream>
#include <iomanip>
#include "src/VDIfile.cpp"
#include "include/VDIfile.h"
#include "include/StepZDebug.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;



int main() {
  /* uint8_t *FileInformation;
   size_t BytesToCount = 500;
   int BytesRead;
*/
   char* filename = "../testFiles/Test-fixed-4k.vdi";
/*
   cout << filename << endl;
   int FileDescriptor = open(filename, O_RDWR);
   cout << FileDescriptor << endl;
   cout << strerror(errno);

   FileInformation = (uint8_t*) malloc(100 * sizeof(uint8_t));
   BytesRead = read(FileDescriptor, FileInformation, BytesToCount);
   cout << "number of bytes read: " << BytesRead << endl;

   StepZDebug::displayBuffer(FileInformation, 400, 0);
*/
  uint8_t buffer[64];

  VDIfile f;
  bool i = f.vdiOpen(filename);
  cout << i << endl;
  f.vdiSeek(446, 0);
  f.vdiRead(buffer, 64);
  StepZDebug::displayBufferPage(buffer, 64, 190, 256);





}
