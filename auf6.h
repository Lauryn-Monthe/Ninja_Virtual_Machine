#include <stdbool.h>

#define VERSION 8
#define HALT 0
#define PUSHC 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define MOD 6

#define RDINT 7
#define WRINT 8
#define RDCHR 9
#define WRCHR 10

#define PUSHG 11
#define POPG 12

#define ASF 13
#define RSF 14
#define PUSHL 15
#define POPL 16

#define EQ 17
#define NE 18
#define LT 19
#define LE 20
#define GT 21
#define GE 22

#define JMP 23
#define BRF 24
#define BRT 25

#define CALL 26
#define RET 27
#define DROP 28
#define PUSHR 29
#define POPR 30

#define DUP 31

#define NEW 32
#define GETF 33
#define PUTF 34

#define NEWA 35
#define GETFA 36
#define PUTFA 37

#define GETSZ 38

#define PUSHN 39
#define REFEQ 40
#define REFNE 41

#define IMMEDIATE(x) ((x) & 0x00FFFFFF) 
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) | 0xFF000000 : (i))
#define MSB (1 << (8 *sizeof(unsigned int) - 1))
#define IS_PRIM(objRef) (((objRef)->size & MSB) == 0)
#define GET_SIZE(objRef) ((objRef)->size & ~MSB)
#define GET_REFS(objRef) ((ObjRef *)(objRef)->data)

void listofprogramm(unsigned int ir);
void exec(unsigned int ir);
void debug();
void add();
void mul();
void sub();
void mod();
void divi();
void push(int number);
void popl(int n);
void pushl(int n);
void popg(int n);
void pushg(int n);
void asf(int n);
void rsf();
void brf(int n);
void brt(int n);
void eq();
void ne();
void lt();
void le();
void gt();
void ge();
void call(int target);
void ret();
void drop(int n);
void pushr();
void popr();
void dup();
void new(int n);
void getf(int n);
void putf(int n);
void newa();
void getfa();
void putfa();
void getsz();
void pushn();
void refeq();
void refne();
int pop();
ObjRef newCompoundObject(int numObjRefs);

typedef struct{
    bool isObjRef;
    union{
        ObjRef objRef; 
        int number;
    }u;
    } StackSlot;
ObjRef popObj();
void pushObj(ObjRef objet);
ObjRef trans(int i);