//
// Created by csis on 4/13/20.
//

#ifndef INC_5806OSPROJECT_INODE_H
#define INC_5806OSPROJECT_INODE_H


#include <cstdint>
struct InodeStruct {
    uint16_t i_mode;
    uint16_t i_uid;

    uint32_t i_size;
    uint32_t i_atime;
    uint32_t i_ctime;
    uint32_t i_mtime;
    uint32_t i_dtime;

    uint16_t i_gid;
    uint16_t i_links_count;

    uint32_t i_blocks;
    uint32_t i_flags;
    uint32_t i_osd1;
    uint32_t i_block[15];
    uint32_t i_generation;
    uint32_t i_file_acl;
    uint32_t i_sizeHigh; // Appears to correlate to i_dir_acl, lunux sets to 0 if file is not a regulat file.
    uint32_t i_faddr;

    /* 96bit OS dependant i_osd2 structure (Linux) */
    uint16_t i_blocksHigh; // Combination of obsolete l_i_frag and l_i_fsize (Fragments are not supported in Linux, this is the new thing in Ext4
    uint16_t reserved16;
    uint16_t i_uidHigh;
    uint16_t i_gidHigh;

    uint32_t reserved32;
};


class Inode {
    public:
        int32_t fetchInode(struct Ext2File *f,uint32_t inum);
        int32_t writeInode(struct Ext2File *f, uint32_t inum);
        int32_t inodeInUse(struct Ext2File *f, uint32_t inum);
        uint32_t allocateInode(struct Ext2File *f, int32_t group);
        int32_t freeInode(struct Ext2File *f, uint32_t inum);
    private:
    struct InodeStruct iNodeStruct;
};


#endif //INC_5806OSPROJECT_INODE_H
