#ifndef SYMBANALYZE_H_INCLUDED
#define SYMBANALYZE_H_INCLUDED

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *);

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *);


#endif // SYMBANALYZE_H_INCLUDED
