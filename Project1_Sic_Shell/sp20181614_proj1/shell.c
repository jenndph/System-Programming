#include "20181614.h"

void hFunc(){           // Print a list of all possible Shell commands on the screen
    printf("h[elp]\n");
    printf("d[ir]\n");
    printf("q[uit]\n");
    printf("hi[story]\n");
    printf("du[mp] [start, end]\n");
    printf("e[dit] address, value\n");
    printf("f[ill] start, end, value\n");
    printf("reset\n");
    printf("opcode mnemonic\n");
    printf("opcodelist\n");

    return;
}

int dFunc(){                                    // Print all files in the current directory
    DIR* dp;
    char* token;
    struct dirent* tmp;                         // struct pointer for directory entry
    struct stat buf;                            // struct for entry state

    if(!(dp=opendir("."))){                     // open current directory
        printf("ERROR. No such directory.\n");
        return -1;
    }
    while((tmp=readdir(dp))!=NULL){             // read directory entry
        lstat(tmp->d_name, &buf);               // save information of file tmp's state at buf
        printf("\t%s", tmp->d_name);            // print file name
        if(S_ISDIR(buf.st_mode))                // if the entry is directory file, add '/'
            printf("/");
        else{                                   // distinguish whether that file is execuatble
            token = strtok(tmp->d_name, ".");   // cut the file name by dot('.')
            if(token!=NULL){
                if((token = strtok(NULL, " "))!=NULL){
                    if(!strcmp(token, "out"))  // if the entry has '.out' extension,
                        printf("*");           // add '*'
                }
            }
        }
        printf("\n");
    }
    closedir(dp);                               // close the directory

    return 0;
}

void hisConnect(char strTmp[]){     // Connect the new history struct
    HLIST *hisP, *hisTmp;
    hisTmp = (HLIST*)malloc(sizeof(HLIST)); // initialize new history struct pointer
    hisTmp-> no = hisCnt++;                 // store the entering number of the command 
    strcpy(hisTmp->command, strTmp);
    hisTmp->link = NULL;
    if(hisHead==NULL)                       // connect first struct to the linked list
        hisHead = hisTmp;
    else{                                   // connect struct at the last of the linked list
        for(hisP=hisHead; hisP->link!=NULL; hisP=hisP->link);
        hisP->link = hisTmp;
    }
}

void hiFunc(){          // Show the commands list so far in order with numbers
    HLIST* hisP;
    if (hisHead==NULL)                              // if the list is empty
        return;
    else
        for(hisP=hisHead; ; hisP=hisP->link){       // show the history list
            printf("\t%d  %s", hisP->no, hisP->command);
            if(hisP->link==NULL) break;
        }

    return;
}
