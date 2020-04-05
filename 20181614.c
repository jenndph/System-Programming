#include "20181614.h"

char* regular_form(char* str);
int commandCase(char* command);

int main(){
    if(makeHash()==-1) return 0;    // make Hash Table
    
    char strTmp[30], strSave[30];
    char* command;
    char *start, *end, *addr, *val, *mnem;
    int cCase;

    hisCnt = 1;                     // initialize the number of saved history list
    hisHead = NULL;                 // initialize the start of the history list
    
    addrSave = 0;                   // initialize the variable storing last printed address
    for(int i=0; i<65536; i++)      // initialize the memory we use with 0x00
        for(int j=0; j<16; j++)
            dumpMem[i][j] = 0x00;
    
    while(1){
        printf("sicsim> ");
        fgets(strTmp, sizeof(strTmp), stdin);   // get command line
        strTmp[strlen(strTmp)-1] = '\0';        // put '\0' at the end of the line
        
        strcpy(strSave, strTmp);                // create copy of command line recorded in history list
        strcpy(strTmp, regular_form(strTmp));   // remove "," characters if exist
        
        command = strtok(strTmp, " ");          // binding mnemonic to command
        cCase  = commandCase(command);
        
        switch(cCase){
            case HELP: {                    // help func
                hFunc();
                hisConnect(strSave);        // record command line in history list
                break;
            }
            case DIRECTORY: {               // directory func
                dFunc();
                hisConnect(strSave);        // record command line in history list
                break;
            }
            case QUIT: {                    // quit sicsim
                return 0;
            }
            case HISTORY: {                 // history func
                hisConnect(strSave);        // record command line in histoy list
                hiFunc();
                break;
            }    
            case DUMP: {
                start = strtok(NULL, " ");
                if(!start){                 // dump
                    if(duFunc(NULL, NULL))
                        hisConnect(strSave);// record command line in history list
                    continue;
                }
                end = strtok(NULL, "\0");
                if(!end){                   // dump start
                    if(duFunc(start, NULL))
                        hisConnect(strSave);// record command line in history list
                    continue;
                }
                if(duFunc(start, end))      // dump start, end
                    hisConnect(strSave);    // record command line in history list
                break;
            }
            case EDIT: {                    // edit address, value
                addr = strtok(NULL, " ");
                val = strtok(NULL, " ");
                if(eFunc(addr, val))
                    hisConnect(strSave);
                break;
            }
            case FILL: {                    // fill start, end, value
                start = strtok(NULL, " ");
                end = strtok(NULL, " ");
                val = strtok(NULL, " ");
                if(fFunc(start, end, val))
                    hisConnect(strSave);
                break;
            }
            case RESET: {                   // reset memory
                resetFunc();
                break;
            }
            case OPCODE: {                  // print opcode of the Mnemonic
                mnem = strtok(NULL, " ");
                if(opMnemFunc(mnem))
                    hisConnect(strSave);
                else printf("Error. No such mnemonic stored.\n");
                break;
            }
            case OPCODELIST: {
                opListFunc();
                hisConnect(strSave);
                break;
            }
            default:
                continue;
        }
    }
    
    return 0;
}

char* regular_form(char* str){      // function that removes any "," character
    int idx=0;
    char* tmpStr = (char*)malloc(strlen(str)*sizeof(char));
    for(int i=0; str[i]!='\0'; i++){
        if(str[i]!=','){
            tmpStr[idx] = str[i];
            idx++;
        }
    }
    return tmpStr;
}

int commandCase(char* command){     // function that returns the type of input
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
    else
        return -1;
}
