#include <stdio.h>
#include <string.h>

int setServoPosition(int servo, int position) {
    FILE* fp = fopen("/dev/servoblaster", "w");
    if(!fp) {
        fprintf(stderr, "[-] Can't open servoblaster device to set servo %d (pos %d)\n", servo, position);
        return 0;
    }

    char buffer[20];
    snprintf(buffer, 19, "%d=%d\n", servo, position);
    fprintf(stderr, "%s", buffer);

    fputs(buffer, fp);

    fclose(fp);

    return 1;
}


int getServoPosition(int servo) {
    FILE* fp = fopen("/dev/servoblaster", "r");
    if(!fp) {
        fprintf(stderr, "[-] Can't open servoblaster device to read servo %d\n", servo);
        return -1;
    }

    int position = -1;
    char buffer[20];
    while(!feof(fp)) {
        fgets(buffer, 19, fp);
        int s,p;
        sscanf(buffer, "%d=%d", &s, &p);

        if(s == servo) {
            position = p;
            break;
        }
    }

    fclose(fp);

    return position;
}
