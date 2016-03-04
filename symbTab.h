#ifndef SYMBTAB_H_INCLUDED
#define SYMBTAB_H_INCLUDED

void codeTitle(void);

void st_insert( char * name, int lineno, int loc );

/* �Ƿ���hash���ű��л򷵻ص�ǰ���ֵ��к�
 */
int st_lookup ( char * name );

/* ��ӡ���ű�
 */
void printSymTab(FILE * listing);

/* ��������ķ����� */
#define SIZE 211

/* ÿһ�����ŵ�������ڵ�
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

//��������ڵ�
typedef struct BucketListRec
   { char * name;
     LineList lines;
     int memloc ; /* ���ֵĴ��� */
     struct BucketListRec * next;
   } * BucketList;


#endif // SYMBTAB_H_INCLUDED
