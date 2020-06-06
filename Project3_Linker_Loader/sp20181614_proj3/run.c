#include "20181614.h"

int findFormat(int opcode);             // function for finding format with opcode
void format2(int opcode, int target);                       // if format == 2
void format3(int opcode, int option, int xbpe, int target); // if format == 3

int end_flag;

int rFunc(){            // Disassemble and run the program
    int opcode, option, target, xbpe;
    int row, col;

    if(end_flag==1) { L = end; PC = exec_addr; end_flag=0;}

    while(PC<=end+prog_addr && PC>=exec_addr){
        row = PC/16; col = PC%16;           // get address of virtual memory
        opcode = dumpMem[row][col] & 0xFC;  // get opcode with bit & operation
        option = dumpMem[row][col] & 0x03;  // get n, i value with bit & operation

        switch(findFormat(opcode)){
            case 1:{                        // format 1
                       PC += 0x0001;
                       break;
                   }
            case 2:{                        // format 2
                       PC += 0x0002;
                       target = ((col+1)==16)? dumpMem[row+1][0]: dumpMem[row][col+1];
                       format2(opcode, target); 
                       break;
                   }
            case 3:{                        // format 3
                       PC += 0x0003;
                       xbpe = *(*(dumpMem+row)+col+1) / 16; // get x, b, p, e value
                       target = (*(*(dumpMem+row)+col+1) & 0x0F) * 256; // get rightmost half byte of disp
                       target += *(*(dumpMem+row)+col+2);   // get rest of disp
                       if(xbpe%2!=1)
                           format3(opcode, option, xbpe, target);
                       else{               // format 4
                           PC += 0x0001;
                           target *= 256;
                           target += (*(*(dumpMem+row)+col+3)); // get additional 1 half byte of disp
                           format3(opcode, option, xbpe, target);
                       }
                       break;
                   }
            default:{                       // constant
                        PC += 0x0003;
                    }
        }

        BLIST* bP;
        for(bP=bHead->link; bP!=NULL; bP=bP->link){             // check if PC is at break point
            if(bP->bpoint==PC){                                 // if so,
                printf("A : %06X\tX : %06X\n", A, X);           // print now register value
                printf("L : %06X\tPC : %06X\n", L, PC);
                printf("B : %06X\tS : %06X\n", B, S);
                printf("T : %06X\n", T);
                printf("\t\t\tStop at checkpoint[%X]\n", PC);
                return 0;                                       // and quit run
            }   
        }
        if(PC==exec_addr) break;
    }

    end_flag = 1;
    PC = end;                                                   // set PC addr as end of the program
    printf("A : %06X\tX : %06X\n", A, X);                       // and print register value
    printf("L : %06X\tPC : %06X\n", L, PC);
    printf("B : %06X\tS : %06X\n", B, S);
    printf("T : %06X\n", T);
    printf("\t\t\tEnd Program\n");

    return 0;
}

int findFormat(int opcode){     // function for finding format with opcode
    HTABLE* hPoint; 
    for(int i=0; i<20; i++){
        for(hPoint = op[i]; hPoint!=NULL; hPoint=hPoint->link){
            if(hPoint->opcode == opcode)
                return hPoint->format;
        }
    }
    return 0;
}

int* returnReg(int n){          // function for find register address with register number
    if(n==0) return &A;
    if(n==1) return &X;
    if(n==2) return &L;
    if(n==3) return &B;
    if(n==4) return &S;
    if(n==5) return &T;
    if(n==6) return &F;
    else return NULL;
}

void format2(int opcode, int target){       // function for format2
    int *R1;
    switch(opcode){
        case 0xB4:{     // CLEAR
                      R1 = returnReg(target/16);
                      *R1 = 0;
                      break;
                  }
        case 0xA0:{     // COMPR
                      break;
                  }
        case 0xB8:{     //TIXR
                      R1 = returnReg(target/16);
                      X += 1;
                      if(X==(*R1)) CC = 0;
                      else CC = (X<(*R1))? -1 : 1;
                      break;
                  }
    }
}

