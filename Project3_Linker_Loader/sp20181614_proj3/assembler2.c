#include "20181614.h"

char hex_to_str(int num);
int regList(char R);

int B, idx;     // B saves the location of symbol saved in B register

char* pass2(char* str){                 // Return the object code of the string
    OBJ *oPoint;
    STABLE* sPoint;                     // symbol table pointer
    HTABLE* hPoint;                     // opcode table pointer
    
    char* token;
    int k, varChk=0;
    token = strtok(str, " ");           // index
    idx = strtol(token, NULL, 10);
    if(idx==5) return " ";

    int chk=0;
    token = strtok(NULL, " ");
    if(token[0]=='.') return " ";       // annotation line
    else if(!strcmp(token, "BASE")) {   // save base address in B
        token = strtok(NULL, " ");
        for(sPoint=st; sPoint!=NULL; sPoint=sPoint->link){
            if(!strcmp(sPoint->symbol, token)){ 
                B = sPoint->loc;
                chk = 1;
                break;
            }
        }
        if(!chk)            // Undefined symbol error
        { printf("Line %d Error. Use Undefined Symbol\n", idx); return "ERROR";}
        else return "BASE";
    }
    else if(!strcmp(token, "END")) return "END";

    OBJ* oTmp = (OBJ*)calloc(1, sizeof(OBJ)); // initialie object code linked list pointer
    oTmp->chk=0;
    oTmp->link = NULL;
    int format=0, loc, alpha, isMnem=0;
    int opcode;
    strcpy(oTmp->loc, token);
    loc = strtol(token, NULL, 16);
    //loc += 0x0003;                          // PC is now pointing next loc
    token = strtok(NULL, " ");              // symbol or mnemonic
    if(token[0]=='+') {                     // format 4
        format = 4;
        for(k=1; token[k]; k++){            // remove the '+' character at the first
            token[k-1] = token[k];
        }
        token[k-1] = '\0';
        if(!strcmp(token, "JSUB")) 
            {jsubChk+=1; oTmp->chk=1;}
    }
    alpha = token[0]%20;                    // check the first alphabet of token
    for(hPoint=op[alpha]; ; hPoint=hPoint->link){
        if(hPoint==NULL) break;
        if(!strcmp(token, hPoint->mnemonic)){
            opcode = hPoint->opcode;
            if(format==0) format = hPoint->format;
            isMnem = 1;
            break;
        }
    }
    if(!isMnem){                            // prev token is symbol or error
        token = strtok(NULL, " ");  
        if(token[0]=='+') {                 // format 4
            format = 4;
            for(k=1; token[k]; k++){        // remove the '+' character at the first
                token[k-1] = token[k];
            }
            token[k-1] = '\0';
            if(!strcmp(token, "JSUB")) 
                {jsubChk+=1; oTmp->chk=1;}
        }
        alpha = token[0]%20;                // check the first alphabet of token
        for(hPoint=op[alpha]; ; hPoint=hPoint->link){
            if(hPoint==NULL) break;
            if(!strcmp(token, hPoint->mnemonic)){
                opcode = hPoint->opcode;
                if(format==0) format = hPoint->format;
                isMnem = 1;
                break;
            }
        }
        if(!isMnem) {   // token is variable, constant, or "START". else error.
            if(strcmp(token, "RESW") && strcmp(token, "RESB") 
                    && strcmp(token, "WORD") && strcmp(token, "BYTE") && strcmp(token, "START"))
                {printf("Line %d Error. Use Undefined Mnemonic\n", idx); return "ERROR";}
        }
    }

    char* objcode = (char*)calloc(2*format, sizeof(char)); 
    if(format==1){                                  // format 1
        loc += format;
        objcode[0] = hex_to_str(opcode/16);         // save opcode
        objcode[1] = hex_to_str(opcode%16);         // "
    }
    if(format==2){                                  // format 2
        loc += format;
        objcode[0] = hex_to_str(opcode/16);         // save opcode
        objcode[1] = hex_to_str(opcode%16);         // "
        token = strtok(NULL, ",");                  // get first register
        objcode[2] = hex_to_str(regList(token[0])); // save register number
        if(!strcmp(token, "SW")) 
            objcode[2] = hex_to_str(9);
        if((token = strtok(NULL, " "))==NULL)       // get second register
            objcode[3] = '0';                       // and if there is no second register, fill number as zero
        else {
            objcode[3] = hex_to_str(regList(token[0]));
            if(!strcmp(token, "SW")) 
                objcode[3] = hex_to_str(9);
        }
    }
    int n, i, x, b, p, e;
    int num, disp = -1;
    int tempNum;
    if(format==3){                                  // format 3
        loc += format;
        token = strtok(NULL, " ");
        if(!token) {                                // if there is no operand
            disp = 0x0000;
            n=1; i=1; x=0; b=0; p=0; e=0;
        }
        else{
            char* ptr = strstr(token, ",");         // check if X register is used
            if(!ptr) x=0;                           // do not use X register
            else {
                token = strtok(token, ",");         // use X register
                x=1;
            }
            if(token[0]=='#'){                      // immediate addressing
                n=0; i=1; e=0;
                for(k=1; token[k]; k++){            // remove the '#' character at the first
                    token[k-1] = token[k];
                }
                token[k-1] = '\0';
                for(sPoint=st; sPoint!=NULL; sPoint=sPoint->link){
                    if(!strcmp(sPoint->symbol, token)){ 
                        disp = sPoint->loc - loc;   // disp is the difference between the symbol's loc and PC's loc
                        break;
                    }
                }
                if(disp==-1) {                      // immediate constant
                    disp = strtol(token, NULL, 10);
                    b=0; p=0;
                }
                else{
                    if(disp>=-2048 && disp <= 2048) {           // PC relative
                        b=0; p=1;
                    }
                    else {b=1; p=0; disp = sPoint->loc - B;}    // Base relative
                }
            }
            else if(token[0]=='@'){                 //indirect addressing
                n=1; i=0; e=0;
                for(k=1; token[k]; k++){            // remove the '@' character at the first
                    token[k-1] = token[k];
                }       
                token[k-1] = '\0';
                for(sPoint=st; sPoint!=NULL; sPoint=sPoint->link){
                    if(!strcmp(sPoint->symbol, token)){ 
                    disp = sPoint->loc - loc;       // disp is the difference between the symbol's loc and PC's loc
                    break;
                    }
                }
                if(disp==-1)                        // use undifined symbol. error
                    {printf("Line %d Error. Use Undefined Symbol.\n", idx); return "ERROR";}
                if(disp>=-2048 && disp <= 2048) {               // PC relative
                    b=0; p=1;
                }
                else {b=1; p=0; disp = sPoint->loc - B;}        // Base relative
            }
            else {                                  // simple addressing
                n=1; i=1; e=0;
                for(sPoint=st; sPoint!=NULL; sPoint=sPoint->link){
                    if(!strcmp(sPoint->symbol, token)){ 
                        disp = sPoint->loc - loc;
                        break;
                    }
                }
                if(disp==-1)                        // use undefined symbol. error
                    {printf("Line %d Error. Use Undefined Symbol.\n", idx); return "ERROR";}
                if(disp>=-2048 && disp <= 2048) {               // PC relative
                    b=0; p=1;
                }
                else {b=1; p=0; disp = sPoint->loc - B;}        // Base relative
            }
        }

        objcode[0] = hex_to_str(opcode/16);     // save first opcode
        opcode %= 16;
        num = opcode/4; num *= 4;               // ignore last 2 bits of opcode
        tempNum = n*2 + i;                      // set rest opcode with n, i value
        num += tempNum;
        objcode[1] = hex_to_str(num);
        tempNum = x*8+b*4+p*2+e;                // set third position of opcode with x, b, p, e value
        objcode[2] = hex_to_str(tempNum);
        
        char dispArr[12];
        if(disp<0x0000 && b!=1){                // if disp is negative, change the value with 2's complement
            sprintf(dispArr, "%X", disp);
            dispArr[strlen(dispArr)]='\0';
            objcode[3] = dispArr[5];            // and save the disp at the rest of the object code
            objcode[4] = dispArr[6];
            objcode[5] = dispArr[7];
        }
        else{                                   // save the disp at the rest of the object code
            objcode[5] = hex_to_str(disp%16);
            disp /= 16;
            objcode[4] = hex_to_str(disp%16);
            disp /= 16;
            objcode[3] = hex_to_str(disp%16);
        }
    }
    if(format==4){                              // format 4
        loc += format; e = 1; b = 0; p = 0;
        token = strtok(NULL, " ");
        char* ptr = strstr(token, ", X");       // check if X register is used
        if(!ptr) x = 0;                         // do not use X register
        else {                                  // use X register
            token = strtok(NULL, ",");  
            x = 1;
        }
        if(token[0]=='#'){                      // immediate addressing
            n=0; i=1; e=1;
            for(k=1; token[k]; k++){            // remove '#' character at the first
                token[k-1] = token[k];
            }
            token[k-1] = '\0';
            for(sPoint=st; sPoint!=NULL; sPoint=sPoint->link){
                if(!strcmp(sPoint->symbol, token)){ 
                    disp = sPoint->loc - loc;   // disp is the difference between the symbol's loc and PC's loc
                    break;
                }
            }
            if(disp==-1) {                      // immediate constant
                disp = strtol(token, NULL, 10);
                b=0; p=0;
            }
        }
        else if(token[0]=='@'){                 //indirect addressing
            n=1; i=0; e=1;
            for(k=1; token[k]; k++){            // remove '@' character at the first
                token[k-1] = token[k];
            }
            token[k-1] = '\0';
            for(sPoint=st; sPoint!=NULL; sPoint=sPoint->link){
                if(!strcmp(sPoint->symbol, token)){ 
                disp = sPoint->loc - loc;       // disp is the difference between the symbol's loc and PC's loc
                break;
                }
            }
            if(disp==-1)                        // use undefined symbol. error
                {printf("Line %d Error. Use Undefined Symbol.\n", idx); return "ERROR";}
        }
        else{ n = 1; i = 1; b = 0; p = 0; e = 1; 
        for(sPoint=st; sPoint!=NULL; sPoint=sPoint->link){
            if(!strcmp(sPoint->symbol, token)){ 
                disp = sPoint->loc;             // disp is the difference between the symbol's loc and PC's loc
                chk=1;
                break;
            }
        }
        if(disp==-1)                            // use undefined symbol. error
            { printf("Line %d Error. Use Undefined Symbol\n", idx); return "ERROR";}
        
        }
        objcode[0] = hex_to_str(opcode/16);     // set first opcode
        opcode %= 16;
        num = opcode/4; num *= 4;               // ignore last 2 bits of opcode
        tempNum = n*2 + i;                      // set rest opcode with n, i value
        num += tempNum;
        objcode[1] = hex_to_str(num);
        tempNum = x*8+b*4+p*2+e;                // set third position of opcode with x, b, p, e value
        objcode[2] = hex_to_str(tempNum);
        objcode[3] = '0';
        chk = 0;
        objcode[7] = hex_to_str(disp%16);       // save the disp at the rest of object code
        disp /= 16;
        objcode[6] = hex_to_str(disp%16);
        disp /= 16;
        objcode[5] = hex_to_str(disp%16);
        disp /= 16;
        objcode[4] = hex_to_str(disp%16);
    }
    if(format==0){                              // constant or variable
        int t = 0; 
        if(!strcmp(token, "RESW") || !strcmp(token, "RESB"))    // variable is not generate object code
            {objcode = " "; varChk=1;}
        else if(!strcmp(token, "BYTE")){        // byte constant
            token = strtok(NULL, " ");
            if(token[0]=='C'){
                objcode = (char*)calloc(2*(strlen(token)-3), sizeof(char)); 
                k=2;
                while(token[k]!='\''){          // fill the object code with the ASCII value of the character
                    objcode[t] = hex_to_str(token[k]/16);
                    objcode[t+1] = hex_to_str(token[k]%16);
                    t+=2; k+=1;
                }
            }
            else{                               // 'X'
                objcode = (char*)calloc((strlen(token)-3), sizeof(char)); 
                t=2;
                while(token[t]!='\''){          // fill the object code with value
                    objcode[t-2] = token[t];
                    t+=1;
                }
            }
        }
        else{                                   // word constant
            objcode = (char*)calloc(6, sizeof(char)); 
            token = strtok(NULL, " ");
            int w = strtol(token, NULL, 10);
            for(t=5; t>=0; t--)                 // fill the word-size object code with the value 
                {objcode[t] = hex_to_str(w%16); w/=16;}
        }
    }

    if(strcmp(objcode," ") || varChk==1){       // case that has valid object code or variable
        strcpy(oTmp->objcode, objcode);
        oTmp->format = format;
        if(varChk) {strcpy(oTmp->objcode, "VARIABLE"); oTmp->format=0;}
        if(oHead->link==NULL){                  // save the information as linked list format
            oHead->link = oTmp;
        }
        else{
            for(oPoint=oHead->link; oPoint->link!=NULL; oPoint=oPoint->link);
            oPoint->link = oTmp;
        }
    }
    return objcode;
}

char hex_to_str(int num){       // Change the integer value into character
    if(num>=0 && num<=9) return '0' + num;
    else return 'A' + num - 10;
}

int regList(char R){            // Return the register number. SW register is checked in pass2
    if(R=='A') return 0;
    if(R=='X') return 1;
    if(R=='L') return 2;
    if(R=='B') return 3;
    if(R=='S') return 4;
    if(R=='T') return 5;
    if(R=='F') return 6;
    if(R=='P') return 8;    // PC register
    else return -1;
}
