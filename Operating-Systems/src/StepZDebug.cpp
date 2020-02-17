//
// Created by csis on 1/26/20.
//

#include "../include/StepZDebug.h"

void StepZDebug::displayBufferPage(uint8_t *buf,uint32_t count, uint32_t start,uint64_t offset) {

    int height = 0;

    int hexWidth = 16;
    int charWidth = 16;

    int hexCursor = 0;
    int charCursor = 0;

    int hexCount = count;
    int charCount = count;

    int hexCurrentSpot = 0;
    int charCurrentSpot = 0;
    if(start >= 256)
    {
        hexCursor = start;
        charCursor = start;
        start = 0;
    }
    /* This is the header of the display table */
    cout << hex << "Offset: 0x" << offset << endl;
    cout << "   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f     0...4...8...c..." <<endl;
    cout << "  +------------------------------------------------+  +----------------+" <<endl;

    /* Loop iterates once for each row */
    while(height < 16) {

        /* Fills out the row label */
        cout << hex << setfill('0')<< setw(2) << height << "|";

        /* This handles the left side of the table with the hex numbers */
        while (hexWidth > 0) {
           /*cout << dec << "start: " << start << " <= " << offset << endl;
           cout << dec << "hexCount: " << hexCount << "> 0" << endl;
           cout << dec << hexCurrentSpot << " >= " << start << endl;*/
            /* if the offset is as least as great as the start and the offset is less than the start and count and count is greater than 0 */
            if (start <= offset && hexCount > 0 && hexCurrentSpot >= start) {
                printf("%02x", buf[hexCursor]);
                cout << " ";
                hexCursor++;
                hexCount--;
            } else {
                cout << "   ";
            }
            hexWidth--;
            hexCurrentSpot++;
        }

        /* Divisor between left/right hex/char sides of the table */
        cout<< hex << "|" << setfill('0') << setw(2) << height << "|";

        /* This handles the right side of the table with the characters */
        while(charWidth > 0) {
            //cout << dec << "start: " << start << " <= " << offset << endl;
           // cout << dec << "charCount: " << charCount << "> 0" << endl;
            //cout << dec << charCurrentSpot << " >= " << start << endl;

            if(isprint(buf[charCursor])){
                //cout << "Entered isPrint: " << buf[charCursor] << endl;
            }




            /* if the offset is as least as great as the start and the offset is less than the start and count and count is greater than 0 */
            if(start <= offset && charCount > 0 && charCurrentSpot >= start){

                /* If the character is printable */
                if(isprint(buf[charCursor])){
                    cout << static_cast<uint8_t>(buf[charCursor]);
                } else {
                    cout << " ";
                }

                charCursor++;
                charCount--;
            } else {
                cout << " ";
                //charCursor++;
            }
            charWidth--;
            charCurrentSpot++;
        }

        /* The right border of the table */
        cout << "|" << endl;

        /* Incrementing values for the next row */
        height++;
        hexWidth = 16;
        charWidth = 16;
    }

    /* The bottom of the table */
    cout << "  +------------------------------------------------+  +----------------+" <<endl;
}