void format3(int opcode, int option, int xbpe, int disp){   // function for format3
    int n, i, x, b, p;
    n = option/2; i = option%2; // get n, i, x, b, p
    x = xbpe/8; xbpe%=8; 
    b = xbpe/4; xbpe%=4; 
    p = xbpe/2; xbpe%=2;

    if(disp&0xF00) { disp = 0xFFF - disp + 1; disp *= -1; }
    if(p==1) disp += PC;        // PC relative
    if(b==1) disp += B;         // BASE relative
    if(x==1) disp += X;         // Indexing

    int target = 0;
    int row, col;
    if(n==0 && i==1){           // immediate addressing
        target = disp;
    }
    else if(n==1 && i==0){      // indirect addressing
        row = disp/16; col = disp%16;
        target = (*(*(dumpMem+row)+col))*256*256 + *(*(dumpMem+row)+col+1)*256 + *(*(dumpMem+row)+col+2);
        row = target/16; col = target%16;
        target = (*(*(dumpMem+row)+col))*256*256 + *(*(dumpMem+row)+col+1)*256 + *(*(dumpMem+row)+col+2);
        disp = target;
    }
    else{                   // simple addressing
        row = disp/16; col = disp%16;
        target = (*(*(dumpMem+row)+col))*256*256 + *(*(dumpMem+row)+col+1)*256 + *(*(dumpMem+row)+col+2);
    }

    switch(opcode){
        case 0x00:{     // LDA
                      A = target;
                      break;
                  }
        case 0x10:{     // STX
                      row = disp/16; col = disp%16;
                      *(*(dumpMem+row)+col) = (X&0xFF0000)/(256*256);
                      *(*(dumpMem+row)+col+1) = (X&0x00FF00)/256;
                      *(*(dumpMem+row)+col+2) = X&0x0000FF;;
                      break;
                  }
        case 0x0C:{     // STA
                      row = disp/16; col = disp%16;
                      *(*(dumpMem+row)+col) = (A&0xFF0000)/(256*256);
                      *(*(dumpMem+row)+col+1) = (A&0x00FF00)/256;
                      *(*(dumpMem+row)+col+2) = A&0x0000FF;;
                      break;
                  }
        case 0x14:{     // STL
                      row = disp/16; col = disp%16;
                      *(*(dumpMem+row)+col) = (L&0xFF0000)/(256*256);
                      *(*(dumpMem+row)+col+1) = (L&0x00FF00)/256;
                      *(*(dumpMem+row)+col+2) = L&0x0000FF;;
                      break;
                  }
        case 0x28:{     // COMP
                      if(A==target) CC = 0;
                      else CC = (A<target)? -1 : 1;
                      break;
                  }
        case 0x30:{     // JEQ
                      if(CC==0) 
                          PC = disp;
                      break;
                  }
        case 0x38:{     // JLT
                      if(CC==-1) 
                          PC = disp;
                      break;
                  }
        case 0x3C:{     // J
                      PC = disp;
                      break;
                  }
        case 0x48:{     // JSUB
                      L = PC; PC = disp;
                      break;
                  }
        case 0x50:{     // LDCH
                      row = disp/16; col = disp%16;
                      A &= 0xFFFF00;
                      A += (*(*(dumpMem+row)+col))&0x00FF;
                      break;
                  }
        case 0x4C:{     // RSUB
                      PC = L;
                      break;
                  }
        case 0x54:{     // STCH
                      row = disp/16; col = disp%16;
                      dumpMem[row][col] = (A&0x00FF);
                      break;
                  }
        case 0x68:{     // LDB
                      B = target;
                      break;
                  }
        case 0x74:{     // LDT
                      T = target;
                      break;
                  }
        case 0xE0:{     //TD
                      CC = -1;
                      break;
                  }
        case 0xD8:{     // RD
                      CC = 0;
                      break;
                  }
        case 0xDC:{     // WD
                      break;
                  }
    }
}


int bFunc(char* str){           // Set break point of the program
    BLIST* bP;
    if(str==NULL){              // print break point list
        printf("breakpoint\n");
        printf("----------\n");
        for(bP=bHead->link; bP!=NULL; bP=bP->link)
            printf("%04X\n", bP->bpoint);
        printf("----------\n");
        return 0;
    }

    if(!strcmp(str, "clear")){  // clear bp list
        bHead->link = NULL;
        printf("\t[ok] clear all breakpoints\n");
    } 
    else{                       // add bp at the list
        BLIST* bNode = (BLIST*)malloc(sizeof(BLIST));
        bNode->bpoint = strtol(str, NULL, 16);
        bNode->link = NULL;

        if(bHead->link==NULL) bHead->link = bNode;      // first bp
        else{                                           // add new bp at list
            for(bP=bHead->link; bP->link!=NULL; bP=bP->link);
            bP->link = bNode;
        }
        printf("\t[ok] create breakpoint %X\n", bNode->bpoint);

    }

    return 0;
}
