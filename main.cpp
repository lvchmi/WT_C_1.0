#include "globles.h"
#include "scan.h"
#include "parse.h"
#include "util.h"
#include "symbAnalyze.h"
#include "asmCode.h"


int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;
int Error = FALSE;

/* ������Ʊ�ʶ */
int TraceScan = FALSE; //��������
int TraceParse = TRUE; //�﷨����syntax tree��
int TraceAnalyze = FALSE; //��ʶ����

int main( int argc, char * argv[] )
{
  TreeNode * syntaxTree;
  char pgm[120];
  if (argc != 2)
    { fprintf(stderr,"usage: %s <filename>\n",argv[0]);
      exit(1);
    }
  strcpy(pgm,argv[1]) ;
  source = fopen(pgm,"r");
  if (source==NULL)
  { fprintf(stderr,"File %s not found\n",pgm);
    exit(1);
  }
  listing = stdout;
  fprintf(listing,"\nTINY COMPILATION: %s\n",pgm);

   syntaxTree = parse(); //�﷨����
   if(TraceParse)
   {
   	fprintf(listing,"\nSyntax tree:\n");
    printTree(syntaxTree);
   }

   if (! Error) //��������ӡ���ű�
  { if (TraceAnalyze) fprintf(listing,"\nBuilding Symbol Table...\n");
    buildSymtab(syntaxTree);
    if (TraceAnalyze) fprintf(listing,"\nChecking Types...\n");
    typeCheck(syntaxTree);
    if (TraceAnalyze) fprintf(listing,"\nType Checking Finished\n");
  }
 if (! Error) //���ɻ�����
  { char * codefile;
    int fnlen = strcspn(pgm,".");
    codefile = (char *) calloc(fnlen+5, sizeof(char));
    strncpy(codefile,pgm,fnlen);
    strcat(codefile,".asm");
    code = fopen(codefile,"w");
    if (code == NULL)
    { printf("Unable to open %s\n",codefile);
      exit(1);
    }
    codePrin(syntaxTree);
    fclose(code);
  }
   fclose(source);

  return 0;
}
