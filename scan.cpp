#include "globles.h"
#include "scan.h"
#include "util.h"

/* 自动机的各个状态 */
typedef enum
   { START,INASSIGN,INCOMMENT,INNUM,INID,DONE }
   StateType;

/* ID或保留字的负号串值 */
char tokenString[MAXTOKENLEN+1];

/*源文件每行最大符号数 */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* 当前读取的符号串 */
static int linepos = 0; /* 词法分析时在符号串中的实时位置 */
static int bufsize = 0; /* 实际每次读取的符号数 */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* getNextChar 从当前lineBuf中读取下一个非空字符读到\n读取下一行 */
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
    { EOF_flag = TRUE; //读到文件结束不再可以读到符号
      return EOF;
    }
  }
  else return lineBuf[linepos++];
}

/* 后退一个符号 */
static void ungetNextChar(void)
{ if (!EOF_flag) linepos-- ;}

/* 保留字结构 */
static struct
    { char* str;
      TokenType tok;
    } reservedWords[MAXRESERVED]
   = {{"if",IF},{"else",ELSE},{"for",FOR},
      {"while",WHILE},{"int",INT},{"char",CHAR}};  //保留字声明

/* 查看ID是否为保留字*/
static TokenType reservedLookup (char * s)
{ int i;
  for (i=0;i<MAXRESERVED;i++)
    if (!strcmp(s,reservedWords[i].str))
      return reservedWords[i].tok;
  return ID;
}

/****************************************/
/* 词法分析的基本函数  */
/****************************************/
/* 返回下一个终结符（token）
 */
TokenType getToken(void)
{  /* index for storing into tokenString */
   int tokenStringIndex = 0;
   /* 用于返回的当前token */
   TokenType currentToken;
   /* 自动机分析时的当前状态*/
   StateType state = START;
   /* 是否保留token已用于建立语法树(,),{,},;不保留 */
   int save;
   while (state != DONE)
   { int c = getNextChar();
    char testc =c;
     save = TRUE;
     switch (state)
     { case START:
         if (isdigit(c))
           state = INNUM; //需继续读取所有数字
         else if (isalpha(c))
           state = INID; //需继续读取所有字符
         else if (c == '=')//赋值
          { state = INASSIGN;
            currentToken = ASSIGN;
          }
         else if ((c == ' ') || (c == '\t') || (c == '\n')) //忽略空字符
           save = FALSE;
         else
         { state = DONE; //非ID，NUM，赋值，等于
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
               currentToken = ERROR; //不可识别token
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
         	ungetNextChar();} //非等于，后退一个字符
         break;
       case INNUM:
         if (!isdigit(c))
         { /* 数字识别结束，后退一个字符 */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = NUM;
         }
         break;
       case INID:
         if (!isalpha(c))
         { /* ID识别结束，后退一个字符 */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = ID;
         }
         break;
       case DONE:
       default: /* DNF运行出错 */
         fprintf(listing,"Scanner Bug: state= %d\n",state);
         state = DONE;
         currentToken = ERROR;
         break;
     }  //词法分析
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

