/*
 * Copyright (c) 2014, Timo Schmid, Kevin Schaller
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the university nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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
