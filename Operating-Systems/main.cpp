/** "
 *  This project was made for the Spring 2020 semester of CSCI 5806 Operating Class at Youngstown State University, under the professor Dr. Bob Kramer.
 *
 *  This project was made by Mark Reinke and Chase Hovel
 *
 *  The purpose of the program is to copy files back and forth from a host system and a VDI file. There are three commands:
 *
 *  -tohost VDIFilePath srcFileInVDIPath destPath
 *      Retrieve a file from the given VDI file (VDIFilePath) by giving the absolute path name of the wanted file (srcFileInVDIPath)
 *      and the absolute path (including the desired file name) for the destination of the copied file (destPath).
 *
 *  -tovdi VDIFilePath srcFilePath
 *      Insert a file into the given VDI file (VDIFilePath) by giving the absolute path name of the wanted file (srcFilePath).
 *
 *  -display VDIFilePath
 *      Display the files within the given VDI file (VDIFilePath)
 *
 *
 *  Thank you for viewing our project, and we believe that, at the very least, your computer shouldn't catch fire upon its execution.
 *  (Though we pride ourselves of having done a better job than that :) )
 *
 * **/

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
        return 0;
    }
    char* command = argv[1];
   if(strcmp(command, "-tohost") == 0) {
       if(argc != 5) {
           cout << "Inadequate arguments. To copy a file from the VDI filesystem to the Host system, you must type: \"-tohost\" followed by the paths for: VDI file, then the source file from the VDI system, then the destination in the host system. The destination path must include the name that you want the file to have (even if you want it to be named the same as its counterpart within the VDI file system)" << endl;
            return  0;
       }
       char *VDIFileName = argv[2];
       char *srcFileName = argv[3];
       char *destFileName = argv[4];
       copyFile::copyFileToHost(VDIFileName, srcFileName, destFileName);
   } else if(strcmp(command, "-tovdi") == 0) {
       if(argc != 4) {
           cout << "Inadequate arguments. To copy a file from the Host system to the VDI file system, you must type: \"-tovdi\" followed by the paths for VDI file and then the source file from the host system." << endl;
        return 0;
       }
       char *VDIFileName = argv[2];
       char *srcFileName = argv[3];
       copyFile::copyFileToVDI(VDIFileName, srcFileName);
   } else if(strcmp(command, "-display") == 0) {
       if(argc != 3) {
           cout << "Inadequate arguments. To display the VDI file system, you must type: \"-display\" followed by the path for the VDI file." << endl;
           return 0;
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
