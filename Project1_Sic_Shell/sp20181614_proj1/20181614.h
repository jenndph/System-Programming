#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX 100000

//////////////////// Command Case ////////////////////
enum commEnum {QUIT=0, HELP, DIRECTORY, HISTORY, DUMP,
    EDIT, FILL, RESET, OPCODE, OPCODELIST};


//////////////////// Shell command ////////////////////
typedef struct historyList{
    int no;
    char command[30];
    struct historyList* link;
}HLIST;
HLIST* hisHead;
int hisCnt;

void hFunc();
int dFunc();
void hisConnect(char []);
void hiFunc();


//////////////////// Memory command ////////////////////
int addrSave;
unsigned char dumpMem[65536][16];

int duFunc(char*, char*);
int eFunc(char*, char*);
int fFunc(char*, char*, char*);
void resetFunc();


//////////////////// Opcode command ////////////////////
typedef struct hashTable{
    int opcode;
    char mnemonic[10];
    int format;
    struct hashTable* link;
}HTABLE;
HTABLE** op;

int makeHash();
int opMnemFunc(char*);
void opListFunc();
