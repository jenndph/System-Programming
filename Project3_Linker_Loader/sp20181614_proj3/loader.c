#include "20181614.h"

int objcnt;         // number of object files
char* str;          // temp string for each lines of object file
char*** ESARR;      // array for reference number of each object file
FILE** p;           // file pointer for each object files
ESTAB* ePoint;      // external hash table pointer

void esnodeConnect(ESTAB* head, ESTAB* node);   // function for making ESTAB
int StrToHex(char c);                           // function for changing char to hex
int findMod(int i, int mod);                    // funcion for finding modification addr

void pFunc(char* c){    // Set PROGADDR(Program Load Address)
    int addr = strtol(c, NULL, 16);
    prog_addr = addr;
}

int lpass1(char* prog1, char* prog2, char* prog3){  // PASS 1 for loading & linking process
    objcnt = (prog3!=NULL)? 3 : (prog2!=NULL)? 2: (prog1!=NULL)? 1 : -1;    // count number of object file
    if(objcnt==-1) return -1;

    p = (FILE**)malloc(objcnt*sizeof(FILE*));
    p[0] = fopen(prog1, "r");                       // open each object file
    if(!p[0]){                                      // and return -1 if error exists in file name
        printf("Error. No such file\n");
        return -1;
    }
    if(objcnt>=2){
        p[1] = fopen(prog2, "r");
        if(!p[1]){
            printf("Error. No such file\n");
            return -1;
        }
    }
    if(objcnt==3){
        p[2] = fopen(prog3, "r");
        if(!p[2]){
            printf("Error. No such file\n");
            return -1;
        }
    }

    cs_addr = exec_addr = prog_addr;                // set CSADDR and EXECADDR as PROGADDR  
    PC = prog_addr;                                 // set PC as PROGADDR
    A = X = B = S = T = F = SW = 0;                 // initialize registers

    int i, j, k, addr, digit, tmp, cnt;
    eHead = (ESTAB**)malloc(objcnt*sizeof(ESTAB*)); // initialize ESTAB for each object file
    for(i=0; i<objcnt; i++) {
        eHead[i] = (ESTAB*)malloc(sizeof(ESTAB));
        eHead[i]->link = NULL;
    }
    ESARR = (char***)malloc(objcnt*sizeof(char**)); // initialize reference array for each object file
    for(i=0; i<objcnt; i++)
        ESARR[i] = (char**)malloc(10*sizeof(char**));
    for(i=0; i<objcnt; i++)
        for(j=0; j<10; j++)
            ESARR[i][j] = (char*)malloc(7*sizeof(char));


    printf("control\tsymbol\taddress\tlength\n");
    printf("section\tname\n");
    printf("-------------------------------------\n");

    for(i=0; i<objcnt; i++){                            // do loading & linking for each object file  
        while(1){
            str = (char*)malloc(100*sizeof(char));
            fgets(str, 100, p[i]);                      // store each line of object file in str
            if(str[0] == 'E')                           // END Record -> end pass 1
                break;
            str[strlen(str)-1] = '\0';                  // set '\0' at the end of the string
            if(str[0] == 'H'){                          // HEADER Record
                ePoint = (ESTAB*)malloc(sizeof(ESTAB)); // for save program name with addr
                for(j=1; j<7 && str[j]!=' '; j++)       // at the ESTAB
                    ePoint->symbol[j-1] = str[j];       // (Program name section)
                ePoint->symbol[j] = '\0';
                
                digit = 1; addr = 0;
                for(j=12; j>=7; j--){                   // (Starting address of object program section)
                    addr += StrToHex(str[j]) * digit;
                    digit *= 16;
                }
                ePoint->addr = cs_addr;
                ePoint->link = NULL;
                esnodeConnect(eHead[i], ePoint);        // connect new node to ESTAB

                digit = 1; addr = 0;
                for(j=18; j>=13; j--){                  // (Length of object program section)
                    addr += StrToHex(str[j]) * digit;
                    digit *= 16;
                }
                cs_addr += addr;                        // add length of object program to CSADDR
                printf("%s\t\t%04X\t%04X\n", ePoint->symbol, ePoint->addr, addr);
                continue;
            }
            if(str[0] == 'D'){                          // DEFINE Record
                for(j=1; str[j]!='\0'; ){
                    ePoint = (ESTAB*)malloc(sizeof(ESTAB));
                    for(k=0; k<6; k++)                  // (Name of defined external symbol section)
                        ePoint->symbol[k] = str[j++];
                    if(k==7)                            
                        ePoint->symbol[j] = '\0';
                    digit = 16*16*16*16*16; addr = 0;        
                    for(k=j, tmp=j; k<tmp+6; k++){      // (Relative address of symbol section)
                        addr += StrToHex(str[j++]) * digit;
                        digit /= 16;
                    }
                    ePoint->addr = addr + exec_addr;    // get real adress of external symbol
                    ePoint->link = NULL;
                    esnodeConnect(eHead[i], ePoint);    // connect new node to ESTAB
                }
                continue;
            }   
            if(str[0] == 'R'){                          // REFER Record
                strcpy(ESARR[i][0], eHead[i]->link->symbol);    // refernce number 01 is program name
                for(j=1; str[j]!='\0'; ){
                    cnt = 0; digit = 10;
                    for(k=0; k<2; k++){                 // (Reference number of external symbol section)
                        cnt += StrToHex(str[j++]) * digit;
                        digit /= 10;
                    }
                    for(k=j, tmp=j; k<tmp+6; k++){      // (Name of referred external symbol section)
                        ESARR[i][cnt-1][k-tmp] = str[k];
                        j++;
                    }
                }
                continue;
            }
            else                                        // Pass TEXT Record and annotaion section(.)
                continue;
        }

        exec_addr = cs_addr;                            // set EXECADDR as CSADDR
        ePoint = eHead[i]->link;
        for(ePoint = ePoint->link; ePoint!=NULL; ePoint=ePoint->link){  // print external symbol table
            printf("\t%s\t%04X\n", ePoint->symbol, ePoint->addr);
        }
        free(str);
    }
    printf("-------------------------------------\n");
    printf("           total length %04X\n", cs_addr - prog_addr);
    L = end = cs_addr - prog_addr;                      // L saves the total length of the object program

    for(i=0; i<objcnt; i++) fclose(p[i]);               // close each object file and

    p[0] = fopen(prog1, "r");                           // open those file again for pass 2
    if(objcnt>=2) p[1] = fopen(prog2, "r");
    if(objcnt==3) p[2] = fopen(prog3, "r");
    lpass2();                                          // PASS 2
    end_flag = 0;

    return 0;
}

