#ifndef _SCAN_H_
#define _SCAN_H_

/* �ս������������ */
#define MAXTOKENLEN 40

/* �ս���ķ��Ŵ� */
extern char tokenString[MAXTOKENLEN+1];

/* ����Դ�ļ��е���һ��token
 */
TokenType getToken(void);

#endif
