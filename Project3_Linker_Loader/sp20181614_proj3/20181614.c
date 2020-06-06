#include "20181614.h"

char* regular_form(char* str);
int commandCase(char* command);

int main(){
    if(makeHash()==-1) return 0;    // make Hash Table
    
    char strTmp[50], strSave[50];
    char* command;
    char *start, *end, *addr, *val, *mnem, *fname;
    char *prog1, *prog2, *prog3;
    int cCase;

    hisCnt = 1;                     // initialize the number of saved history list
    hisHead = NULL;                 // initialize the start of the history list

    st = (STABLE*)malloc(sizeof(STABLE));   // initialize linked list st
    st->link = NULL;                        // st will save the symbol used in *.asm file

    prog_addr = 0x00;                       // initialize PROGADDR
    bHead = (BLIST*)malloc(sizeof(BLIST));  // initialize break point linked list
    bHead->link = NULL;

    addrSave = 0;                   // initialize the variable storing last printed address
    for(int i=0; i<65536; i++)      // initialize the memory we use with 0x00
        for(int j=0; j<16; j++)
            dumpMem[i][j] = 0x00;
    
    while(1){
        printf("sicsim> ");
        fgets(strTmp, sizeof(strTmp), stdin);   // get command line
        
        strcpy(strSave, strTmp);                // create copy of command line recorded in history list
        strcpy(strTmp, regular_form(strTmp));   // remove "," characters if exist
        
        command = strtok(strTmp, " ");          // binding mnemonic to command variable
        cCase  = commandCase(command);          // commandCase() will return the number of function
        
        switch(cCase){
            case HELP: {                    // help func
                hFunc();
                hisConnect(strcat(command, "\n"));        // record command in history list
                break;
            }
            case DIRECTORY: {               // directory func
                dFunc();
                hisConnect(strcat(command, "\n"));        // record command in history list
                break;
            }
            case QUIT: {                    // quit sicsim
                return 0;
            }
            case HISTORY: {                 // history func
                hisConnect(strcat(command, "\n"));        // record command in histoy list
                hiFunc();
                break;
            }    
            case DUMP: {                    // dump func
                start = strtok(NULL, " ");
                if(!start){                 // dump
                    if(duFunc(NULL, NULL))  // if function did work well,
                        hisConnect(strSave);// record command line in history list
                    continue;
                }
                end = strtok(NULL, "\0");
                if(!end){                   // dump start
                    if(duFunc(start, NULL)) // if function did work well,
                        hisConnect(strSave);// record command line in history list
                    continue;
                }
                if(duFunc(start, end))      // dump start, end
                    hisConnect(strSave);    // if function did work well, 
                                            // record command line in history list
                break;
            }
            case EDIT: {                    // edit address, value
                addr = strtok(NULL, " ");
                val = strtok(NULL, " ");
                if(eFunc(addr, val))        // if funtion did work well,
                    hisConnect(strSave);    // record command line in history list
                break;
            }
            case FILL: {                    // fill start, end, value
                start = strtok(NULL, " ");
                end = strtok(NULL, " ");
                val = strtok(NULL, " ");
                if(fFunc(start, end, val))  // if function did work well,
                    hisConnect(strSave);    // record command line in history list
                break;
            }
            case RESET: {                           // reset function
                resetFunc();
                hisConnect(strcat(command, "\n"));  // record command in history list
                break;
            }
            case OPCODE: {                  // print opcode of the Mnemonic
                mnem = strtok(NULL, " ");
                if(opMnemFunc(mnem))        // if function did work well,
                    hisConnect(strSave);    // record command line in history list
                else printf("Error. No such mnemonic stored.\n");
                break;
            }
            case OPCODELIST: {                          // print all opcode list          
                opListFunc();
                hisConnect(strcat(command, "\n"));      // record command line in history list
                break;
            }
            case TYPE: {                        // print the contents of the file
                fname = strtok(NULL, " ");
                if(tFunc(fname))                // if function did work well,
                    hisConnect(strSave);        // record commend line in history list
                break;
            }
            case ASSEMBLE: {                    // read the *.asm file to make *.lst file and *.obj file
                fname = strtok(NULL, " ");
                if(aFunc(fname))                // if funtion did work well,
                    hisConnect(strSave);        // record commend line in history list
                break;
            }
            case SYMBOL: {                      // print symbol table made during assemble function
                sFunc();
                hisConnect(strcat(command, "\n"));  // record command in history list
                break;
            }
            case PROGADDR: {                      // 
                start = strtok(NULL, " ");
                pFunc(start);
                hisConnect(strcat(command, "\n"));  // record command in history list
                break;
            }
            case LOADER: {                    // 
                prog1 = strtok(NULL, " ");
                prog2 = strtok(NULL, " ");
                if(!prog2){                 //
                    if(lpass1(prog1, NULL, NULL))  // if function did work well,
                        hisConnect(strSave);// record command line in history list
                    continue;
                }
                prog3 = strtok(NULL, "\0");
                if(!prog3){                   // 
                    if(lpass1(prog1, prog2, NULL)) // if function did work well,
                        hisConnect(strSave);// record command line in history list
                    continue;
                }
                if(lpass1(prog1, prog2, prog3))      // 
                    hisConnect(strSave);    // if function did work well, 
                                            // record command line in history list
                break;
            }
            case RUN: {                      // print symbol table made during assemble function
                if(rFunc())
                    hisConnect(strcat(command, "\n"));  // record command in history list
                break;
            }
            case BP: {                      // 
                addr = strtok(NULL, " ");
                if(!addr)
                    bFunc(NULL);
                else bFunc(addr);
                break;
            }
            default:                        // if entered command is not defined
                continue;
        }
    }
    
    return 0;
}

