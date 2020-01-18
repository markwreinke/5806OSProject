#include <iostream>
#include <iomanip>
#include <VDIfile.h>
#include "step_0.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void displayBufferPage(uint8_t *buf,uint32_t count, uint32_t start,uint64_t offset)
{
    int width = 16;
    int height = 16;
    int loop = 4;
    int Cursor = 0;
    cout << "Offset: 0x" << offset << endl;
    cout << "   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f     0...4...8...c..." <<endl;
    cout << "  +------------------------------------------------+  +----------------+" <<endl;
    while(height > 0)
    {
        uint8_t *temp;
        for(int n = 0; n<16; n++)
        {
           temp[n] = buf[n];
        }

        cout << hex << setfill('0')<< setw(2) << height << "|";
        while(width > 0)
        {
            if(start <= offset && offset < start + count && count > 0)
            {
                cout << hex << setfill('0') <<setw(2)<< static_cast<uint8_t>(buf[Cursor]) << " ";
            }
            else
            {
                cout << "   ";
            }
            width--;
            Cursor++;
        }
        cout<< hex << "|" << setfill('0') << setw(2) << height << "|";
        while(loop > 0)
        {
            int Cursor2 = 0;
            if(isprint(buf[Cursor2]))
            {
             cout << temp[Cursor2] <<"   ";
            }
            else
            {
                cout << "    ";
            }
            loop--;
            Cursor2++;
        }
        cout << "|" << endl;
        height--;
        width = 16;
        loop = 4;
    }
    cout << "  +------------------------------------------------+  +----------------+" <<endl;
}
void displayBuffer(uint8_t *buf,uint32_t count,uint64_t offset)
{
    int start = 0;
    //uint8_t* buff = buf;
    while(count > 0)
    {
        if(count > 256)
        {
        displayBufferPage(buf,256,start,offset);
        start += 256;
        count -+ 256;
        offset = 0;

        }
        else{
            displayBufferPage(buf, count, start, offset);
            count -= count;
        }

    }
}
int main()
{
    uint8_t *FileInformation;/// this will become the name of the buffer we want to display, for now its not dynamic
    size_t BytesToCount = 500;/// how many bytes to count, will be dynamic later, currently testing
    int BytesRead, BytesWritten; /// this is returned by read,Its how many bytes were read
    //char *BytesToWrite;
    //cout << "write something into the file" << endl;
    //cin >> BytesToWrite;
    char* filename = "Testdoc.txt";///if you can get a VDI file to open with this please lemme know how, i couldnt get it working
    cout << filename << endl;
    int FileDescriptor = open(filename,O_RDWR);
    cout << FileDescriptor << endl;
    cout << strerror(errno);

    FileInformation = (uint8_t*) malloc(100 * sizeof(uint8_t));///allocate memory for the FileInformation
    BytesRead = read(FileDescriptor, FileInformation, BytesToCount);
    cout << endl <<"number of bytes read: " << BytesRead << endl;
    cout << "those bytes say:   " << FileInformation << endl;

    ///testing the write method
    //BytesWritten = write(FileDescriptor,BytesToWrite, strlen(BytesToWrite));
    //cout <<"Number of bytes written: " << BytesWritten << endl;
    ///for some reason after reading the file earlier the next read wont catch the changes to the file.
    //BytesRead = read(FileDescriptor, FileInformation, BytesToCount);
    //cout << endl<< "after writing" << endl;
    //cout  <<"number of bytes read: " << BytesRead << endl;
    //cout << "those bytes say:   " << FileInformation << endl;
    //cout << BytesToWrite <<endl << endl;

    displayBuffer(FileInformation, 256, 0);
}
