#include <iostream>
#include <iomanip>
#include <VDIfile.h>
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
    int height = 0;
    int loop = 16;
    int Cursor = start;
    int Cursor2 = start;
    cout << "Offset: 0x" << offset << endl;
    cout << "   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f     0...4...8...c..." <<endl;
    cout << "  +------------------------------------------------+  +----------------+" <<endl;

    while(height < 16)
    {
        cout << hex << setfill('0')<< setw(2) << height << "|";
        while(width > 0)
        {
            if(start <= offset && offset < start + count  && count > 0)
            {
                printf("%02x", buf[Cursor]);///had to use printf to get buf to print as a hex
                cout << " ";
                Cursor++;
            }
            else
            {
                cout << "   ";
            }
            width--;
            count = count - 1;
        }
        cout<< hex << "|" << setfill('0') << setw(2) << height << "|";
        while(loop > 0)
        {
            if(start <= offset && offset < start + count  && count > 0 && isprint(buf[Cursor2]))
            {
             cout << static_cast<uint8_t>(buf[Cursor2]);
            }
            else
            {
                cout << " ";
            }
            loop--;
            Cursor2++;
        }
        cout << "|" << endl;
        height++;
        width = 16;
        loop = 16;
    }
    cout << "  +------------------------------------------------+  +----------------+" <<endl;
}
void displayBuffer(uint8_t *buf,uint32_t count,uint64_t offset)
{
    int start = 0;
    while(count > 0)
    {
        if(count > 256)
        {
        displayBufferPage(buf,256,start,offset);
        start += 256;
        count = count - 256;
        offset+= 256;

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
    int BytesRead; /// this is returned by read,Its how many bytes were read
    //char *BytesToWrite;
    //cout << "write something into the file" << endl;
    //cin >> BytesToWrite;
    ///go to https://virtualboxes.org/images/ and click the ArchLinux link, I chose the second option because its relatively small.
    ///After downloading unzip the file and add the VDI to wherever the main for the project is stored

    char* filename = "Archlinux-2011.08.19-i686.vdi";///if you can get a VDI file to open with this please lemme know how, i couldnt get it working
    cout << filename << endl;
    int FileDescriptor = open(filename,O_RDWR);
    cout << FileDescriptor << endl;
    cout << strerror(errno);

    FileInformation = (uint8_t*) malloc(100 * sizeof(uint8_t));///allocate memory for the FileInformation
    BytesRead = read(FileDescriptor, FileInformation, BytesToCount);
    cout << endl <<"number of bytes read: " << BytesRead << endl;
    //cout << "those bytes say:   " << FileInformation << endl;

    ///testing the write method
    //BytesWritten = write(FileDescriptor,BytesToWrite, strlen(BytesToWrite));
    //cout <<"Number of bytes written: " << BytesWritten << endl;
    ///for some reason after reading the file earlier the next read wont catch the changes to the file.
    //BytesRead = read(FileDescriptor, FileInformation, BytesToCount);
    //cout << endl<< "after writing" << endl;
    //cout  <<"number of bytes read: " << BytesRead << endl;
    //cout << "those bytes say:   " << FileInformation << endl;
    //cout << BytesToWrite <<endl << endl;

    displayBuffer(FileInformation, 400, 0);
    return 0;
}
