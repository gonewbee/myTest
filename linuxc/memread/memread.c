#include<stdio.h>  
#include<unistd.h>  
#include<sys/mman.h>  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h> 

/**
 * 通过mmap映射/dev/mem
 */
void testmmap(int fd) {
    unsigned char *map_base;
    unsigned long addr;
    unsigned char content;
    int i = 0;
    map_base = mmap(NULL, 0xff, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x20000);
    if (MAP_FAILED==map_base) {
        perror("mmap");
        return;
    }
    for (i=0; i<0xff; i++) {
        addr = (unsigned long)(map_base+i);
        content = map_base[i];
        printf("address:0x%lx; content:0x%x\n", 
                addr, (unsigned int)content);
    }
    munmap(map_base, 0xff);
}

void testmmap4(int fd) {
    uint32_t *map_base;
    unsigned long addr;
    uint32_t content;
    int i = 0;
    int dataNum = 0x40;
    map_base = mmap(NULL, dataNum*sizeof(uint32_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x20000);
    if (MAP_FAILED==map_base) {
        perror("mmap");
        return;
    }
    for (i=0; i<dataNum; i++) {
        addr = (unsigned long)(map_base+i);
        content = map_base[i];
        printf("address:0x%lx; content:0x%x\n", 
                addr, content);
    }
    munmap(map_base, dataNum*sizeof(uint32_t));
}

void searchInt(int fd, uint32_t num) {
    uint32_t content = 0;
    int i = 0;
    printf("%s enter\n", __func__);
#if 1
#if 0
    /* 测试lseek */
    if (-1==lseek(fd, 0x20000, SEEK_SET))
        perror("lseek");
    else
        printf("lseek success\n");
#endif
    while(-1!=read(fd, &content, sizeof(uint32_t))) {
        if (num==content) {
            printf("index:%x;content:%x\n", i*4, content);
        }
        i++;
    }
    perror("read");
    printf("%s end i:%x\n", __func__, i);
#else
    for (i=0; i<0x10000; i++) {
        if (-1==read(fd, &content, sizeof(uint32_t))) {
            perror("read");
            return;
        }
        if (num==content) {
            printf("i:%x;content:%x\n", i, content);
        }
    }
#endif
}

int main(int argc, char **argv) {
    int fd;
    fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (-1==fd)
        return 1;
    if (argc==2) {
        uint32_t num = atoi(argv[1]);
        searchInt(fd, num);
    } else {
        //testmmap(fd);
        testmmap4(fd);
    }
    close(fd);
    return 0;
}

