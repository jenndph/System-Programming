#include "20181614.h"

char* remove_space(char* str);
char* make_regular(char* str);
char Hex_to_Str(int num);

int symbolChk;      // if there is symbol in a string, symbolChk = 1

int loc, idx;       // location and index
int sLoc, eLoc;     // saved location for *.obj file
char progName[7];   // name of the program for *.obj file

int aFunc(char* fname){
    FILE* fp = fopen(fname, "r");
    if(!fp){
        printf("Error. File does not exist.\n");
        return 0;
    }
    st->link=NULL;      // initialize the symbol list; 

    /******************** Pass 1 ********************/
    idx = 5;
    int format;
    char str[60], saveStr[60], lstfname[15], saveName[15];
    char *name;
    name= strtok(fname, ".");           // make name of *.lst file
    strcpy(saveName, name);
    strcpy(lstfname, name);
    strcat(lstfname, ".lst");
    semifile = fopen("semi.lst", "w");  // semifile save the result of pass1
    symfile = fopen("symbol.txt", "w"); // symfile save the symbol list used at *.asm file

    loc = 0x0000;       // initialization of location
    sHead = (STABLE*)malloc(sizeof(STABLE)); // temp linked list for save used-symbol list
    sHead->link = NULL;                      // initialization 
    while(fgets(str, sizeof(str), fp)!=NULL){
        symbolChk=0;                        // if there is symbol in that string, symbolChk = 1
        strcpy(saveStr, str);               // save string for printing the result of pass1
        
        str[strlen(str)-1] = '\0';
        strcpy(str, remove_space(str));     // delete unnecessary blank in the string
        format = pass1(str);                // pass1 funtion will return the format of the mnemonic in the string

        if(symbolChk)                               // if there exists a new symbol, record that in symfile 
            fprintf(symfile, "\t%04X\n", loc);
        if(format=='B' || format=='.')              // if object code is not generated
            fprintf(semifile, "%-4d\t    \t%s", idx, saveStr);
        else if(format=='E'){                       // end of the file
            fprintf(semifile, "%-4d\t    \t%s", idx, saveStr);
            break;
        }
        else if(format==-1) return -1;              // if there is an error
        else{                                       // record the assembly line with loc, and idx
            fprintf(semifile, "%-4d\t%04X\t%s", idx, loc, saveStr);
            loc += format;
        }
        idx += 5;
        if(idx==10) sLoc = loc;     // save start loc for making *.obj file
    }
    eLoc = loc;                     // save end loc for making *.obj file
    fclose(fp);
    fclose(semifile);
    fclose(symfile);

    /******************** Make Linked list of Symbol ********************/
    STABLE *sPoint, *sPrev;
    symfile = fopen("symbol.txt", "r");
    while(fscanf(symfile, "%s %X", name, &loc)!=EOF){
        STABLE* sTmp = (STABLE*)malloc(sizeof(STABLE));     // initialization
        strcpy(sTmp->symbol, name);                         // save the loc and name of the symbol
        sTmp->loc = loc; sTmp->link = NULL;
        if(st->link==NULL){                                 // first symbol
            st->link = sTmp;
        }
        else{
            sPrev = st;
            for(sPoint=st->link; ; sPoint=sPoint->link, sPrev=sPrev->link){
                if(sPoint==NULL){
                    sPrev->link = sTmp;
                    break;
                }
                if(strcmp(sPoint->symbol,sTmp->symbol)>0){  // for sorting symbols
                    sTmp->link = sPoint;
                    sPrev->link = sTmp;
                    break;
                }
            }
        }
    }
    fclose(symfile);

    /******************** Pass 2 ********************/
    char* objcode = (char*)malloc(9*sizeof(char));          // objcode will save the assembly line's object code
    lstfile = fopen(lstfname, "w");                         // lstfile save the result of pass2
    semifile = fopen("semi.lst", "r");
    symfile = fopen("symbol.txt", "r");

    oHead = (OBJ*)malloc(sizeof(OBJ));          // head of a linked list saving object codes
    oHead->link = NULL;                         // initialization
    jsubChk=0;                                  // check the number of Modification Record
    while(fgets(str, sizeof(str), semifile)!=NULL){
        strcpy(saveStr, str);                   // save the string for printing the result of pass2
        saveStr[strlen(saveStr)-1] = ' ';
        str[strlen(str)-1] = '\0';
        strcpy(str, make_regular(str));         // delete unnecessaty blank or tab in the string
         
        objcode = pass2(str);                   // pass2 funtion will return the object code of the assembly line
        if(!strcmp(objcode, "ERROR")) return -1;                // if there is an error
        if(!strcmp(objcode, "BASE") || !strcmp(objcode, "END")) // if object code is not generated
            objcode = " ";

        fprintf(lstfile, "%-50s\t%s\n", saveStr, objcode);      // record the string with its object code
        if(!strcmp(objcode, "END")) break;
    }
    fclose(semifile);
    fclose(lstfile);
    fclose(symfile);
    printf("Completed lst file...\n");
    
    /******************** (Make *.obj file) ********************/
    char objfname[15], HRec[21], TRec[80], ERec[10];
    name = strtok(saveName, ".");       // make name of *.obj file
    strcpy(objfname, name);
    strcat(objfname, ".obj");
    objfile = fopen(objfname, "w");
    
    HRec[0] = 'H';                      // initialize the header record
    int i, k;
    for(i=0; progName[i]; i++){         // program name of the *.asm file
        HRec[i+1] = progName[i];
    }
    while(i<7) HRec[++i] = ' ';         // fill in the blanks if there are any left spaces
    loc = sLoc;                         // save the starting address for end record
    for(i=12; i>7; i--)                 // binding the starting address of the program in bytes (hex)
        {HRec[i] = Hex_to_Str(sLoc%16); sLoc /= 16;}
    HRec[7] = Hex_to_Str(sLoc);
    sLoc = loc;                         // save the starting address for end record
    loc = eLoc - loc;                   // binding the length of object program in bytes (hex)
    for(i=18; i>13; i--)
        {HRec[i] = Hex_to_Str(loc%16); loc /= 16;}
    HRec[13] = Hex_to_Str(loc);
    HRec[19] = '\n';    HRec[20] = '\0';
    fputs(HRec, objfile);               // print the header record in *.obj file

    OBJ* oPoint;
    char** modRec = (char**)malloc(jsubChk*sizeof(char*));  // make array for saving modification record
    for(k=0; k<jsubChk; k++) modRec[k] = (char*)malloc(11*sizeof(char));

    int len=0, varChk=0, t=0; i=0; k=0;
    for(oPoint=oHead->link; oPoint; oPoint=oPoint->link){   // while there is an object code remaining
        if(oPoint->format==4 && oPoint->chk==1){            // when modification record is needed (JSUB)
            loc = strtol(oPoint->loc, NULL, 16);
            modRec[k][0] = 'M';                             // initialize the modification record
            modRec[k][6] = Hex_to_Str(loc%16)+0x0001; loc /= 16;    // pass a byte
            for(t=5; t>=1; t--)                                     // save the location
                {modRec[k][t] = Hex_to_Str(loc%16); loc /= 16;}
            modRec[k][7]='0'; modRec[k][8]='5'; 
            modRec[k][9] ='\n'; modRec[k][10] = '\0';
            k++;
        }
        if(!strcmp(oPoint->objcode, "VARIABLE")) varChk+=1;     // if variable line, varChk = 1, 
        else varChk=0;
        if(len>=0x1C || varChk>=1){     // if the line is excess the limit(I set it to 0x16) or has variable 
                                        // print that text record line and move to the next line
            if(varChk>1) continue;      // continuous variable
            TRec[7] = Hex_to_Str(len/16);   // length of object code in this record in bytes (hex)
            TRec[8] = Hex_to_Str(len%16);   
            TRec[i]='\n'; TRec[i+1]='\0';
            fputs(TRec, objfile);           // print the text record in *.obj file
            len=0;                          // initialize the length of object code
            if(varChk==1) continue;         // if variable line, pass to next object code
        }
        if(len==0){                                 // start of a text record
            TRec[0] = 'T';                          // initialize the text record
            TRec[1] = TRec[2] = '0';                // initialize start of starting address
            TRec[7] = TRec[8] = '0';                // initialize length of object code
            for(i=6; (oPoint->loc)[6-i]; i--)       // set starting address for object code in this record (hex)
                TRec[i] = (oPoint->loc)[strlen(oPoint->loc)-(6-i)-1];
            for(i=9; (oPoint->objcode)[i-9]; i++)   // record the first object code of the line
                TRec[i] = (oPoint->objcode)[i-9];
            len += strlen(oPoint->objcode)/2;       // record the length of the line
        }
        else{
            for(t=i; (oPoint->objcode)[t-i]; t++)   // record the object code in the line
                TRec[t] = (oPoint->objcode)[t-i];
            i = t;
            len += strlen(oPoint->objcode)/2;
        }
    }
    if(len!=0){                         // print the rest object code line in *.obj file
        TRec[7] = Hex_to_Str(len/16);
        TRec[8] = Hex_to_Str(len%16);
        TRec[i]='\n'; TRec[i+1]='\0';
        fputs(TRec, objfile);           // print the text record in *.obj file
    }
    for(k=0; k<jsubChk; k++)            // print the modification record in *.obj file
        fputs(modRec[k], objfile);
    ERec[0] = 'E';                      // initialize the end record
    for(i=6; i>1; i--)                  // save the address of first executable instruction in object program (hex)
        { ERec[i] = Hex_to_Str(sLoc%16); sLoc /= 16; }
    ERec[1] = Hex_to_Str(sLoc);
    ERec[7] = '\n'; ERec[8]='\0';
    fputs(ERec, objfile);
    printf("Completed obj file...\n");
    fclose(objfile);

    printf("Successfully assemble %s.asm\n", name);
    return 1;
}

