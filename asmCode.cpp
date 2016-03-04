#include "globles.h"
#include "symbTab.h"
#include "asmCode.h"

static int tmpOffset = 0;
static int jLabel = 0; //��ת��ʶ

/* prototype for internal recursive code generator */
static void codeGen (TreeNode * tree);

static void genStmt(TreeNode * tree)
{
	TreeNode * p1, * p2, * p3, * p4;
	int label1, label2;
	label1 = jLabel++; label2 = jLabel++;
	switch(tree->kind.stmt)
	{
	case IfK:
		p1 = tree->child[0] ;
		p2 = tree->child[1] ;
		p3 = tree->child[2] ;
		codeGen(p1);
		switch(p1->attr.op)
		{
		case LT:
			fprintf(code, "POP EBX\n");
			fprintf(code, "POP EAX\n");
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JG label%d\n", label1);
			break;
		case EQ:
			fprintf(code, "POP EBX\n");
			fprintf(code, "POP EAX\n");
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JZ label%d\n", label1);
		}
		codeGen(p2);
		switch(p1->attr.op)
		{
		case LT:
			fprintf(code, "J label%d\n", label2);
			break;
		case EQ:
			fprintf(code, "J label%d\n", label2);
		}
		fprintf(code, "label%d:\n", label1);
		codeGen(p3);
		fprintf(code, "label%d:\n", label2);
		break;
	case WhileK:
		p1 = tree->child[0] ;
		p2 = tree->child[1] ;
		codeGen(p1);
		switch(p1->attr.op)
		{
		case LT:
			fprintf(code, "POP EBX\n");
			fprintf(code, "POP EAX\n");
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JG label%d\n", label1);
			break;
		case EQ:
			fprintf(code, "POP EBX\n");
			fprintf(code, "POP EAX\n");
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JZ label%d\n", label1);
		}
		codeGen(p2);
		fprintf(code, "label%d:\n", label1);
		break;
	case ForK:
		p1 = tree->child[0];
		p2 = tree->child[1];
		p3 = tree->child[2];
		p4 = tree->child[3];
		codeGen(p1);
		fprintf(code, "label%d:\n", label1);
		codeGen(p2);
		switch(p1->attr.op)
		{
		case LT:
			fprintf(code, "POP EBX\n");
			fprintf(code, "POP EAX\n");
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JG label%d\n", label2);
			break;
		case EQ:
			fprintf(code, "POP EBX\n");
			fprintf(code, "POP EAX\n");
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JZ label%d\n", label2);
		}
		codeGen(p4);
		codeGen(p3);
		fprintf(code, "J label%d\n", label1);
		fprintf(code, "label%d:\n", label2);
		break;
	case AssignK:
		p1 = tree->child[0];
		codeGen(p1);
		fprintf(code, "POP EAX\n"); //��ֵ���ʽ�ұߵ�ֵ��Ҫ��ջ
		fprintf(code, "MOV %s, EAX\n", tree->attr.name);
		break;
	case Vark:
		break;
	}
}

static void genExp(TreeNode * tree)
{
	TreeNode * p1, * p2;
	switch(tree->kind.exp)
	{
	case OpK:
		p1 = tree->child[0];
		p2 = tree->child[1];
		codeGen(p1);
		codeGen(p2);
		fprintf(code, "POP EAX\n"); //������2
		fprintf(code, "POP EBX\n"); //������1
		switch(tree->attr.op)
		{
		case PLUS:
			fprintf(code, "ADD EBX, EAX\n");
			fprintf(code, "PUSH EBX\n");
			break;
		case MINUS:
			fprintf(code, "SUB EBX, EAX\n");
			fprintf(code, "PUSH EBX\n");
			break;
		case MUL:
			fprintf(code, "MUL EBX\n");
			fprintf(code, "PUSH EAX\n");
			break;
		}
	case ConstK:
		fprintf(code, "MOV EAX %d\n", tree->attr.val);
		fprintf(code, "PUSH EAX\n");
		break;
	case IdK:
		if(st_lookup(tree->attr.name) != -1)
		{
			fprintf(code, "MOV EAX, %s\n", tree->attr.name);
			fprintf(code, "PUSH EAX\n");
		}else{fprintf(code, ">>>Error����%sδ����\n", tree->attr.name);} //������
		break;
	}
}

static void codeGen(TreeNode * tree)
{
	if (tree != NULL)
	{ switch (tree->nodekind) {
			case StmtK:
				genStmt(tree);
				break;
			case ExpK:
				genExp(tree);
				break;
			default:
				break;
		}
		codeGen(tree->sibling);
	}
}

void codePrin(TreeNode * syntaxTree, char * codefile)
{
	fprintf(code, ".MODEL SMALL\n");
	fprintf(code, ".STACK 200\n");
	fprintf(code, ".DATA\n");
	codeTitle();
	fprintf(code, ".CODE\n");
	fprintf(code, ".STARTUP\n");
	codeGen(syntaxTree);
	fprintf(code, ".EXIT\n");
	fprintf(code, "END\n");
}
