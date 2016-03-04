#include "globles.h"
#include "scan.h"
#include "util.h"

/* �Զ����ĸ���״̬ */
typedef enum
   { START,INASSIGN,INCOMMENT,INNUM,INID,DONE }
   StateType;

/* ID�����ֵĸ��Ŵ�ֵ */
char tokenString[MAXTOKENLEN+1];

/*Դ�ļ�ÿ���������� */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* ��ǰ��ȡ�ķ��Ŵ� */
static int linepos = 0; /* �ʷ�����ʱ�ڷ��Ŵ��е�ʵʱλ�� */
static int bufsize = 0; /* ʵ��ÿ�ζ�ȡ�ķ����� */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* getNextChar �ӵ�ǰlineBuf�ж�ȡ��һ���ǿ��ַ�����\n��ȡ��һ�� */
static int getNextChar(void)
{ if (!(linepos < bufsize))
  { lineno++;
    if (fgets(lineBuf,BUFLEN-1,source))
    {
      bufsize = strlen(lineBuf);
      linepos = 0;
      return lineBuf[linepos++];
    }
    else
    { EOF_flag = TRUE; //�����ļ��������ٿ��Զ�������
      return EOF;
    }
  }
  else return lineBuf[linepos++];
}

/* ����һ������ */
static void ungetNextChar(void)
{ if (!EOF_flag) linepos-- ;}

/* �����ֽṹ */
static struct
    { char* str;
      TokenType tok;
    } reservedWords[MAXRESERVED]
   = {{"if",IF},{"else",ELSE},{"for",FOR},
      {"while",WHILE},{"int",INT},{"char",CHAR}};  //����������

/* �鿴ID�Ƿ�Ϊ������*/
static TokenType reservedLookup (char * s)
{ int i;
  for (i=0;i<MAXRESERVED;i++)
    if (!strcmp(s,reservedWords[i].str))
      return reservedWords[i].tok;
  return ID;
}

/****************************************/
/* �ʷ������Ļ�������  */
/****************************************/
/* ������һ���ս����token��
 */
TokenType getToken(void)
{  /* index for storing into tokenString */
   int tokenStringIndex = 0;
   /* ���ڷ��صĵ�ǰtoken */
   TokenType currentToken;
   /* �Զ�������ʱ�ĵ�ǰ״̬*/
   StateType state = START;
   /* �Ƿ���token�����ڽ����﷨��(,),{,},;������ */
   int save;
   while (state != DONE)
   { int c = getNextChar();
    char testc =c;
     save = TRUE;
     switch (state)
     { case START:
         if (isdigit(c))
           state = INNUM; //�������ȡ��������
         else if (isalpha(c))
           state = INID; //�������ȡ�����ַ�
         else if (c == '=')//��ֵ
          { state = INASSIGN;
            currentToken = ASSIGN;
          }
         else if ((c == ' ') || (c == '\t') || (c == '\n')) //���Կ��ַ�
           save = FALSE;
         else
         { state = DONE; //��ID��NUM����ֵ������
           switch (c)
           { case EOF:
               save = FALSE;
               currentToken = ENDFILE;
               break;
             case '<':
               currentToken = LT;
               break;
             case '+':
               currentToken = PLUS;
               break;
             case '-':
               currentToken = MINUS;
               break;
             case '*':
               currentToken = MUL;
               break;
             case '/':
               currentToken = DIV;
               break;
             case '(':
               currentToken = LPAREN;
               break;
             case ')':
               currentToken = RPAREN;
               break;
             case ';':
               currentToken = SEMI;
               break;
			 case '{':
               currentToken = LBRACE;
               break;
			 case '}':
				currentToken = RBRACE;
				break;
             default:
               currentToken = ERROR; //����ʶ��token
               break;
           }
         }
         break;
       case INASSIGN:
       	state = DONE;
         if (c == '='){
			currentToken = EQ;
         }else{
         	save = FALSE;
         	ungetNextChar();} //�ǵ��ڣ�����һ���ַ�
         break;
       case INNUM:
         if (!isdigit(c))
         { /* ����ʶ�����������һ���ַ� */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = NUM;
         }
         break;
       case INID:
         if (!isalpha(c))
         { /* IDʶ�����������һ���ַ� */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = ID;
         }
         break;
       case DONE:
       default: /* DNF���г��� */
         fprintf(listing,"Scanner Bug: state= %d\n",state);
         state = DONE;
         currentToken = ERROR;
         break;
     }  //�ʷ�����
     if ((save) && (tokenStringIndex <= MAXTOKENLEN))
       tokenString[tokenStringIndex++] = (char) c;
     if (state == DONE)
     { tokenString[tokenStringIndex] = '\0';
       if (currentToken == ID)
         currentToken = reservedLookup(tokenString);
     }
   }
   if (TraceScan) {
     fprintf(listing,"\t%d: ",lineno);
     printToken(currentToken,tokenString);
   }
   return currentToken;
} /* end getToken */

