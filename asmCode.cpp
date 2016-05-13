#include "globles.h"
#include "symbTab.h"
#include "asmCode.h"

static int tmpOffset = 0;
static int jLabel = 0; //跳转标识
int fa=0, fb=0;

/* 生成代码 */
static void codeGen (TreeNode * tree);

static void genStmt(TreeNode * tree) //语句汇编生成
{
	TreeNode * p1, * p2, * p3, * p4;
	int label1, label2;
	//label1 = jLabel++; label2 = jLabel++;
	switch(tree->kind.stmt)
	{
	case IfK:
		label1 = jLabel++;
		label2 = jLabel++;
		p1 = tree->child[0] ;
		p2 = tree->child[1] ;
		p3 = tree->child[2] ;
		//codeGen(p1);
		switch(p1->attr.op)
		{
		case LT:
			fprintf(code, "MOV EBX, %s\n", p1->child[0]->attr.name);
			fprintf(code, "MOV EBX, %d\n", p1->child[1]->attr.val);
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JG label_%d\n", label1);
			break;
		case EQ:
			fprintf(code, "MOV EBX, %s\n", p1->child[0]->attr.name);
			fprintf(code, "MOV EBX, %d\n", p1->child[1]->attr.val);
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JZ label_%d\n", label1);
		}
		codeGen(p2);
		switch(p1->attr.op)
		{
		case LT:
			fprintf(code, "J label_%d\n", label2);
			break;
		case EQ:
			fprintf(code, "J label_%d\n", label2);
		}
		fprintf(code, "label_%d:\n", label1);
		codeGen(p3);
		fprintf(code, "label_%d:\n", label2);
		break;
	case WhileK:
		label1 = jLabel++;
		p1 = tree->child[0] ;
		p2 = tree->child[1] ;
		//codeGen(p1);
		switch(p1->attr.op)
		{
		case LT:
			fprintf(code, "MOV EBX, %s\n", p1->child[0]->attr.name);
			fprintf(code, "MOV EBX, %d\n", p1->child[1]->attr.val);
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JG label_%d\n", label1);
			break;
		case EQ:
			fprintf(code, "MOV EBX, %s\n", p1->child[0]->attr.name);
			fprintf(code, "MOV EBX, %d\n", p1->child[1]->attr.val);
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JZ label_%d\n", label1);
		}
		codeGen(p2);
		fprintf(code, "label_%d:\n", label1);
		break;
	case ForK:
		label1 = jLabel++;
		label2 = jLabel++;
		p1 = tree->child[0];
		p2 = tree->child[1];
		p3 = tree->child[2];
		p4 = tree->child[3];
		codeGen(p1);
		fprintf(code, "label_%d:\n", label1);
		//codeGen(p2);
		switch(p2->attr.op)
		{
		case LT:
			fprintf(code, "MOV EBX, %s\n", p2->child[0]->attr.name);
			fprintf(code, "MOV EBX, %d\n", p2->child[1]->attr.val);
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JG label_%d\n", label2);
			break;
		case EQ:
			fprintf(code, "MOV EBX, %s\n", p2->child[0]->attr.name);
			fprintf(code, "MOV EBX, %d\n", p2->child[1]->attr.val);
			fprintf(code, "CMP EAX, EBX\n");
			fprintf(code, "JZ label_%d\n", label2);
		}
		codeGen(p4);
		codeGen(p3);
		fprintf(code, "J label_%d\n", label1);
		fprintf(code, "label_%d:\n", label2);
		break;
	case AssignK:
		p1 = tree->child[0];
		codeGen(p1);
		fa=0;
		fprintf(code, "MOV %s, EAX\n", tree->attr.name);
		break;
	case Vark:
		break;
	}
}

static void genExp(TreeNode * tree) //算式汇编生成
{
	TreeNode * p1, * p2;
	int f1=0, f2=0;
	switch(tree->kind.exp)
	{
	case OpK:
		p1 = tree->child[0];
		p2 = tree->child[1];
		if(p1->kind.exp == OpK)
		{
			codeGen(p1);
			codeGen(p2);
			f1=1, f2=0;
		}else{
			codeGen(p2);
			codeGen(p1);
			f1=0, f2=1;
		}
		switch(tree->attr.op)
		{
		case PLUS:
				fprintf(code, "ADD EAX, EBX\n");
				fb=0;
			break;
		case MINUS:
			if(f1)
			{
				fprintf(code, "SUB EAX, EBX\n");
				fb=0;
			}else{
				fprintf(code, "SUB EBX, EAX\n");
				fa=0;
			}
			break;
		case MUL:
				fprintf(code, "MUL EBX\n");
				fb=0;
			break;
		}
	case ConstK:
		if(!fa){fprintf(code, "MOV EAX, %d\n", tree->attr.val); fa=1;}
		else{fprintf(code, "MOV EBX, %d\n", tree->attr.val); fb=1;}
		break;
	case IdK:
		if(st_lookup(tree->attr.name) != -1)
		{
			if(!fa){fprintf(code, "MOV EAX, %s\n", tree->attr.name); fa=1;}
			else{fprintf(code, "MOV EBX, %s\n", tree->attr.name); fb=1;}
		}else{fprintf(code, ">>>Error变量%s未声明\n", tree->attr.name);} //错误检查
		break;
	}
}

static void codeGen(TreeNode * tree) //判断是stmt或exp
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

void codePrin(TreeNode * syntaxTree) //打印头部和尾部
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
