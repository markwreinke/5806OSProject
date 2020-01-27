//
// Created by csis on 1/26/20.
//

#include "../include/StepZDebug.h"

void StepZDebug::displayBufferPage(uint8_t *buf,uint32_t count, uint32_t start,uint64_t offset) {
    int width = 16;
    int height = 0;
    int loop = 16;
    int cursor = start;
    int cursor2 = start;

    cout << "Offset: 0x" << offset << endl;
    cout << "   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f     0...4...8...c..." <<endl;
    cout << "  +------------------------------------------------+  +----------------+" <<endl;

    while(height < 16) {

        cout << hex << setfill('0')<< setw(2) << height << "|";
        while (width > 0) {
            if (start <= offset && offset < start + count && count > 0) {
                printf("%02x", buf[cursor]);
                cout << " ";
                cursor++;
            } else {
                cout << "   ";
            }
            width--;
            count--;
        }
        cout<< hex << "|" << setfill('0') << setw(2) << height << "|";

        while(loop > 0)
        {
            if(start <= offset && offset < start + count && count > 0 && isprint(buf[cursor2])){
                cout << static_cast<uint8_t>(buf[cursor2]);
            } else{
                cout << " ";
            }
            loop--;
            cursor2++;
        }
        cout << "|" << endl;
        height++;
        width = 16;
        loop = 16;
    }
    cout << "  +------------------------------------------------+  +----------------+" <<endl;
}





void StepZDebug::displayBuffer(uint8_t *buf, uint32_t count, uint64_t offset) {
    int start = 0;

    while(count > 0) {
        if(count > 256){
            displayBufferPage(buf,256,start,offset);
            start += 256;
            count -= 256;
            offset += 256;
        } else{
            displayBufferPage(buf,count,start,offset);
            count = 0;
        }
    }
}



