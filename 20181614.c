#include "20181614.h"

char* regular_form(char* str);

int main(){
    char strTmp[30], strSave[30];
    char* command;
    char *start, *end;
    int cCase;

    hisCnt = 1;
    hisHead = NULL;

    while(1){
        printf("sicsim> ");
        fgets(strTmp, sizeof(strTmp), stdin); 
        regular_form(strTmp);
        strcpy(strSave, strTmp);
        
        command = strtok(strTmp, " ");
        cCase  = commandCase(command);    

        printf("command is %s\n", command);
        printf("%d\n", cCase);

        switch(cCase){
            case HELP: {
                           //printf("HELP\n");
                           hisConnect(strSave);
                           hFunc();
                           break;
                        }
            case DIRECTORY: {
                                //printf("DIRECTORY\n");
                                hisConnect(strSave);
                                dFunc();
                                break;
                            }
            case QUIT: {
                           //printf("BYE!\n");
                           return 0;
                       }
            case HISTORY: {
                            //printf("HISTORY\n");
                            hisConnect(strSave);
                            hiFunc();
                            break;
                          }

            case DUMP: {
                           printf("DUMP\n");
                           strcpy(strTmp, strSave);
                           printf("strTmp is %s\n", strTmp);
                           command = strtok(strTmp, " ");
                           printf("command is %s\n", command);
                           start = strtok(NULL, ",");
                           if(!start){ // dump

                           }
                           printf("start is %s\n", start);
                           end = strtok(NULL, "\0");
                           if(!end){    // dump start

                           }
                           printf("end is %s\n", end);
                           /*
                           for(int i=1; i<strlen(end); i++){
                               strcpy(end[i-1], end[i]);
                           }
                           printf("deleted end is %s", end);
                           */
                           // dump start end
                           
                           break;
                       }
            case EDIT: {
                           printf("EDIT\n");
                           break;
                       }
            case FILL: {
                           printf("FILL\n");
                           break;
                       }
            case RESET: {
                            printf("RESET\n");
                            break;
                        }
            case OPCODE: {
                             printf("OPCODE\n");
                             break;
                         }
            case OPCODELIST: {
                                 printf("OPCODELIST\n");
                                 break;
                             }
            default:
                             continue;
        }
    }

    return 0;
}

char* regular_form(char* str){

    printf("%s\n", str);
    str[strlen(str)-1] = '\0';
    for(int i=0; str[i]!='\0'; i++){
        if(str[i]==','){
            strcpy(str, str+1);
            str--;
        }
    }
    printf("%s\n", str);
    return str;
}
