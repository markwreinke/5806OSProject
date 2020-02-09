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
    int countDracula = count;

    /* This is the header of the display table */
    cout << hex << "Offset: 0x" << offset << endl;
    cout << "   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f     0...4...8...c..." <<endl;
    cout << "  +------------------------------------------------+  +----------------+" <<endl;

    /* Loop iterates once for each row */
    while(height < 16) {

        /* Fills out the row label */
        cout << hex << setfill('0')<< setw(2) << height << "|";

        /* This handles the left side of the table with the hex numbers */
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

        /* Divisor between left/right hex/char sides of the table */
        cout<< hex << "|" << setfill('0') << setw(2) << height << "|";

        /* This handles the right side of the table with the characters */
        while(loop > 0)
        {
            if(start <= offset && offset < start + countDracula && countDracula > 0 && isprint(buf[cursor2])){
                cout << static_cast<uint8_t>(buf[cursor2]);
            } else{
                cout << " ";
            }
            loop--;
            cursor2++;
            countDracula--;
        }

        /* The right border of the table */
        cout << "|" << endl;

        /* Incrementing values for the next row */
        height++;
        width = 16;
        loop = 16;
    }

    /* The bottom of the table */
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

void StepZDebug::dumpVDIHeader(struct VDIHeaderInfo* headerInfo) {
    cout << "Image Name: " << headerInfo->szFileInfo << endl;
    cout << "Signature: 0x" << hex << headerInfo->u32Signature << endl;
    cout << "Version: " << hex << headerInfo->u32Version << endl;
    cout << "Header Size: 0x" << hex << setw(8) <<  setfill('0') << headerInfo->cbHeader << "   " << dec << headerInfo->cbHeader << endl;
    cout << "offblocks: " << hex << headerInfo->offBlocks << endl;
    ///displayBufferPage(headerInfo->szComment, 256, 0, 0);
}



