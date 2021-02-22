#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <bigint.h>
#include <support.h>
#include "auf6.h"
StackSlot stack [10000];
int sp = 0; 
int PC = 0;
int fp = 0;
int x = 0;
ObjRef rvr;
char y = 'a';
char *array_format;
unsigned int IR = 0;
unsigned int *arrayoftherest; 
unsigned int *registerr;
ObjRef *sda = NULL;
unsigned int sdaSize=0;
FILE *file = NULL;

int main(int argc, char *argv[]) {
    arrayoftherest = malloc(sizeof(int)*3);
    array_format = malloc(sizeof(char)*4);
    if(argc > 1){
        if(argc > 2){
            for(int i = 0; i<argc; i++){
                if((strcmp(argv[i],"--help") == 0)){
                    printf("Usage: ./njvm [options] <code file>\nOptions:\n  --version        show version and exit\n  --help           show this help and exit\n");
                    exit(0);
                } 
                else if((strcmp(argv[i],"--version") == 0)){
                   printf("Ninja Virtual Machine version %d (compiled Jun 12 2020, 10:22:55)\n", VERSION); 
                   exit(0);
                } 
                else if((strcmp (argv[i],"--debug") == 0)){
                    printf("DEBUG: file '%s' loaded (code size = %d, data size = %d)\n",argv[1], arrayoftherest[1], arrayoftherest[2]);
                    file = fopen(argv[1], "rb");
                    if(file==NULL){
                        printf("Error: cannot open code file '%s'\n", argv[1]);
                        exit(0);

                    }else{
                         debug(); fclose(file); exit(0);
                    }
                }
            }
        printf("Error: more than one code file specified\n");
        exit(0);
        }
        int test1 = strcmp(argv[1],"--help");
        int test2 = strcmp(argv[1],"--version");
        if(test1 == 0){
            printf("Usage: ./njvm [options] <code file>\nOptions:\n  --version        show version and exit\n  --help           show this help and exit\n");
            exit(0);
        }
        if(test2 == 0){
            printf("Ninja Virtual Machine version %d (compiled Jun 12 2020, 10:22:55)\n", VERSION); 
            exit(0);
        }
        file = fopen(argv[1], "rb");
        if(file == NULL) printf("Error: cannot open code file '%s'\n", argv[1]);
        if(file == NULL && strstr(argv[1],"bin") != NULL){
            printf("Error: cannot open code file '%s'\n", argv[1]);   
        }
        else{
            fread(array_format, sizeof(char), 4, file); 
            fread(arrayoftherest, sizeof(int),3, file); 
            registerr = malloc(sizeof(int)*arrayoftherest[1]);
            if(arrayoftherest[2] != 0){
                sdaSize=arrayoftherest[2];
                sda = malloc(sizeof(ObjRef)*arrayoftherest[2]);
                }
            fread(registerr, sizeof(int),arrayoftherest[1],file);
            if(strcmp(array_format, "NJBF") == 0 && arrayoftherest[0]== VERSION && argc == 2){
                printf("Ninja Virtual Machine started\n");
                PC = 0;
                do{
                    IR = registerr[PC];
                    PC = PC + 1;
                    exec(IR);
                }while(HALT != IR);
                printf("Ninja Virtual Machine stopped\n");
            }
            if(arrayoftherest[0] != VERSION){
                printf("Version ist falsch\n");
                exit(1);
            }
            else if(strcmp(array_format, "NJBF") != 0 && argc == 2){
                printf("Format ist falsch\n");
                exit(1);
            }
            fclose(file);
        }
        //if(strstr(argv[1],"bin") == NULL && test1!=0 && test2 != 0) printf("Error: cannot open code file '%s'\n", argv[1]);
    }
    else if (argc < 2) printf("Error: no code file specified\n");
    return 0;  
}
void listofprogramm(unsigned int ir){
    switch (ir>>24)
    {
    case PUSHC: printf("%.4d:\t pushc \t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case HALT: printf("%.4d:\t halt\n", PC);
    break;
    case ADD: printf("%.4d:\t add\n", PC);
    break;
    case SUB: printf("%.4d:\t sub\n", PC);
    break;
    case MUL: printf("%.4d:\t mul\n", PC);
    break;
    case DIV: printf("%.4d:\t div\n", PC);
    break;
    case WRCHR: printf("%.4d:\t wrchr\n", PC);
    break;
    case WRINT: printf("%.4d:\t wrint\n", PC);
    break;
    case RDINT: printf("%.4d:\t rdint\n", PC);
    break;
    case RDCHR: printf("%.4d:\t rdchr\n", PC);
    break;
    case POPG: printf("%.4d:\t popg \t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case PUSHG: printf("%.4d:\t pushg \t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case PUSHL: printf("%.4d:\t pushl \t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case POPL: printf("%.4d:\t popl \t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case ASF: printf("%.4d:\t asf \t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case RSF: printf("%.4d:\t rsf\n",PC);
    break;
    case MOD: printf("%.4d:\t mod\n", PC);
    break;
    case EQ: printf("%.4d:\t eq\n", PC);
    break;
    case NE: printf("%.4d:\t ne\n", PC);
    break;
    case LT: printf("%.4d:\t lt\n", PC);
    break;
    case LE: printf("%.4d:\t le\n", PC);
    break;
    case GT: printf("%.4d:\t gt\n", PC);
    break;
    case GE: printf("%.4d:\t ge\n", PC);
    break;
    case JMP: printf("%.4d:\t jmp \t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case BRF: printf("%.4d:\t brf\t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case BRT: printf("%.4d:\t brt\t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case CALL: printf("%.4d:\t call\t%d\n",PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case RET: printf("%.4d:\t ret\n", PC);
    break;
    case DROP: printf("%.4d:\t drop\t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case DUP: printf("%.4d:\t dup\n", PC);
    break;
    case PUSHR: printf("%.4d:\t pushr\n", PC);
    break;
    case POPR: printf("%.4d:\t popr\n", PC);
    break;
    case NEW: printf("%.4d:\t new\t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case GETF: printf("%.4d:\t getf\t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case PUTF: printf("%.4d:\t putf\t%d\n", PC, SIGN_EXTEND(IMMEDIATE(ir)));
    break;
    case NEWA: printf("%.4d:\t newa\n", PC);
    break;
    case PUTFA: printf("%.4d:\t putfa\n", PC);
    break;
    case GETFA: printf("%.4d:\t getfa\n", PC);
    break;
    case GETSZ: printf("%.4d:\t getsz\n", PC);
    break;
    case PUSHN: printf("%.4d:\t pushn\n", PC);
    break;
    case REFEQ: printf("%.4d:\t refeq\n", PC);
    break;
    case REFNE: printf("%.4d:\t refne\n", PC);
    break;
    default:
        break;
    }
}
void fatalError(char *msg) {
  printf("Fatal error: %s\n", msg);
  exit(1);
}
ObjRef newPrimObject(int dataSize) {
  ObjRef objRef;

  objRef = malloc(sizeof(unsigned int) +
                  dataSize * sizeof(unsigned char));
  if (objRef == NULL) {
    fatalError("newPrimObject() got no memory");
  }
  objRef->size = dataSize;
  return objRef;
}
ObjRef trans(int i){
    ObjRef objRef = malloc(sizeof(unsigned int) + sizeof(int));
    objRef-> size = sizeof(int);
    *(int*)objRef-> data  = i;
    return objRef;
}
void pushObj(ObjRef objet){
    if(sp > 10000) fatalError("Stack overflow");
    stack[sp].isObjRef = true;
    stack[sp].u.objRef = objet;
    sp +=1;
}
ObjRef popObj(){
    if(sp <= 0) fatalError("Stack underflow");
    ObjRef objet;
    if(stack[sp-1].isObjRef == true){
        objet = stack[sp-1].u.objRef;
        sp -=1;}
    else fatalError("Wert ist nicht ein Objet\n");
    return objet;
}
void push(int number){
    if(sp > 10000) fatalError("Stack overflow");
    stack[sp].isObjRef = false;
    stack[sp].u.number = number;
    sp += 1;
}
int pop(){
    if(sp <= 0) fatalError("Stack underflow");
    int num = 0;
    if(stack[sp-1].isObjRef == false){
        num = stack[sp-1].u.number;
        sp -=1;}
    else fatalError("Wert ist nicht ein Zahl\n");
    return num;
}
void add(){
    bip.op2 = popObj(); 
    bip.op1 = popObj();
    bigAdd();
    pushObj(bip.res);
}
void sub(){
    bip.op2 = popObj(); 
    bip.op1 = popObj();
    bigSub();
    pushObj(bip.res);
}
void mul(){
    bip.op2 = popObj(); 
    bip.op1 = popObj();
    bigMul();
    pushObj(bip.res);
}
void mod(){
    bip.op2 = popObj(); 
    bip.op1 = popObj();
    
    /*if(*(int*)bip.op2->data == 0){
        fatalError("Modulo with zero is undefined\n");
        exit(1);
    }*/
        bigDiv();
        pushObj(bip.rem);
}
void divi(){
    bip.op2 = popObj(); 
    bip.op1 = popObj();
    /*if(bip.op2->size == 0){
        fatalError("Division by zero is undefined\n");
        exit(1);
    }*/
        bigDiv();
        pushObj(bip.res);
}
void pushg(int n){
    pushObj(sda[n]);
}
void popg(int n){
    sda[n] = popObj();
}
void pushl(int n){
    pushObj(stack[n+fp].u.objRef);
}
void popl(int n){
    stack[fp+n].u.objRef = popObj();
}   
void asf(int n){
    push(fp);
    fp = sp;
    sp = sp + n;
}
void rsf(){
    sp = fp;
    fp = pop();
}
void eq(){
    bip.op1 = popObj(); 
    bip.op2 = popObj();
    int result = bigCmp();
    //printf("valeur %d\n", result);
    if(result == 0){
        bigFromInt(1);
        pushObj(bip.res);}
    else {
        bigFromInt(0);
        pushObj(bip.res);
    }
}
void ne(){
    bip.op1 = popObj(); 
    bip.op2 = popObj();
    int result = bigCmp();
    //printf("valeur %d\n", result);
    if(result != 0){ 
        bigFromInt(1);
        pushObj(bip.res);}
    else {
        bigFromInt(0);
        pushObj(bip.res);
    }
}

 //never op1 and op2 in compare mode, correct

void lt(){  
    bip.op2 = popObj();
    bip.op1 = popObj(); 
    int result = bigCmp();
    //printf("valeur %d\n", result);
    if(result < 0){ 
        bigFromInt(1);
        pushObj(bip.res);}
    else {
        bigFromInt(0);
        pushObj(bip.res);
    }
}
void le(){ 
    bip.op2 = popObj();
    bip.op1 = popObj();
    int result = bigCmp();
    //printf("valeur %d\n", result);
    if(result <= 0){ 
        bigFromInt(1);
        pushObj(bip.res);}
    else {
        bigFromInt(0);
        pushObj(bip.res);
    }
}
void gt(){
    bip.op2 = popObj();
    bip.op1 = popObj();  
    int result = bigCmp();
    //printf("valeur %d\n", result);
    if(result > 0){ 
        bigFromInt(1);
        pushObj(bip.res);}
    else {
        bigFromInt(0);
        pushObj(bip.res);
    }
}
void ge(){
    bip.op2 = popObj();
    bip.op1 = popObj();
    int result = bigCmp(); // was bigToInt() required was bigCmp 
    if(result >= 0){ 
        bigFromInt(1);
        pushObj(bip.res);}
    else {
        bigFromInt(0);
        pushObj(bip.res);
    }
}
void brf(int n){
    bip.op1 = popObj();
    int value = bigToInt() ;
    if(value == 0) PC = n;
}
void brt(int n){
    bip.op1 = popObj();
    int value = bigToInt() ;
    if(value==1) PC = n;  //  i changed !=0 to ===1
    //printf("value %d\n", value);
}
void call(int target){
    push(PC);
    PC = target;
}
void ret(){
    PC = pop();
}
void drop(int n){
    int i;
    for(i = 0; i < n; i++) popObj();
}
void pushr(){
    pushObj(rvr);
}
void popr(){
    rvr = popObj();
}
void dup(){
    pushObj(stack[sp-1].u.objRef);
}
ObjRef newCompoundObject(int numObjRefs){
    ObjRef o = malloc(sizeof(unsigned int)+(numObjRefs * sizeof(ObjRef)));
    o->size = numObjRefs | MSB;
    return o;
}
void new(int n){
    ObjRef o = newCompoundObject(n);
    pushObj(o);
}
void getf(int n){
    ObjRef o = popObj();    
    pushObj(GET_REFS(o)[n]);
}
void putf(int n){
    ObjRef o1 = popObj(); 
    ObjRef o2 = popObj();
    GET_REFS(o2)[n] = o1;
}
void newa(){
    bip.op1 = popObj();
    ObjRef o = newCompoundObject(bigToInt()); 
    pushObj(o);
}
void putfa(){
    ObjRef o1 = popObj(); 
    bip.op1 = popObj();
    ObjRef o2 = popObj();
    GET_REFS(o2)[bigToInt()] = o1;
}
void getfa(){
    bip.op1 = popObj();
    ObjRef o1 = popObj();
    pushObj(GET_REFS(o1)[bigToInt()]);
}
void getsz(){
    bigFromInt(GET_SIZE(popObj()));
    pushObj(bip.res);
}
void pushn(){
    pushObj(NULL);
}

// GET_REFS missed xD
// but fix now 

void refeq(){
    ObjRef o1 = popObj();
    ObjRef o2 = popObj();
    bigFromInt(GET_REFS(o1)==GET_REFS(o2));
    pushObj(bip.res);
}
void refne(){
    ObjRef o1 = popObj();
    ObjRef o2 = popObj();
    bigFromInt(GET_REFS(o1)!=GET_REFS(o2));
    pushObj(bip.res);
}
void exec(unsigned int i){
    switch (i >> 24)
    {
    case PUSHC: bigFromInt((SIGN_EXTEND(IMMEDIATE(i)))); pushObj(bip.res);          
    break;
    case ADD: add();
    break;
    case SUB: sub();
    break;
    case MUL: mul();
    break;
    case DIV: divi();
    break;
    case MOD: mod();
    break;
    case WRINT: bip.op1 = popObj(); bigPrint(stdout); //printf("%d",*(int *)popObj()->data); 
    break;
    case WRCHR: bip.op1 = popObj(); int value = bigToInt(); printf("%c", value);
    break;
    case RDINT: bigRead(stdin); pushObj(bip.res);   //scanf("%d", &x); pushObj(trans(x));                                    
    break;
    case RDCHR: scanf("%c",&y); bigFromInt(y); pushObj(bip.res); 
    break;
    case PUSHL: pushl(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case POPL: popl(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case PUSHG:pushg(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case POPG:popg(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case ASF:asf(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case RSF:rsf();
    break;
    case EQ: eq();
    break;
    case NE: ne();
    break;
    case LT: lt();
    break;
    case GT: gt();
    break;
    case GE: ge();
    break;
    case LE: le();
    break;
    case JMP: PC = SIGN_EXTEND(IMMEDIATE(i));
    break;
    case BRF: brf(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case BRT: brt(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case CALL: call(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case RET: ret();
    break;
    case DROP: drop(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case PUSHR: pushr();
    break;
    case POPR: popr();
    break;
    case DUP: dup();
    break;
    case NEW: new(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case GETF: getf(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case PUTF: putf(SIGN_EXTEND(IMMEDIATE(i)));
    break;
    case NEWA: newa();
    break;
    case PUTFA: putfa();
    break;
    case GETFA: getfa();
    break;
    case GETSZ: getsz();
    break;
    case PUSHN: pushn();
    break;
    case REFEQ: refeq();
    break;
    case REFNE: refne();
    break;
    default:
        break;
    }
}
void debug(){
    if(fread(array_format, sizeof(char), 4, file)!=4){
        printf("Error bei array_format\n");
        exit(0);
    }
    if(fread(arrayoftherest, sizeof(int), 3, file)!=3){
        printf("Error bei arrayof the rest");
        exit(0);
    }
    printf("J\n");
    registerr = malloc(sizeof(int)*arrayoftherest[1]);
    if(arrayoftherest[2] != 0){
        sdaSize=arrayoftherest[2];
        sda = malloc(sizeof(ObjRef)*arrayoftherest[2]);
    }
    fread(registerr, sizeof(int),arrayoftherest[1],file);
    char choice;
    char choice1;
    int choice2;
    //char o;
    unsigned int breakpoint = HALT;
    printf("Ninja Virtual Machine started\n");
    listofprogramm(registerr[0]);
    while(choice != 'q'){
        printf("DEBUG: inspect, list, breakpoint, step, run, quit?\n");
        scanf("%c", &choice); 
        switch (choice){
            case 'l':
            PC = 0;
            do{
                IR = registerr[PC];
                listofprogramm(IR);
                PC = PC + 1;
            }while(PC < arrayoftherest[1]);
            printf("\t---end of code---\n");
            PC = 0;
            break;
            case'r':
            PC = 0;
            if(breakpoint == HALT){
                do{
                    IR = registerr[PC];
                    PC = PC + 1;
                    exec(IR);
                }while(breakpoint != IR);
            printf("Ninja Virtual Machine stopped\n");
            exit(0);}
            else{
              do{
                IR = registerr[PC];
                PC = PC + 1;
                exec(IR);
            }while(breakpoint != IR);
            listofprogramm(IR);}
            break;
            case 'i':
            printf("DEBUG [inspect]: stack, data, object?\n");
            scanf("%c",&choice1);
            scanf("%c",&choice1);
            if(choice1 == 'd'){
                for(int i = 0; i < sdaSize; i++){
                    printf("data[%.4d]:\t%s\n", i, sda[i]->data);
                }
                printf("\t\t--- end of data ---\n");
            }
            else if(choice1 == 's'){
                if(sp == 0) printf("sp, fp\t--->\t%.4d:\txxxx\n--- bottom of stack ---\n",sp);
                else{
                    printf("sp\t--->\t%.4d:\txxxx\n",sp);
                    for(int i = sp-1; i > fp; i--){
                        if(stack[i].isObjRef) printf("  \t    \t%.4d:\t(objref) %p\n",i,stack[i].u.objRef->data);
                        else printf("  \t    \t%.4d:\t(number) %d\n",i,stack[i].u.number);
                    printf("value = %d\n",*(int*)stack[sp-1].u.objRef->data);
                    }
                    if(stack[fp].isObjRef) printf("fp\t--->\t%.4d:\t(objref) %p\n",fp,stack[fp].u.objRef->data);
                    else printf("fp\t--->\t%.4d:\t(number) %d\n",fp, stack[fp].u.number);
                    for(int i = fp-1; i >= 0; i--){
                        if(stack[i].isObjRef) printf("  \t    \t%.4d:\t(objref) %p\n",i,stack[i].u.objRef->data);
                        else printf("  \t    \t%.4d:\t(number) %d\n",i, stack[i].u.number);
                    }
                    printf("\t\t---bottom of stack---\n");
                }
            }
            /*else if (choice1 == 'o'){
                scanf("%c", &o);
                scanf("%c", &o);
                for(int i = 0; i < sp; i++){
                    if(strcmp (stack[i].u.objRef->data, o)==0)
                    printf("value = %d\n",*(int*)stack[sp-1].u.objRef->data);
                }
            }
            break;*/
            case 's':
            do{
                IR = registerr[PC];
                PC = PC + 1;
                exec(IR);
                IR = registerr[PC];
                listofprogramm(IR);
                break;
            }while(HALT != IR);
            if(IR == HALT) {exec(IR); exit(0);}
            break;
            case 'b':
            printf("DEBUG [breakpoint]: cleared\nDEBUG [breakpoint]: address to set, -1 to clear, <ret> for no change?\n");
            scanf("%d",&choice2);
            if(choice2 != -1){
                breakpoint = registerr[choice2-1];
                printf("now set at %d\n", choice2);}
            else if (choice2 == -1){
                breakpoint = HALT;
                printf("now cleared\n");
            }
            break;
        }
    }
}
