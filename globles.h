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

/* �����ֵ����ֵ */
#define MAXRESERVED 6

typedef enum

   {ENDFILE,ERROR,
    /* ������ */
    IF,ELSE,FOR,WHILE,INT,CHAR,
    /* ��ʶ�������� */
    ID,NUM,
    /* �����ʶ�� */
    ASSIGN,EQ,LT,PLUS,MINUS,MUL,DIV,LPAREN,RPAREN,LBRACE,RBRACE,SEMI
   } TokenType;

extern FILE* source;
extern FILE* listing;
extern FILE* code;

extern int lineno; /* �˷����������е�Դ�ļ��к� */

/**************************************************/
/***********   �����﷨��Ҫ�õ��Ľڵ㽨�� ************/
/**************************************************/

typedef enum {StmtK,ExpK} NodeKind; //�ڵ����ͣ������������ʽ
typedef enum {IfK,ForK,WhileK,AssignK,Vark} StmtKind; //�������
typedef enum {OpK,ConstK,IdK} ExpKind; //�������ʽ����

/* �������� */
typedef enum {Char,Integer,Boolean} ExpType;

#define MAXCHILDREN 4 //�ڵ������ӽڵ���

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling; //��һ�����
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp;} kind;
     union { TokenType op;
             int val;
             char * name; } attr;
     ExpType type; /* �������ʽֵ���� */
   } TreeNode;

extern int Error;

extern int TraceScan; //��������
extern int TraceParse; //�﷨����syntax tree��
extern int TraceAnalyze; //��ʶ����

#endif // _GLOBALS_H_
