//
// Created by csis on 1/26/20.
//

#include <cstring>
#include "../include/StepZDebug.h"
#include "../include/Ext2File.h"

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
void StepZDebug::dumpPartitionTable(VDIFile *f, struct PartitionEntry *P){
    for(int PartitionNumber = 0; PartitionNumber < 4; PartitionNumber++) {
        //p[array#][Byte#]

        cout << "partition: " << PartitionNumber + 1 << endl;

        cout << "Status: ";
        if (P->partitionEntries[PartitionNumber][0] == 00)
            cout << "Inactive" << endl;
        else if (P->partitionEntries[PartitionNumber][0] >= 1 && P->partitionEntries[PartitionNumber][0] <= 127)
            cout << "Invalid" << endl;
        else
            cout << P->partitionEntries[PartitionNumber][0] << endl;

        cout << hex << "First sector CHS: ";
        printf("%u", P->partitionEntries[PartitionNumber][3]);
        cout << "-";
        printf("%u", P->partitionEntries[PartitionNumber][1]);
        cout << "-";
        printf("%u", P->partitionEntries[PartitionNumber][2]);
        cout << endl;

        cout << "Last Sector CHS: ";
        printf("%u", P->partitionEntries[PartitionNumber][7]);
        cout << "-";
        printf("%u", P->partitionEntries[PartitionNumber][5]);
        cout << "-";
        printf("%u", P->partitionEntries[PartitionNumber][6]);
        cout << endl;

        cout << "partition Type: ";
        printf("%x", P->partitionEntries[PartitionNumber][4]);
        cout << " ";
        if (P->partitionEntries[PartitionNumber][4] == 0x83)
            cout << " linux native" << endl;
        else if (P->partitionEntries[PartitionNumber][4] == 0x00)
            cout << " empty" << endl;
        else
            cout << "You done fucked up now ricky bobby   " << P->partitionEntries[PartitionNumber][4] << endl;

        cout << "First LBA sector: ";

        uint32_t temp = P->partitionEntries[PartitionNumber][8] | (P->partitionEntries[PartitionNumber][9] << 8) |
                        (P->partitionEntries[PartitionNumber][10] << 16) |
                        (P->partitionEntries[PartitionNumber][11] << 24);
        printf("%u", temp);
        cout << endl;

        cout << "LBA Sector Count: ";
        temp = P->partitionEntries[PartitionNumber][12] | P->partitionEntries[PartitionNumber][13] << 8 |
               P->partitionEntries[PartitionNumber][14] << 16 | P->partitionEntries[PartitionNumber][15] << 24;
        printf("%u", temp);
        cout << endl << endl;
    }
}
void StepZDebug::dumpSuperBlock(Ext2File *ext2 ){
    char mtime[32],wtime[32],chkTime[32];
    time_t tmp;

    cout << "Superblock contents:" << endl;
    cout << "Number of Inodes: ";
        printf("%u",ext2->superBlock.s_inodes_count);
        cout << endl;
    cout << "Number of Blocks: ";
        printf("%u",ext2->superBlock.s_blocks_count);
        cout << endl;
    cout << "Number of reserved blocks: ";
        printf("%u", ext2->superBlock.s_r_blocks_count);
        cout << endl;
    cout << "Number of free blocks: ";
        printf("%u",ext2->superBlock.s_free_blocks_count);
        cout << endl;
    cout << "Number of free inodes: ";
        printf("%u",ext2->superBlock.s_free_inodes_count);
        cout << endl;
    cout << "First data Block: ";
        printf("%u",ext2->superBlock.s_first_data_block);
        cout << endl;
    cout << "Log block size: ";
        printf("%u",ext2->superBlock.s_log_block_size);
        cout << endl;
    cout << "Log fragment size: ";
        printf("%u",ext2->superBlock.s_log_frag_size);
        cout << endl;
    cout << "Blocks per group: ";
        printf("%u",ext2->superBlock.s_blocks_per_group);
        cout << endl;
    cout << "Fragments per group: ";
        printf("%u",ext2->superBlock.s_frags_per_group);
        cout << endl;
    cout << "Inodes per group: ";
        printf("%u",ext2->superBlock.s_inodes_per_group);
        cout << endl;
    cout << "Last mount time: ";
        tmp = ext2->superBlock.s_mtime;
        strcpy(mtime,ctime(&tmp));
        printf("%s",mtime);
    cout << "Last write time: ";
        tmp = ext2->superBlock.s_wtime;
        strcpy(wtime,ctime(&tmp));
        printf("%s",wtime);
    cout << "Mount count: ";
        printf("%u",ext2->superBlock.s_mnt_count);
        cout << endl;
    cout << "Max mount count: ";
        printf("%u",ext2->superBlock.s_max_mnt_count);
        cout << endl;
    cout << "Magic Number: ";
        cout << "0x";
        printf("%x",ext2->superBlock.s_magic);
        cout << endl;
    cout << "State: ";
        printf("%u",ext2->superBlock.s_state);
        cout << endl;
    cout << "error processing: ";
        printf("%u",ext2->superBlock.s_errors);
        cout << endl;
    cout << "Revision level: ";
        printf("%2.1u",ext2->superBlock.s_rev_level);
        cout << endl;
    cout << "Last system check: ";
        tmp = ext2->superBlock.s_lastcheck;
        strcpy(chkTime,ctime(&tmp));
        printf("%s",chkTime);
    cout << "Check interval: ";
        printf("%u",ext2->superBlock.s_checkinterval);
        cout << endl;
    cout << "OS creator: ";
        printf("%u",ext2->superBlock.s_creator_os);
        cout << endl;
    cout << "Default reserve UID: ";
        printf("%u",ext2->superBlock.s_def_resuid);
        cout << endl;
    cout << "Default reserve GID: ";
        printf("%u",ext2->superBlock.s_def_resgid);
        cout << endl;
    cout << "First inode number: ";
        printf("%u",ext2->superBlock.s_first_ino);
        cout << endl;
    cout << "Inode size: ";
        printf("%u",ext2->superBlock.s_inode_size);
        cout << endl;
    cout << "Block group number: ";
        printf("%u",ext2->superBlock.s_block_group_nr);
        cout << endl;
    cout << "Feature compatibility bits: ";
        cout << "0x";
        printf("%08x",ext2->superBlock.s_feature_compat);
        cout << endl;
    cout << "Feature incompatibility bits: ";
    cout << "0x";
        printf("%08x",ext2->superBlock.s_feature_incompat);
        cout << endl;
    cout << "Feature read/only compatibility bits: ";
        cout << "0x";
        printf("%08x",ext2->superBlock.s_feature_ro_compat);
        cout << endl;
   // cout << "UUID: ";
    cout << "Volume name: ";
        printf("%s",ext2->superBlock.s_volume_name);
        cout << endl;
    cout << "Last mount point: ";
        printf("%s",ext2->superBlock.s_last_mounted);
        cout << endl;
    cout << "Alogrithm bitmap: ";
        printf("%08x",ext2->superBlock.s_algo_bitmap);
        cout << endl;
    cout << "Number of blocks to preallocate: ";
        printf("%u",ext2->superBlock.s_prealloc_blocks);
        cout << endl;
    cout << "Number of blocks to preallocate for directories: ";
        printf("%u",ext2->superBlock.s_prealloc_dir_blocks);
        cout << endl;
    //cout << "Journal UUID: ";
    cout << "Journal inode number: ";
        printf("%u",ext2->superBlock.s_journal_inum);
        cout << endl;
    cout << "Journal device number: ";
        printf("%u",ext2->superBlock.s_journal_dev);
        cout << endl;
    cout << "Journal last orphan inode number: ";
        printf("%u",ext2->superBlock.s_last_orphan);
        cout << endl;
    cout << "Default hash version: ";
        printf("%u",ext2->superBlock.s_def_hash_version);
        cout << endl;
    cout << "Default mount option bitmap: ";
        cout << "0x";
        printf("%08x",ext2->superBlock.s_default_mount_options);
        cout << endl;
    cout << "First meta block group: ";
        printf("%u",ext2->superBlock.s_first_meta_bg);
        cout << endl;
        cout << endl;
}
void StepZDebug::dumpBGDT(Ext2File *ext2){
    cout << "Block   Block   Inode   Inode  Free    Free    Used" << endl;
    cout << "Number  Bitmap  Bitmap  Table  Blocks  Inodes  Dirs" << endl;
    cout << "------  ------  ------  -----  ------  ------  ----" << endl;
    for(int bgNum = 0; bgNum < ext2->getNumBlockGroups(); bgNum++){
        printf("%-6u",bgNum);
        cout << "  ";
        printf("%-6u",ext2->BGDT[bgNum].bg_block_bitmap);
        cout << "  ";
        printf("%-6u",ext2->BGDT[bgNum].bg_inode_bitmap);
        cout << "  ";
        printf("%-5u",ext2->BGDT[bgNum].bg_inode_table);
        cout << "  ";
        printf("%-6u",ext2->BGDT[bgNum].bg_free_blocks_count);
        cout << "  ";
        printf("%-6u",ext2->BGDT[bgNum].bg_free_inodes_count);
        cout << "  ";
        printf("%-4u",ext2->BGDT[bgNum].bg_used_dirs_count);
        cout << endl;
    }
}
void StepZDebug::dumpInode(Ext2File *ext2, struct InodeStruct inodeStruct, uint32_t iNum) {
    char crTime[32],atime[32],mtime[32], dtime[32];
    time_t tmp;


    cout << "Inode " << dec << iNum << ":" << endl;
 displayBuffer(reinterpret_cast<uint8_t*>(&inodeStruct), ext2->superBlock.s_inode_size,0);
 cout << "Mode: ";
    printf("%o \n",inodeStruct.i_mode);
 cout << "Size: ";
    printf("%u \n",inodeStruct.i_size);
 cout << "Blocks: ";
    printf("%u \n",inodeStruct.i_blocks);
 cout << "UID / GID:";
    printf("%u", inodeStruct.i_uid);
    cout << " / ";
    printf("%u \n",inodeStruct.i_gid);
 cout << "Links: ";
    printf("%u \n",inodeStruct.i_links_count);
 cout << "Created: ";
    tmp = inodeStruct.i_ctime;
    strcpy(crTime,ctime(&tmp));
    printf("%s",crTime);
 cout << "Last Access: ";
    tmp = inodeStruct.i_atime;
    strcpy(atime,ctime(&tmp));
    printf("%s",atime);
 cout << "Last modification: ";
    tmp = inodeStruct.i_mtime;
    strcpy(mtime,ctime(&tmp));
    printf("%s",mtime);
 cout << "Deleted: ";
    tmp = inodeStruct.i_dtime;
    strcpy(dtime,ctime(&tmp));
    printf("%s",dtime);
 cout << "Flags: ";
    printf("%u \n",inodeStruct.i_flags);
 cout << "File Version: ";
    printf("%u \n",inodeStruct.i_generation);
 cout << "ACL block: ";
    printf("%u \n",inodeStruct.i_file_acl);
 cout << "Direct Blocks: " << endl;
 cout << "0-3: ";
    printf("%5u ",inodeStruct.i_block[0]);
    printf("%5u ",inodeStruct.i_block[1]);
    printf("%5u ",inodeStruct.i_block[2]);
    printf("%5u \n", inodeStruct.i_block[3]);
 cout << "4-7: ";
    printf("%5u ",inodeStruct.i_block[4]);
    printf("%5u ",inodeStruct.i_block[5]);
    printf("%5u ",inodeStruct.i_block[6]);
    printf("%5u \n",inodeStruct.i_block[7]);
 cout << "8-11: ";
    printf("%5u ",inodeStruct.i_block[8]);
    printf("%5u ",inodeStruct.i_block[9]);
    printf("%5u ",inodeStruct.i_block[10]);
    printf("%5u \n", inodeStruct.i_block[11]);
 cout << "Single indirect block: ";
    printf("%5u \n",inodeStruct.i_block[12]);
 cout << "Double indirect block: ";
    printf("%5u \n", inodeStruct.i_block[13]);
 cout << "Triple indirect block: ";
    printf("%5u \n", inodeStruct.i_block[14]);
}
