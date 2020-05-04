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



int main(int argc, char* argv[]) {
    if(argc <= 1) {
        cout << "Unbelievably unacceptable arguments. To copy a file from the VDI filesystem to the Host system, you must type: \"-tohost\" followed by the paths for: VDI file, then the source file from the VDI system, then the destination in the host system. The destination path must include the name that you want the file to have (even if you want it to be named the same as its counterpart within the VDI file system)" << endl;
        cout << endl;
        cout << "To copy a file from the Host system to the VDI file system, you must type: \"-tovdi\" followed by the paths for VDI file and then the source file from the host system." << endl;
        cout << endl;
        cout << "To display the VDI file system, you must type: \"-display\" followed by the path for the VDI file." << endl;
        cout << endl;
        cout << "Have a good day, and shoot for the stars." << endl;
        return -1;
    }

   if(strcmp(argv[1], "-tohost")){
       if(argc != 5) {
           cout << "Inadequate arguments. To copy a file from the VDI filesystem to the Host system, you must type: \"-tohost\" followed by the paths for: VDI file, then the source file from the VDI system, then the destination in the host system. The destination path must include the name that you want the file to have (even if you want it to be named the same as its counterpart within the VDI file system)" << endl;
       }
       char *VDIFileName = argv[2];
       char *srcFileName = argv[3];
       char *destFileName = argv[4];
       copyFile::copyFileToHost(VDIFileName, srcFileName, destFileName);
   } else if(strcmp(argv[1], "-tovdi")) {
       if(argc != 4) {
           cout << "Inadequate arguments. To copy a file from the Host system to the VDI file system, you must type: \"-tovdi\" followed by the paths for VDI file and then the source file from the host system." << endl;
       }
       char *VDIFileName = argv[2];
       char *srcFileName = argv[3];
       copyFile::copyFileToVDI(VDIFileName, srcFileName);
   } else if(strcmp(argv[1], "-display")) {
       if(argc != 3) {
           cout << "Inadequate arguments. To display the VDI file system, you must type: \"-display\" followed by the path for the VDI file." << endl;
       }
       char *VDIFileName = argv[2];
       copyFile::viewVDIDirectories(VDIFileName);
   } else {
       cout << "Unbelievably unacceptable arguments. To copy a file from the VDI filesystem to the Host system, you must type: \"-tohost\" followed by the paths for: VDI file, then the source file from the VDI system, then the destination in the host system. The destination path must include the name that you want the file to have (even if you want it to be named the same as its counterpart within the VDI file system)" << endl;
       cout << endl;
       cout << "To copy a file from the Host system to the VDI file system, you must type: \"-tovdi\" followed by the paths for VDI file and then the source file from the host system." << endl;
       cout << endl;
       cout << "To display the VDI file system, you must type: \"-display\" followed by the path for the VDI file." << endl;
       cout << endl;
       cout << "Have a good day, and shoot for the stars." << endl;
   }
   return 0;
}
