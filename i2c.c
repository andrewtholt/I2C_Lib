#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT "/dev/ttys003"

main() {
    int fd=-1;
    int ret=-1;

    char obuffer[4];
    char ibuffer[4];
    
    obuffer[0] = 0x5a;
    obuffer[1] = 0x01;
    obuffer[2] = 0x00;
    obuffer[3] = 0x00;

    fd = open(PORT,O_RDWR);
    if(fd < 0) {
        printf("Failed to open port\n");
        exit(-1);
    }

    ret=write(fd,obuffer,4);
    if(ret < 0) {
        perror("ERROR:");
        exit(-2);
    }
    fprintf(stderr,"Written %d\n",ret);

    ret=read(fd,ibuffer,1);
    if(ret < 0) {
        perror("ERROR:");
        exit(-2);
    }
    fprintf(stderr,"read %d\n",ret);
    fprintf(stderr,"- ibuffer[0] %02x\n",ibuffer[0]);

    close(fd);



}

