#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX 1000000

typedef struct historyList{
    int no;
    char command[20];
    struct historyList* link;
}HLIST;
int hisCnt;
HLIST* hisHead;

int addrSave;
unsigned char dumpMem[65536][16];

void hFunc();
int dFunc();
void hiFunc();
void du1Func();
void du2Func();
void du3Func();
void resetFunc();

int main(){
    char strTmp[100];
    char* token[4] = {NULL,};
    int cnt=0;

    hisCnt=1;
    hisHead = NULL;
    HLIST *hisP, *hisTmp;

    addrSave = 0;

    while(1){
        system("clear");
        printf("sicsim> ");
        scanf("%[^\ns]", strTmp);
        char* comm = strtok(strTmp, " ");
        cnt=0;
        while (comm != NULL){
            token[cnt++] = comm;
            comm = strtok(NULL, " ");
        }

        if(!strcmp("h", token[0]) || !strcmp("help", token[0])){
            if(cnt==1){
                hisTmp = (HLIST*)malloc(sizeof(HLIST));
                hisTmp->no = hisCnt++;
                strcpy(hisTmp->command, token[0]);
                hisTmp->link = NULL;
                if (hisHead==NULL)
                    hisHead = hisTmp;
                else{
                    for(hisP=hisHead; hisP->link!=NULL; hisP=hisP->link);
                    hisP->link = hisTmp;
                }
                hFunc();
            }
            else
                continue;
        }
        if(!strcmp("d", token[0]) || !strcmp("dir", token[0])){
            if(cnt==1){
                dFunc();
            }
            else
                continue;
        }
        if(!strcmp("q", token[0]) || !strcmp("quit", token[0])){
            return 0;
        }
        if(!strcmp("hi", token[0]) || !strcmp("history", token[0])){
            if(cnt==1){
                hisTmp = (HLIST*)malloc(sizeof(HLIST));
                hisTmp->no = hisCnt++;
                strcpy(hisTmp->command, token[0]);
                hisTmp->link = NULL;
                if (hisHead==NULL)
                    hisHead = hisTmp;
                else{
                    for(hisP=hisHead; hisP->link!=NULL; hisP=hisP->link);
                    hisP->link = hisTmp;
                }
                hiFunc();
            }
            else
                continue;
        }
        if(!strcmp("dump", token[0]) || !strcmp("du", token[0])){
            if(cnt==1){
                hisTmp = (HLIST*)malloc(sizeof(HLIST));
                hisTmp->no = hisCnt++;
                strcpy(hisTmp->command, token[0]);
                hisTmp->link = NULL;
                if (hisHead==NULL)
                    hisHead = hisTmp;
                else{
                    for(hisP=hisHead; hisP->link!=NULL; hisP=hisP->link);
                    hisP->link = hisTmp;
                }
                du1Func();
            }
            if(cnt==2){
                hisTmp = (HLIST*)malloc(sizeof(HLIST));
                hisTmp->no = hisCnt++;
                strcpy(hisTmp->command, strTmp);
                hisTmp->link = NULL;
                if (hisHead==NULL)
                    hisHead = hisTmp;
                else{
                    for(hisP=hisHead; hisP->link!=NULL; hisP=hisP->link);
                    hisP->link = hisTmp;
                }
                du2Func();
            }
            if(cnt==3){
                hisTmp = (HLIST*)malloc(sizeof(HLIST));
                hisTmp->no = hisCnt++;
                strcpy(hisTmp->command, strTmp);
                hisTmp->link = NULL;
                if (hisHead==NULL)
                    hisHead = hisTmp;
                else{
                    for(hisP=hisHead; hisP->link!=NULL; hisP=hisP->link);
                    hisP->link = hisTmp;
                }
                du3Func();
            }
            else
                continue;
        }


        if(!strcmp("reset", token[0])){
            if(cnt==1){
                hisTmp = (HLIST*)malloc(sizeof(HLIST));
                hisTmp->no = hisCnt++;
                strcpy(hisTmp->command, token[0]);
                hisTmp->link = NULL;
                if (hisHead==NULL)
                    hisHead = hisTmp;
                else{
                    for(hisP=hisHead; hisP->link!=NULL; hisP=hisP->link);
                    hisP->link = hisTmp;
                }
                resetFunc();
            }
            else
                continue;
        }
    }

    return 0;
}

void hFunc(){
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

    printf("PRESS ANY KEY TO GO BACK\n");
    char ch = getchar();
    scanf("%c", &ch);

    return;
}

int dFunc(){    //일반 파일에서 실행 파일과 그냥 파일 분류하기
    DIR* dp;
    char dirt[MAX];
    struct dirent* tmp;
    struct stat buf;

    if(!(dp=opendir(getcwd(dirt,MAX)))){
        printf("ERROR. No such Directory\n");
        return -1;
    }
    while((tmp=readdir(dp))!=NULL){
        lstat(tmp->d_name, &buf);
        if(S_ISDIR(buf.st_mode))
            printf("%s/\t", tmp->d_name);
        else if(S_ISREG(buf.st_mode))
            printf("%s\t", tmp->d_name);
    }

    closedir(dp);

    printf("PRESS ANY KEY TO GO BACK\n");
    char ch = getchar();
    scanf("%c", &ch);

    return 0;
}

void hiFunc(){
    HLIST* hisP;
    if (hisHead==NULL)
        return;
    else{
        for(hisP=hisHead; ; hisP=hisP->link){
            printf("%d\t%s\n", hisP->no, hisP->command);
            if(hisP->link==NULL) break;
        }
    }
    printf("PRESS ANY KEY TO GO BACK\n");
    char ch = getchar();
    scanf("%c", &ch);

    return;
}

void du1Func(){
    printf("Saved address is : %d", addrSave);
}

void du2Func(){

}

void du3Func(){

}

void resetFunc(){
    for(int i=0; i<65536; i++){
        for(int j=0; j<16; j++){
            dumpMem[i][j] = '0';
        }
    }
}
