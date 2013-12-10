#include <stdio.h>
#include <string.h>

int setServo(int servo, int position) {
    FILE* fp = fopen("/dev/servoblaster", "w");
    if(!fp)
        return 0;

    char buffer[20];
    snprintf(buffer, 19, "%d=%d\n", servo, position);

    fputs(buffer, fp);

    fclose(fp);

    return 1;
}


int getServo(int servo) {
    FILE* fp = fopen("/dev/servoblaster", "r");
    if(!fp)
        return -1;

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
