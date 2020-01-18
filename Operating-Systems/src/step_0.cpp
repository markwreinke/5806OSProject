#include "step_0.h"


step_0::step_0(char *fn)
{
    FileName = fn;
}
int step_0::open2(char *fn)
{
    int FileNameInt = open(fn,O_TRUNC);
    if(FileNameInt == -1)
    {
        cout <<"File failed to open"<<endl;
        return -1;
    }
    else
    {
        cout <<*fn << " has opened" << endl;
        return FileNameInt;
    }
}
void step_0::closeFile(int fd)
{

}

ssize_t step_0::read(int fd, void *buf, size_t count)
{

}
ssize_t step_0::write(int fd, void *buf, size_t count)
{

}
off_t step_0::lseek(int fd, off_t offset, int anchor)
{

}
void step_0::displayBufferPage(uint8_t *buf,uint32_t count, uint32_t start,uint64_t offset)
{
    offset = 0;
    int width = 16;
    int height = 0;
    int loop = 4;
    uint8_t buffer = '!'; //will equal whatever is is buffer
    cout << "Offset: 0x" << offset << endl;
    cout << "   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f     0...4...8...c..." <<endl;
    cout << "  +------------------------------------------------+  +----------------+" <<endl;
    while(height < 16)
    {
        uint8_t temp = buffer;
        cout << hex << setfill('0')<< setw(2) << height << "|";
        while(width > 0)
        {
        cout << hex <<setfill('0') <<setw(2)<< static_cast<int>(buffer) << " ";
        width--;
        buffer++;
        }
        cout<< hex << "|" << setfill('0') << setw(2) << height << "|";
        while(loop > 0)
        {
            if(isprint(buffer))
            {
             cout << temp <<"   ";
            }
            else
            {
                cout << "    ";
            }
            temp++;
            loop--;
        }
        cout << "|" << endl;
        height++;
        buffer++;
        width = 16;
        loop = 4;
    }
    cout << "  +------------------------------------------------+  +----------------+" <<endl;
}
void step_0::displayBuffer(uint8_t *buf,uint32_t count,uint64_t offset)
{
    int start = 0;
    uint8_t* buff = buf;
    while(count > 0)
    {
        displayBufferPage(buff,count,start,offset);
        start +=1;
        count--;
    }
}
