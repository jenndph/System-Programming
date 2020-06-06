#include "20181614.h"

int duFunc(char* s, char* e){                   // Print the contents of the memory of the range
    int start, end;
    if(s==NULL && e==NULL){                     // dump
        if(addrSave!=0) addrSave += 0x01;       // start address is (last printed address + 1)
        start = addrSave;
        if(start>0xFFFFF){                      // when start address is bigger than 0xFFFFF
            printf("ERROR. Start should NOT be bigger than 0xFFFFF.\n");
            addrSave = 0x00;
            return 0;
        }
        end = (start + 0x9F)>0xFFFFF? 0xFFFFF : start + 0x9F;   // if 160 addr from start is bigger than 0xFFFFF,
                                                                // set end as 0xFFFFF
        addrSave = (end==0xFFFFF)? 0x00 : end;  // save the last printed address
    }
    else if(s!=NULL && e==NULL){                // dump start
        start = strtol(s, NULL, 16);            // convert start address to hexadecimal         
        if(start>0xFFFFF){                      // when start address is bigger than 0xFFFFF
            printf("ERROR. Start should NOT be bigger than 0xFFFFF.\n");
            return 0;
        }
        if(!strncmp(s, "-", 1)){                         // when start address is smaller than 0x00000
            printf("ERROR. Start should NOT be smaller than 0x00000.\n");
            return 0;
        }
        end = (start + 0x9F)>0xFFFFF? 0xFFFFF : start + 0x9F;   // if 160 addr from start is bigger than 0xFFFFF,
        // set end as 0xFFFFF
    }
    else{                                       // dump start, end
        start = strtol(s, NULL, 16);            // convert start and end address to hexadecimal
        end = strtol(e, NULL, 16);
        if(start>end){                          // when start address is bigger than end address
            printf("ERROR. Start must be smaller than end.\n");
            return 0;
        }
        if(!strncmp(s, "-", 1)){                         // when start address is smaller than 0x00000
            printf("ERROR. Start should NOT be smaller than 0x00000.\n");
            return 0;
        }
        if(start>0xFFFFF){                      // when start address is bigger than 0xFFFFF
            printf("ERROR. Start should NOT be bigger than 0xFFFFF.\n");
            return 0;
        }
        if(end>0xFFFFF){                        // when end address is bigger than 0xFFFFF
            printf("ERROR. End should NOT be bigger than 0xFFFFF.\n");
            return 0;
        }
    }

    int row, tmp;
    int i, j;
    int cArr[16];       // integer array storing ASCII codes of memory in current address line

    row = start/16;     // start line address
    for(tmp=start; tmp<=end; ){
        for(i=0; i<16; i++){                            // i means the column of dumpMem[][]
            if(i==0)                                    // start of the line
                printf("%05X ", row*16);
            if(row*16+i>=start && row*16+i<=end){       // address is in the range
                printf("%02X ", dumpMem[row][i]);
                cArr[i] = dumpMem[row][i];              // save the ASCII code of current address
                tmp += 0x01;
            }
            else{                                       // address is out of the range
                printf("   ");
                cArr[i] = 0x00;                         // set default value as 0x00
            }

            if(i==15){                                  // end of the line
                printf("; ");
                for(j=0; j<16; j++){                    // print the saved ASCII codes of current line
                    if(cArr[j]>=0x20 && cArr[j]<=0x7E)  // if the ASCII code is in 20 ~ 7E range
                        printf("%c", cArr[j]);
                    else                                // if the ASCII code is out of range,
                        printf(".");                    // or that address is out of range
                }
                printf("\n");
            }
        }
        row += 1;
    }
    return 1;
}

int eFunc(char* a, char* v){                            // Change the value of the address to new one
    int addr, value, row, col;
    addr = strtol(a, NULL, 16);                         // convert address and value to hexadecimal
    value = strtol(v, NULL, 16);
    if(addr>0xFFFFF){                                   // when start address is bigger than 0xFFFFF
        printf("ERROR. Address should NOT be bigger than 0xFFFFF.\n");
        return 0;
    }
    if(!strncmp(a, "-", 1)){                            // when start address is smaller than 0x00000
        printf("ERROR. Address should NOT be smaller than 0x00000.\n");
        return 0;
    }
    if(value>0xFF){                                     // when value is bigger than 0xFF
        printf("ERROR. Value should NOT be bigger than 0xFF.\n");
        return 0;
    }
    if(!strncmp(v, "-", 1)){                            // when value is negative
        printf("ERROR. Value should NOT be smaller than 0x00.\n");
        return 0;
    }
    row = addr/16; col = addr%16;
    dumpMem[row][col] = value;

    return 1;
}

int fFunc(char* s, char* e, char* v){                   // Change the value from start to end to new one
    int start, end, value, row;
    start = strtol(s, NULL, 16); end = strtol(e, NULL, 16);
    value = strtol(v, NULL, 16);                        // convert start, end and value to hexadecimal
    if(start>end){                                      // when start address is bigger than end address
        printf("ERROR. Start must be smaller than end.\n");
        return 0;
    }
    if(!strncmp(s, "-", 1)){                            // when start address is smaller than 0x00000
        printf("ERROR. Start should NOT be smaller than 0x00000.\n");
        return 0;
    }
    if(start>0xFFFFF){                                  // when start address is bigger than 0xFFFFF
        printf("ERROR. Start should NOT be bigger than 0xFFFFF.\n");
        return 0;
    }
    if(end>0xFFFFF){                                    // when end address is bigger than 0xFFFFF
        printf("ERROR. End should NOT be bigger than 0xFFFFF.\n");
        return 0;
    }
    if(value>0xFF){                                     // when value is bigger than 0xFF
        printf("ERROR. Value should NOT be bigger than 0xFF.\n");
        return 0;
    }
    if(!strncmp(v, "-", 1)){                            // when value is negative
        printf("ERROR. Value should NOT be smaller than 0x00.\n");
        return 0;
    }

    row = start/16;
    for(int tmp=start; tmp<=end; ){
        for(int i=0; i<16; i++){
            if(row*16+i>=start && row*16+i<=end){
                dumpMem[row][i] = value;
                tmp += 0x01;
            }
        }
        row += 1;
    }
    return 1;
}

void resetFunc(){       // Change all value in the memory to 0x00
    int i, j;
    for(i=0; i<65536; i++)
        for(j=0; j<16; j++)
            dumpMem[i][j] = 0x00;
}