int lpass2(){                                           // PASS 2 for loading & linking process
    int i, j, digit, addr, len, row, col, tmp;
    cs_addr = exec_addr = prog_addr;                    // set CSADDR and EXECADDR as PROGADDR

    int modNum;                                         // modNum will store the modification addr
    for(i=0; i<objcnt; i++){
        cs_addr = eHead[i]->link->addr;                 // set CSADDR as start address of each .obj file
        while(1){
            str = (char*)malloc(100*sizeof(char));      // store each line of object file in str 
            fgets(str, 100, p[i]);
            str[strlen(str)-1] = '\0';                  // set '\0' at the end of the line
            if(str[0]=='E' || strlen(str)==0){          // END Record -> end Pass 2
                if(strlen(str)==1)                      // without executable address
                    break;
                else{                               
                    digit = 1; addr = 0;                // with executable address
                    for(j=6; j>=1; j--){
                        addr += StrToHex(str[j]) * digit;
                        digit *= 16;
                    }
                    exec_addr = cs_addr + addr;         // set EXECADDR as CSADDR + addr in E Record
                    PC = exec_addr;                     // set PC as EXECADDR
                    break;                              
                }
            }
            if(str[0]=='T'){                            // TEXT Record
                digit = 1; addr = 0; len = 0;
                for(j=6; j>=1; j--){                    // (Starting address of object program section)
                    addr += StrToHex(str[j]) * digit;
                    digit *= 16;
                }
                exec_addr = cs_addr + addr;             // set EXECADDR as real address

                row = exec_addr/16; col = exec_addr%16; // get virtual memory address of EXECADDR
                len = StrToHex(str[7])*16 + StrToHex(str[8]); // (Length of this object code line section)
                for(j=9; j<9+len*2; j+=2){              // save object code in virtual memory
                    dumpMem[row][col++] = StrToHex(str[j])*16 + StrToHex(str[j+1]);
                    if(col>=16){ row += 1; col = 0; }
                }    
                continue;
            }
            if(str[0]=='M'){                            // MODIFICATION Record
                digit = 1; addr = 0; tmp = 0;
                for(j=6; j>=1; j--){                    // (Starting location section)
                    addr += StrToHex(str[j]) * digit;
                    digit *= 16;
                }
                addr += cs_addr;                        // get real address
                row = addr/16; col = addr%16;           // get address of virtual memory
                len = StrToHex(str[7])*16 + StrToHex(str[8]);       // 05 or 06

                modNum = StrToHex(str[10])*16 + StrToHex(str[11]);  // get reference number
                modNum = findMod(i, modNum);
                if(len==5){                             // pass half byte and relocate 5 half byte
                    tmp += (dumpMem[row][col]%16)*256*256 + dumpMem[row][col+1]*256 + dumpMem[row][col+2];
                    if(dumpMem[row][col]%16 == 'F'){    // if address is negative value
                        tmp = 0xFFFFF - tmp + 1;
                        tmp *= -1;
                    }
                    tmp = (str[9]=='+')? tmp+modNum : (tmp>=modNum)? tmp-modNum : (modNum-tmp)*(-1);
                    if(tmp<0) tmp = 0xFFFFF + tmp + 1;
                    dumpMem[row][col+2] = tmp%256; tmp/=256;    // save relocation result
                    dumpMem[row][col+1] = tmp%256; tmp/=256;
                    dumpMem[row][col] += tmp;
                }
                if(len==6){                             // relocation 6 half byte
                    tmp += dumpMem[row][col]*256*256 + dumpMem[row][col+1]*256 + dumpMem[row][col+2];
                    if(dumpMem[row][col]/16 == 'F'){    // if address is negative value
                        tmp = 0xFFFFFF - tmp + 1;
                        tmp *= -1;
                    }
                    tmp = (str[9]=='+')? tmp+modNum : (tmp>=modNum)? tmp-modNum : (modNum-tmp)*(-1);
                    if(tmp<0) tmp = 0xFFFFFF + tmp + 1;
                    dumpMem[row][col+2] = tmp%256; tmp/=256;    // save relocation result
                    dumpMem[row][col+1] = tmp%256; tmp/=256;
                    dumpMem[row][col] = tmp;
                }
            }
            else continue;
        }
    }

    return 0;
}

void esnodeConnect(ESTAB* head, ESTAB* node){   // function making ESTAB
    ESTAB* point;
    if(head->link==NULL){
        head->link = node;
    }
    else{
        for(point=head->link; point->link!=NULL; point=point->link);
        point->link = node;
    }
}

int StrToHex(char c){   // function for changing char to hex
    if(c >= '0' && c <= '9')
        return c - '0';
    if(c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else return 0;
}

int findMod(int i, int mod){    // function for finding modification addr
    int modNum=0;
    char modChar[7];
    strcpy(modChar, ESARR[i][mod-1]);   // get external symbol with reference number

    for(i=0; i<objcnt; i++){            // find external symbol's address with ESTAB
        for(ePoint=eHead[i]->link; ePoint!=NULL; ePoint=ePoint->link){
            if(!strncmp(ePoint->symbol, modChar, strlen(modChar)))
                return ePoint->addr;
        }
    }
    return modNum;
}