int pass1(char* str){       // Get the format of the line's instruction or variable or constant to set loc
    STABLE *sPoint;
    STABLE *sTmp = (STABLE*)malloc(sizeof(STABLE));

    char *token, *sym;
    token = strtok(str, " ");
    if(idx==5) strcpy(progName,token);
    if(!strcmp(token, "END")) return 'E';       // end of the assembly file
    if(!strcmp(token, "BASE")) return 'B';      // BASE
    if(token[0]=='.') return '.';               // annotation section
    if(token[0]=='+') return 4;                 // format 4

    HTABLE* hPoint;
    int alpha, format, chk=0;
    alpha = token[0]%20;                            // check the first alphabet of token
    for(hPoint=op[alpha]; ; hPoint=hPoint->link){
        if(hPoint==NULL) break;
        if(!strcmp(hPoint->mnemonic, token)){       // if the token is mnemonic, set chk = 1  
            format = hPoint->format;
            chk=1;
        }
    }
    if(!chk){       // first token was jut symbol, or error
        sym = token; 
        chk = 0;
        token = strtok(NULL, " ");
        if(!strcmp(token, "START")) {   // if the token is "START", initialize start loc with number after token
            token = strtok(NULL, " ");
            loc = strtol(token, NULL, 16);
            return 0;
        }
        if(token[0]=='+'){      // format 4 -> first token was symbol
            strcpy(sTmp->symbol, sym); sTmp->loc = loc; sTmp->link = NULL;
            for(sPoint=sHead; ; sPoint=sPoint->link){
                if(!strcmp(sPoint->symbol, sTmp->symbol)) // overlapping variable
                    { printf("Line %d Error. Duplicated Symbol\n", idx); return -1; }
                if(sPoint->link==NULL){                   // link the symbol at the last of the linked list
                    sPoint->link = sTmp;
                    break;
                }
            }
            fprintf(symfile, "%9s", sym);                 // print the symbol at symfile
            symbolChk=1;
            return 4;
        }

        alpha = token[0]%20;                        // check the first alphabet of token
        for(hPoint=op[alpha]; ; hPoint=hPoint->link){
            if(hPoint==NULL) break;
            if(!strcmp(hPoint->mnemonic, token)){   // if the token is mnemonic, set chk = 1
                format = hPoint->format;
                chk=1;
            }
        }
        if(!chk){   // error or variable, constant
            if(!strcmp(token,"RESW")) {     // Word variable 
                strcpy(sTmp->symbol, sym); sTmp->loc = loc; sTmp->link = NULL;
                for(sPoint=sHead; ; sPoint=sPoint->link){
                    if(!strcmp(sPoint->symbol, sTmp->symbol))   // overlapping variable
                        { printf("Line %d Error. Duplicated Symbol\n", idx); return -1; }
                    if(sPoint->link==NULL){                     // link the symbol at the last of the linked list
                        sPoint->link = sTmp;
                        break;
                    }
                }
                fprintf(symfile, "%9s", sym);                   // print the symbol at symfile
                symbolChk=1;
                token = strtok(NULL, " ");
                return 3*atoi(token);                           // return word size * number
            }
            else if(!strcmp(token,"RESB")){     // Byte variable 
                strcpy(sTmp->symbol, sym); sTmp->loc = loc; sTmp->link = NULL;
                for(sPoint=sHead; ; sPoint=sPoint->link){
                    if(!strcmp(sPoint->symbol, sTmp->symbol))   // overlapping variable
                        { printf("Line %d Error. Duplicated Symbol\n", idx); return -1; }
                    if(sPoint->link==NULL){                     // link the symbol at the last of the linked list
                        sPoint->link = sTmp;
                        break;
                    }
                }
                fprintf(symfile, "%9s", sym);                   // print the symbol at symfile
                symbolChk=1;
                token = strtok(NULL, " ");
                return 1*atoi(token);                           // return byte size * number
            }
            else if(!strcmp(token,"WORD")){     // Word Constant
                strcpy(sTmp->symbol, sym); sTmp->loc = loc; sTmp->link = NULL;
                for(sPoint=sHead; ; sPoint=sPoint->link){
                    if(!strcmp(sPoint->symbol, sTmp->symbol))   // overlapping variable
                        { printf("Line %d Error. Duplicated Symbol\n", idx); return -1; }
                    if(sPoint->link==NULL){                     // link the symbol at the last of the linked list
                        sPoint->link = sTmp;
                        break;
                    }
                }
                fprintf(symfile, "%9s", sym);                   // print the symbol at symfile
                symbolChk=1;
                token = strtok(NULL, " ");                      // return word size
                return 3; 
            }
            else if(!strcmp(token,"BYTE")){     // Byte Constant
                strcpy(sTmp->symbol, sym); sTmp->loc = loc; sTmp->link = NULL;
                for(sPoint=sHead; ; sPoint=sPoint->link){
                    if(!strcmp(sPoint->symbol, sTmp->symbol))   // overlapping variable
                        { printf("Line %d Error. Duplicated Symbol\n", idx); return -1; }
                    if(sPoint->link==NULL){                     // link the symbol at the last of the linked list
                        sPoint->link = sTmp;
                        break;
                    }
                }
                fprintf(symfile, "%9s", sym);                   // print the symbol at symfile
                symbolChk=1;    
                token = strtok(NULL, " ");
                if(token[0]=='X') return strlen(token-3)/2;     // return the half length of token after X
                else{   // 'C'
                    return strlen(token)-3;                     // return the length of token after C
                }
            }
            else{ printf("Line %d Error. Use Undefined Symbol or Mnemonic Name\n", idx); return -1; }
        }
         
        strcpy(sTmp->symbol, sym); sTmp->loc = loc; sTmp->link = NULL;      // save left symbols
        for(sPoint=sHead; ; sPoint=sPoint->link){
            if(sPoint->link==NULL){
                sPoint->link = sTmp;
                break;
            }
            if(!strcmp(sPoint->symbol, sTmp->symbol))                       // overlapping variable
                { printf("Line %d Error. Duplicated Symbol\n", idx); return -1; }
        }
        fprintf(symfile, "%9s", sym);                                       // print the symbol at symfile
        symbolChk=1;
    }
    return format;
}

