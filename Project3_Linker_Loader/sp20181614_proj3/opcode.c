#include "20181614.h"

void hashConnect(int o, char* m, char* f){          // save Mnemonic in linked list format
    HTABLE* tmp = (HTABLE*)malloc(sizeof(HTABLE));  // initialize a new struct of hash table struct
    tmp->opcode = o;
    strcpy(tmp->mnemonic, m);
    if(!strcmp(f, "1")) tmp->format = 1;            // save format as integer value
    else if(!strcmp(f, "2")) tmp->format = 2;
    else tmp-> format = 3;                          // if format is 3/4, save it as 3
    tmp->link = NULL;

    HTABLE* hPoint;
    int alpha = m[0]%20;                            // Categorize mnemonics by their first alphabet
    for(hPoint=op[alpha]; hPoint->link!=NULL; hPoint=hPoint->link);
    hPoint->link = tmp;
}

int makeHash(){                                     // Make hash table of the Mnemonics
    op = (HTABLE**)malloc(20*sizeof(HTABLE*));              // initialize the hash table
    for(int i=0; i<20; i++){
        op[i] = (HTABLE*)malloc(sizeof(HTABLE));
        op[i]->opcode = -1;
        strcpy(op[i]->mnemonic, " ");
        op[i]->format = 0;              
        op[i]->link = NULL;
    }

    FILE* fp;
    if((fp = fopen("opcode.txt", "r")) == NULL){            // open opcode.txt
        printf("Error. No such file.\n");
        return -1;
    }
    int o;
    char str[10], form[5];
    while(fscanf(fp, "%02X %s %s", &o, str, form)!=EOF){    // read a line
        hashConnect(o, str, form);                          // connect that into linked list format
    }
    fclose(fp);                                             // close opcode.txt

    return 0;
}

int opMnemFunc(char* mnem){                     // Print the corresponing opcode of the Memonic
    HTABLE* hPoint;
    int alpha, chk=0;
    alpha = mnem[0]%20;                         // find the correct index with first alphabet of the mnemonic
    for(hPoint=op[alpha]; ; hPoint=hPoint->link){
        if(hPoint==NULL) break;           // if Mnemonic starts with that alphabet does not exist
        if(!strcmp(hPoint->mnemonic, mnem)){    // if Mmenonic information exists,
            printf("%s is %02X\n", hPoint->mnemonic, hPoint->opcode);
            chk = 1;                            // print that opcode and make chk value is 1.
            break;
        }
    }
    return chk;
}

void opListFunc(){                              // Print the contents of the Hash table
    HTABLE* hPoint;
    for(int i=0; i<20; i++){
        printf("%d : ", i);
        for(hPoint=op[i]; ; hPoint=hPoint->link){
            if(hPoint->link==NULL) break;       // if Mnemonics start with that alphabet do not exist
            printf("[%s,%02X]", hPoint->link->mnemonic, hPoint->link->opcode); // else
            if(hPoint->link->link!=NULL) printf(" -> ");
        }
        printf("\n");
    }
}