char* regular_form(char* str){      // function that removes any "," character
    int idx=0, cnt=0;
    char* tmpStr = (char*)calloc(strlen(str), sizeof(char));    // temp string for storing new string
    for(int i=0; str[i]!='\n'; i++){    // until reaching end of the line
        cnt++;                          // cnt counts the length of the line
        if(str[i]!=','){                // if there exists any ',' character in the old string,
            tmpStr[idx] = str[i];       // not to transfer it to a new string
            idx++;
        }
    }
    char* regularStr = (char*)malloc((cnt+1)*sizeof(char)); // temp string for storing new string
    strcpy(regularStr, tmpStr);
    regularStr[strlen(regularStr)] = '\0';                  // put '\0' at the end of the new string

    return regularStr;
}

int commandCase(char* command){     // function that returns the type of input command
    if(!strcmp("q", command) || !strcmp("quit", command))
        return 0;
    else if(!strcmp("h", command) || !strcmp("help", command))
        return 1;
    else if(!strcmp("d", command) || !strcmp("dir", command))
        return 2;
    else if(!strcmp("hi", command) || !strcmp("history", command))
        return 3;
    else if(!strcmp("du", command) || !strcmp("dump", command))
        return 4;
    else if(!strcmp("e", command) || !strcmp("edit", command))
        return 5;
    else if(!strcmp("f", command) || !strcmp("fill", command))
        return 6;
    else if(!strcmp("reset", command))
        return 7;
    else if(!strcmp("opcode", command))
        return 8;
    else if(!strcmp("opcodelist", command))
        return 9;
    else if(!strcmp("type", command))
        return 10;
    else if(!strcmp("assemble", command))
        return 11;
    else if(!strcmp("symbol", command))
        return 12;
    else if(!strcmp("progaddr", command))
        return 13;
    else if(!strcmp("loader", command))
        return 14;
    else if(!strcmp("run", command))
        return 15;
    else if(!strcmp("bp", command))
        return 16;
    else
        return -1;
}
