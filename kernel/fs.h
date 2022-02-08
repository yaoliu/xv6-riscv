// On-disk file system format.
// Both the kernel and user programs use this header file.


#define ROOTINO  1   // root i-number
#define BSIZE 1024  // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {  // 超级块
  uint magic;        // Must be FSMAGIC
  uint size;         // Size of file system image (blocks) 
  uint nblocks;      // Number of data blocks // 数据块 数量 默认 954
  uint ninodes;      // Number of inodes. // inode 块 数量 默认 10
  uint nlog;         // Number of log blocks // log 块 数量 默认 13
  uint logstart;     // Block number of first log block 日志块开始位置
  uint inodestart;   // Block number of first inode block 索引节点块开始位置
  uint bmapstart;    // Block number of first free map block // bitmap块开始位置
};

#define FSMAGIC 0x10203040

#define NDIRECT 11
#define NINDIRECT (BSIZE / sizeof(uint))
// 11 + 256 + 256 * 256
#define MAXFILE (NDIRECT + NINDIRECT + NINDIRECT * NINDIRECT)

// On-disk inode structure // 存储在磁盘中的索引节点的数据结构
struct dinode {
  short type;           // File type // 索引节点类型 该索引节点是文件还是目录
  short major;          // Major device number (T_DEVICE only)
  short minor;          // Minor device number (T_DEVICE only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT+1];   // Data block addresses
};

// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB           (BSIZE*8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) ((b)/BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};

