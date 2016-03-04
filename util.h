#ifndef _UTIL_H_
#define _UTIL_H_


/* 打印token和他的符号值
 */
void printToken( TokenType, const char* );

/* 创建一个语法树语句节点
 */
TreeNode * newStmtNode(StmtKind);

/* 创建一个语法树算式节点
 */
TreeNode * newExpNode(ExpKind);

/* 复制字符串
 */
char * copyString( char * );

/* 打印语法树
 */
void printTree( TreeNode * );

#endif // _UTIL_H_
