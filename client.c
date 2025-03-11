#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#include "utils.h"

void test_ioctl(int fd, int cmd){
    printf("cmd: %d\n", cmd);

    char buf[10];

    int ret = ioctl(fd, cmd, buf);
}


unsigned char *mem;

#define CACHE_MISS (250)

void test_flush_reload(void){
    char buf[10];
    int results[256] = {0};
    for(int try=0; try <1000; ++try){
        int ret = ioctl(3, 1, buf);

        if(ret > 0){
            for (size_t i = 0; i < 256; i++) {
                size_t mix_i = ((i * 167) + 13) & 255;
                uint64_t t1 = mrdtsc();
                maccess(&mem[mix_i * 4096]);
                uint64_t t2 = mrdtsc() - t1;
                if (t2 < CACHE_MISS && mix_i != 0) {
                    results[mix_i]++;
                }
            }
        }
    }

    int j = -1,  k = -1;
    for(int i = 0; i < 256; ++i){
        if(j < 0 || results[i] >= results[j]){
            k = j;
            j = i;
        }
        else if (k < 0 || results[i] >= results[k]){
            k = i;
        }
    }
    printf("idx: %c score: %d, second_idx:%2x, second score: %d\n", j, results[j], k, results[k]);
    
}


int main(int argc, char **argv){
    int fd = open("/dev/mioctl", O_RDWR);

    if (fd < 0){
        perror("device open failed!\n");
        exit(-1);
    }

    int cmd = 1;
    if(argc > 1)
        sscanf(argv[1], "%d", &cmd);

    // test_ioctl(fd, cmd);


    mem = mmap(NULL, 4096*256, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (mem == MAP_FAILED){
        perror("mmap failed!\n");
        exit(-1);
    }

    test_flush_reload();

    return 0;

}