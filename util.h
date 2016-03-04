#ifndef _UTIL_H_
#define _UTIL_H_


/* ��ӡtoken�����ķ���ֵ
 */
void printToken( TokenType, const char* );

/* ����һ���﷨�����ڵ�
 */
TreeNode * newStmtNode(StmtKind);

/* ����һ���﷨����ʽ�ڵ�
 */
TreeNode * newExpNode(ExpKind);

/* �����ַ���
 */
char * copyString( char * );

/* ��ӡ�﷨��
 */
void printTree( TreeNode * );

#endif // _UTIL_H_
