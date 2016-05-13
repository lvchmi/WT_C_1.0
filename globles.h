#ifndef _GLOBALS_H_
#define _GLOBALS_H_


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* 保留字的最大值 */
#define MAXRESERVED 6

typedef enum

   {ENDFILE,ERROR,
    /* 保留字 */
    IF,ELSE,FOR,WHILE,INT,CHAR,
    /* 标识符和数字 */
    ID,NUM,
    /* 特殊标识符 */
    ASSIGN,EQ,LT,PLUS,MINUS,MUL,DIV,LPAREN,RPAREN,LBRACE,RBRACE,SEMI
   } TokenType;

extern FILE* source;
extern FILE* listing;
extern FILE* code;

extern int lineno; /* 此法分析过程中的源文件行号 */

/**************************************************/
/***********   建立语法树要用到的节点建立 ************/
/**************************************************/

typedef enum {StmtK,ExpK} NodeKind; //节点类型，语句或算术表达式
typedef enum {IfK,ForK,WhileK,AssignK,Vark} StmtKind; //语句类型
typedef enum {OpK,ConstK,IdK} ExpKind; //算数表达式类型

/* 变量类型 */
typedef enum {Char,Integer,Boolean} ExpType;

#define MAXCHILDREN 4 //节点的最大子节点数

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling; //下一条语句
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp;} kind;
     union { TokenType op;
             int val;
             char * name; } attr;
     ExpType type; /* 算数表达式值类型 */
   } TreeNode;

extern int Error;

extern int TraceScan; //分析过程
extern int TraceParse; //语法树（syntax tree）
extern int TraceAnalyze; //标识符表

#endif // _GLOBALS_H_
