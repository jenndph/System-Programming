#include "20181614.h"

char* regular_form(char* str);

int main(){
    if(makeHash()==-1) return 0;

    char strTmp[30], strSave[30];
    char* command;
    char *start, *end, *addr, *val, *mnem;
    int cCase;

    hisCnt = 1;
    hisHead = NULL;

    addrSave = 0;
    for(int i=0; i<65536; i++)
        for(int j=0; j<16; j++)
            dumpMem[i][j] = 0x00;

    while(1){
        printf("sicsim> ");
        fgets(strTmp, sizeof(strTmp), stdin); 
        strTmp[strlen(strTmp)-1] = '\0';

        strcpy(strSave, strTmp);
        strcpy(strTmp, regular_form(strTmp));
        
        command = strtok(strTmp, " ");
        cCase  = commandCase(command);    

        switch(cCase){
            case HELP: {
                           hisConnect(strSave);
                           hFunc();
                           break;
                        }
            case DIRECTORY: {
                                hisConnect(strSave);
                                dFunc();
                                break;
                            }
            case QUIT: {
                           return 0;
                       }
            case HISTORY: {
                            hisConnect(strSave);
                            hiFunc();
                            break;
                          }

            case DUMP: {
                           hisConnect(strSave);
                           start = strtok(NULL, " ");
                           if(!start){ // dump
                               duFunc(NULL, NULL);
                               continue;
                           }
                           end = strtok(NULL, "\0");
                           if(!end){    // dump start
                                duFunc(start, NULL);
                                continue;
                           }
                           
                           // dump start end
                           duFunc(start, end);
                           
                           break;
                       }
            case EDIT: {
                           hisConnect(strSave);
                           addr = strtok(NULL, " ");
                           val = strtok(NULL, " ");
                           eFunc(addr, val);
                           break;
                       }
            case FILL: {
                           hisConnect(strSave);
                           start = strtok(NULL, " ");
                           end = strtok(NULL, " ");
                           val = strtok(NULL, " ");
                           fFunc(start, end, val);
                           break;
                       }
            case RESET: {
                            resetFunc();
                            break;
                        }
            case OPCODE: {
                             printf("OPCODE\n");
                             mnem = strtok(NULL, " ");
                             opMnemFunc(mnem);
                             break;
                         }
            case OPCODELIST: {
                                 printf("OPCODELIST\n");
                                 opListFunc();
                                 break;
                             }
            default:
                             continue;
        }
    }

    return 0;
}

char* regular_form(char* str){
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
