#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>



int main(int argc, char **argv){
    int fd = open("/dev/mioctl", O_RDWR);

    if (fd < 0){
        perror("device open failed!\n");
        exit(-1);
    }

    int cmd = 1;
    if(argc > 1)
        sscanf(argv[1], "%d", &cmd);

    printf("cmd: %d\n", cmd);

    char buf[10];

    int ret = ioctl(fd, cmd, buf);

    return 0;

}