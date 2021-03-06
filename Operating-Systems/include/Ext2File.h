//
// Created by csis on 3/23/20.
//

#ifndef INC_5806OSPROJECT_EXT2FILE_H
#define INC_5806OSPROJECT_EXT2FILE_H

#include <cstdint>
#include <time.h>
#include "VDIFile.h"
#include "Partition.h"

struct SuperBlock{
    uint32_t s_inodes_count; // Total number of inodes
    uint32_t s_blocks_count; // Number of blocks in the system (including used, free and reserved)
    uint32_t s_r_blocks_count; // Number of blocks reserved for usage of the super user
    uint32_t  s_free_blocks_count; // Number of free blocks, including s_r_blocks_count. Sum of all free blocks of all the block groups.
    uint32_t s_free_inodes_count; // Sum of all free inodes of all block groups.
    uint32_t s_first_data_block; // first data block, in other words, the id of the block containing the superblock structure. ( always 0 for file systems with block size > 1KB, and always 1 for block size = 1KB. Superblock always starts at the 1024th byte of the disk)
    uint32_t s_log_block_size; // block size == 1024*pow(2, s_log_block_size)
    uint32_t s_log_frag_size;
    uint32_t s_blocks_per_group;
    uint32_t s_frags_per_group;
    uint32_t s_inodes_per_group;
    uint32_t s_mtime;
    uint32_t s_wtime;
    uint16_t s_mnt_count;
    uint16_t s_max_mnt_count;
    uint16_t s_magic;
    uint16_t s_state;
    uint16_t s_errors;
    uint16_t s_minor_rev_level;
    uint32_t s_lastcheck;
    uint32_t s_checkinterval;
    uint32_t s_creator_os;
    uint32_t s_rev_level;
    uint16_t s_def_resuid;
    uint16_t s_def_resgid;
    uint32_t s_first_ino;
    uint16_t s_inode_size;
    uint16_t s_block_group_nr;
    uint32_t s_feature_compat;
    uint32_t s_feature_incompat;
    uint32_t s_feature_ro_compat;
    char s_uuid[16];
    uint8_t s_volume_name[16];
    uint8_t s_last_mounted[64];
    uint32_t s_algo_bitmap;
    uint8_t s_prealloc_blocks;
    uint8_t s_prealloc_dir_blocks;
    char s_journal_uuid[16];
    uint32_t s_journal_inum;
    uint32_t s_journal_dev;
    uint32_t s_last_orphan;
    uint32_t s_hash_seed[4];
    uint8_t s_def_hash_version;
    uint32_t s_default_mount_options;
    uint32_t s_first_meta_bg;
    uint8_t padding[760];

};
struct BlockGroupDescriptor{
    uint32_t bg_block_bitmap;
    uint32_t bg_inode_bitmap;
    uint32_t bg_inode_table;
    uint16_t bg_free_blocks_count;
    uint16_t bg_free_inodes_count;
    uint16_t bg_used_dirs_count;
    uint16_t bg_pad;
    uint8_t bg_reserved[12];
};

class Ext2File {
public:
    Ext2File();
    bool ext2Open(char *fn);
    void ext2Close();
    int32_t fetchBlock(uint32_t blockNum, void *buf);
    int32_t writeBlock(uint32_t blockNum, void *buf);

    int32_t fetchSuperBlock(uint32_t blockNum, struct SuperBlock *sb);
    int32_t writeAllSuperBlocks(SuperBlock *superBlock);

    int32_t fetchBGDT(uint32_t blockNum, BlockGroupDescriptor *bgdt);
    int32_t writeAllBGDT(struct BlockGroupDescriptor *bgdt);

    struct SuperBlock superBlock;
    struct BlockGroupDescriptor *BGDT;

    /* Getter function for numBlockGroups */
    uint32_t getNumBlockGroups() {return numBlockGroups;}
    uint32_t getBlockSize() {return blockSize;}

    uint32_t allocateBlock(int32_t blockGroup = -1);

private:
    VDIFile *vdiFile;
    Partition *partition;
    uint32_t blockSize;
    uint32_t numBlockGroups;

    int32_t writeSuperBlock(uint32_t blockNum, struct SuperBlock *sb);
    int32_t writeBGDT(uint32_t blockNum, BlockGroupDescriptor *bgdt);

    bool containsCopyOfSuperBlockOrBGDT(uint32_t blockGroupNumber);


};


#endif //INC_5806OSPROJECT_EXT2FILE_H