/* Displays the buffer, outputing in 256 byte chunks through calling displayBufferPage */
void StepZDebug::displayBuffer(uint8_t *buf, uint32_t count, uint64_t offset) {
    int start = 0;

    while(count > 0) {
        if(count > 256){
            displayBufferPage(buf,256,start,offset);
            start+=256;
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
    cout << "Image Type: 0x" << hex << setw(8) <<  setfill('0') << headerInfo->imageType << endl;
    cout << "Flags: 0x" << hex << setw(8) <<  setfill('0') << headerInfo->fFlags << endl;
    cout << "Virtual CHS: " << headerInfo->legacyGeometry[0] << "-" << headerInfo->legacyGeometry[1] << "-" << headerInfo->legacyGeometry[2] << endl;
    cout << "Sector size: 0x" << hex << setw(8) <<  setfill('0') << headerInfo->legacyGeometry[3] << "   " << dec << headerInfo->legacyGeometry[3] << endl;
    cout << "Logical CHS: " << dec << headerInfo->LCHSGeometry[0] << "-" << headerInfo->LCHSGeometry[1] << "-" << headerInfo->LCHSGeometry[2] << endl;
    cout << "Sector size: 0x" << hex << setw(8) <<  setfill('0') << headerInfo->LCHSGeometry[3] << "   " << dec << headerInfo->LCHSGeometry[3] << endl;
    cout << "Map Offset (""Offblocks""): 0x" << hex << setw(8) << setfill('0') << headerInfo->offBlocks << "   " << dec << headerInfo->offBlocks << endl;
    cout << "Frame offset (""OffData""): 0x" << hex << setw(8) << setfill('0') << headerInfo->offData << "   " << dec << headerInfo->offData << endl;
    cout << "Frame size (""cbBlock""): 0x" << hex << setw(8) << setfill('0') << headerInfo->cbBlock << "   " << dec << headerInfo->cbBlock << endl;
    cout << "Extra Frame Size: 0x" << hex << setw(8) << setfill('0') << headerInfo->cbBlockExtra << "   " << dec << headerInfo->cbBlockExtra << endl;
    cout << "Total frames (""cBlocks""): 0x" << hex << setw(8) << setfill('0') << headerInfo->cBlocks << "   " << dec << headerInfo->cBlocks << endl;
    cout << "Frames allocated: 0x" << hex << setw(8) << setfill('0') << headerInfo->cBlocksAllocated << "   " << dec << headerInfo->cBlocksAllocated << endl;
    cout << "Disk size (""cbDisk""): 0x" << hex << setw(16) << setfill('0') << headerInfo->cbDisk << "   " << dec << headerInfo->cbDisk << endl;
    cout << "Image Comment: " << endl;
    displayBufferPage(reinterpret_cast<uint8_t *>(headerInfo->szComment), 256, 0, 84);
}
void StepZDebug::dumpPartitionTable(struct PartitionEntry *P){
    for(int PartitionNumber = 0; PartitionNumber < 4; PartitionNumber++){
        //p[array#,Byte#]
        cout << "Partition: " << PartitionNumber << endl;
        cout << "Status: ";
        if(P->partitionEntry[PartitionNumber] == 00)
            cout << "inactive" << endl;
        else if(P->partitionEntry[PartitionNumber] >= hex(01) && P->partitionEntry[PartitionNumber] <= hex(7f))
            cout << "invalid" << endl;
        else
            cout << P->partitionEntry[PartitionNumber] << endl;

        cout << dec << "First sector CHS: " << &P->partitionEntry[PartitionNumber] + 1 << "-" << P->partitionEntry[PartitionNumber] + 2 << "-" << P->partitionEntry[PartitionNumber] + 3 << endl;
        cout << dec << "Last sector CHS: " << P->partitionEntry[PartitionNumber] + 5 << "-" << P->partitionEntry[PartitionNumber] + 6 << "-" << P->partitionEntry[PartitionNumber] + 7 << endl;

        cout << "Partition Type: " << P->partitionEntry[PartitionNumber] + 4;
        if(P->partitionEntry[PartitionNumber] + 4 == hex(83)
        cout << " linux native" << endl;
        else if(P->partitionEntry[PartitionNumber] + 4 == 00)
        cout << " empty" << endl;
        else
            cout << "You done fucked up now ricky bobby   " << (P->partitionEntry[PartitionNumber] + 4 << endl;
        cout << "First LBA sector: " << dec << (P->partitionEntry[PartitionNumber] + 8)+(P->partitionEntry[PartitionNumber] + 9)+(P->partitionEntry[PartitionNumber] + 10)+(P->partitionEntry[PartitionNumber] + 11) << endl;
        cout << "LBA Sector Count: " << dec << (P->partitionEntry[PartitionNumber] + 12)+(P->partitionEntry[PartitionNumber] + 13)+(P->partitionEntry[PartitionNumber] + 14)+(P->partitionEntry[PartitionNumber] + 15) << endl;
        cout << endl;
    }
}


