#ifndef _SCAN_H_
#define _SCAN_H_

/* 终结符的最大符号数 */
#define MAXTOKENLEN 40

/* 终结符的符号串 */
extern char tokenString[MAXTOKENLEN+1];

/* 返回源文件中的下一个token
 */
TokenType getToken(void);

#endif