void sFunc(){       // Print the symbol table using linked-list of symbols saved during assemble function
    STABLE* sPoint;
    for(sPoint=st->link; sPoint!=NULL; sPoint=sPoint->link){
        printf("\t%-10s%04X\n", sPoint->symbol, sPoint->loc);
    }
}

char* remove_space(char* str){      // Remove unnecessary space
    int trans_idx=0, chk=0;
    char* tmpStr = (char*)calloc(strlen(str+1), sizeof(char));    // temp string for storing new string
    for(int i=0; str[i]; i++){                                    // until reaching end of the line
        if(str[i]!=' ' || (chk==0&&trans_idx!=0)){                // if it is not blank or necassary blank
            chk = (str[i]==' ')? 1 : 0;                           // set blank count
            tmpStr[trans_idx] = str[i];                           // copy the character to a new string
            trans_idx++;
        }
    }
    tmpStr[trans_idx] = '\0';
    
    char* regularStr = (char*)malloc((trans_idx+2)*sizeof(char)); // temp string for storing new string
    strcpy(regularStr, tmpStr);
    free(tmpStr);
    regularStr[strlen(regularStr)] = '\0';                        // put '\0' at the end of the new string
    return regularStr;
}

char* make_regular(char* str){      // Remove unnecessary space and tab
    int trans_idx=0, chk=0;
    char* tmpStr = (char*)calloc(strlen(str+1), sizeof(char));    // temp string for storing new string
    for(int i=0; str[i]; i++){                                    // until reaching end of the line
        if(str[i]!=' ' || (chk==0 && trans_idx!=0)){              // if it is not blank or necseeary blank
            if(str[i]=='\t'){                                     // if it is tab,
                if(!chk){                                         // change it into blank if needed or pass
                    tmpStr[trans_idx++] = ' ';
                    chk=1;
                }
                continue;
            }
            chk = (str[i]==' ')? 1 : 0;                           // set blank count
            tmpStr[trans_idx] = str[i];                           // copy the character to a new string
            trans_idx++;
        }
    }
    tmpStr[trans_idx] = '\0';

    char* regularStr = (char*)malloc((trans_idx+2)*sizeof(char)); // temp string for storing new string
    strcpy(regularStr, tmpStr);
    free(tmpStr);
    regularStr[trans_idx] = '\0';                                 // put '\0' at the end of the new string

    return regularStr;
}

char Hex_to_Str(int num){       // Change the number into Charater
    if(num>=0 && num<=9) return num + '0';
    else return num + 'A' - 10;
}
