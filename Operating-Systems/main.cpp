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



int main()
{
   uint8_t *FileInformation;
   size_t BytesToCount = 500;
   int BytesRead;

   char* filename = "../experimentFiles/Testdoc.txt";
   cout << filename << endl;
   int FileDescriptor = open(filename, O_RDWR);
   cout << FileDescriptor << endl;
   cout << strerror(errno);

   FileInformation = (uint8_t*) malloc(100 * sizeof(uint8_t));
   BytesRead = read(FileDescriptor, FileInformation, BytesToCount);
   cout << "number of bytes read: " << BytesRead << endl;

   StepZDebug::displayBuffer(FileInformation, 400, 0);


}
