#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <linux/fs.h>
#include<ext2fs/ext2_fs.h>
#include<stdlib.h>

//#include <ext2fs/ext2_fs.h>

int main(int argc, char *argv[])
{ 
	int fd;
	int count;
	int ino , i;
	int bgno , iindex, inode_size, block_size;
	unsigned long inode_offset;
	struct ext2_super_block sb;
	struct ext2_inode inode;
	struct ext2_group_desc bgdesc;

	ino = atoi(argv[2]);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("readsuper:");
		exit(errno);
	}
	
	
	lseek64(fd, inode.i_block[0] * block_size, SEEK_SET);
        for(i = 0; i < 10; i++) 
        {
               read(fd, &ext2_dirent, sizeof(ext2_dirent));
               printf("%d %d %d %s\n", ext2_dirent.inode, ext2_dirent.rec_len, ext2_dirent.name_len, ext2_dirent.name);
               lseek64(fd, ext2_dirent.rec_len - sizeof(ext2_dirent), SEEK_CUR);
        }
        
        
	lseek64(fd, 1024, SEEK_CUR);
	printf("size of super block = %lu\n", sizeof(struct ext2_super_block));
	count = read(fd, &sb, sizeof(struct ext2_super_block));
	printf("Magic: %x\n" , sb.s_magic);
	printf("Inodes Count: %d\n", sb.s_inodes_count);
	printf("size of BG DESC = %lu\n", sizeof(struct ext2_group_desc));
	inode_size = sb.s_inode_size;
	block_size = 1024 << sb.s_log_block_size;
	
	bgno = (ino -1) / sb.s_inodes_per_group;
	iindex = (ino -1) % sb.s_inodes_per_group;
	lseek64(fd, 1024 + block_size + bgno * sizeof(bgdesc), SEEK_SET);
	count = read(fd, &bgdesc, sizeof(struct ext2_group_desc));
	printf("Inode Table: %d\n", bgdesc.bg_inode_table);
	
	inode_offset = bgdesc.bg_inode_table * block_size + iindex * inode_size;
	lseek64(fd, inode_offset, SEEK_SET);
	read(fd, &inode, sizeof(inode));
	printf("size of file %d \n", inode.i_size);
	printf("No of blocks %d , blocks: " , inode.i_blocks);
	for (i=0; i < inode.i_blocks; i++)
		printf("%d, ", inode.i_block[i]);
	close(fd);
}
