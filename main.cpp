#include "globles.h"
#include "scan.h"
#include "parse.h"
#include "util.h"
#include "symbAnalyze.h"

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;
int Error = FALSE;

/* allocate and set tracing flags */
int EchoSource = FALSE; //Դ�������
int TraceScan = FALSE; //��������
int TraceParse = FALSE; //�﷨����syntax tree��
int TraceAnalyze = TRUE; //��ʶ����
int TraceCode = FALSE; //��Ŀ���������ע��

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
  ///
   syntaxTree = parse();
   fprintf(listing,"\nSyntax tree:\n");
   printTree(syntaxTree);

   if (! Error)
  { if (TraceAnalyze) fprintf(listing,"\nBuilding Symbol Table...\n");
    buildSymtab(syntaxTree);
    if (TraceAnalyze) fprintf(listing,"\nChecking Types...\n");
    typeCheck(syntaxTree);
    if (TraceAnalyze) fprintf(listing,"\nType Checking Finished\n");
  }

   fclose(source);

  return 0;
}
