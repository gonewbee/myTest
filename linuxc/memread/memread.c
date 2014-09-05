/**
 * 通过mmap映射/dev/mem
 */
#include<stdio.h>  
#include<unistd.h>  
#include<sys/mman.h>  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h> 

int main(int argc, char **argv) {
    unsigned char *map_base;
    int n, fd;
    unsigned long addr;
    unsigned char content;
    int i = 0;
    fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (-1==fd)
        return 1;
    map_base = mmap(NULL, 0xff, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x20000);
    if (MAP_FAILED==map_base) {
        perror("mmap");
        return 1;
    }
    for (i=0; i<0xff; i++) {
        addr = (unsigned long)(map_base+i);
        content = map_base[i];
        printf("address:0x%lx; content:0x%x\n", 
                addr, (unsigned int)content);
    }
    close(fd);
    munmap(map_base, 0xff);
    return 0;
}

