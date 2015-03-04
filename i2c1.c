#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

main() {
    FILE *fd;
    int ret=-1;

    char obuffer[4];
    char ibuffer[4];
    
    obuffer[0] = 0x5a;
    obuffer[1] = 0x01;
    obuffer[2] = 0x00;
    obuffer[3] = 0x00;

    fd = fopen("/dev/ttyUSB0","r+");
    if(fd < 0) {
        printf("Failed to open port\n");
        exit(-1);
    }

    ret=fwrite(obuffer,1,4,fd);
    fflush(fd);

    if(ret < 0) {
        perror("ERROR:");
        exit(-2);
    }
    fprintf(stderr,"Written %d\n",ret);

    ret=fread(ibuffer,1,1,fd);
    if(ret < 0) {
        perror("ERROR:");
        exit(-2);
    }
    fprintf(stderr,"read %d\n",ret);
    fprintf(stderr,"- ibuffer[0] %02x\n",ibuffer[0]);

    close(fd);



}

