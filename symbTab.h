#ifndef SYMBTAB_H_INCLUDED
#define SYMBTAB_H_INCLUDED

void codeTitle(void);

void st_insert( char * name, int lineno, int loc );

/* 是否在hash符号表中或返回当前出现的行号
 */
int st_lookup ( char * name );

/* 打印符号表
 */
void printSymTab(FILE * listing);

/* 最多声明的符号数 */
#define SIZE 211

/* 每一个符号的行链表节点
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

//符号链表节点
typedef struct BucketListRec
   { char * name;
     LineList lines;
     int memloc ; /* 出现的次序 */
     struct BucketListRec * next;
   } * BucketList;


#endif // SYMBTAB_H_INCLUDED
