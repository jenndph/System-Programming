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
    EDIT, FILL, RESET, OPCODE, OPCODELIST, TYPE, ASSEMBLE, SYMBOL,
        PROGADDR, LOADER, RUN, BP};


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
int tFunc(char*);


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

//////////////////// Assembler command ////////////////////
typedef struct symbolTable{
    char symbol[10];
    int loc;
    struct symbolTable* link;
}STABLE;
STABLE* st;
STABLE* sHead;

typedef struct objList{
    char objcode[10];
    char loc[5];
    int format;
    int chk;
    struct objList* link;
}OBJ;
OBJ* oHead;
int jsubChk;

FILE *lstfile, *symfile, *semifile, *objfile;
int aFunc(char*);
int pass1(char*);
void sFunc();
char* pass2(char*);

//////////////////// Linker&Loader command ////////////////////
int prog_addr, cs_addr, exec_addr, end;

typedef struct estab{
    char symbol[7];
    int addr;
    struct estab* link;
}ESTAB;
ESTAB** eHead;

void pFunc(char*);
int lpass1(char*, char*, char*);
int lpass2();

//////////////////// Run command ////////////////////
int A, X, L, B, S, T, F, PC, SW;
int CC;
int end_flag;

typedef struct bpList{
    int bpoint;
    struct bpList* link;
}BLIST;
BLIST* bHead;

int rFunc();
int bFunc(char*);
