/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

int if_debug = FALSE;

void debug(const char *__restrict__ __format, ...)
{
    if (if_debug)
    {
        printf(__format);
    }
}

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char *tokenString)
{
    switch (token)
    {
    case IF:
    case WHILE:
    case ELSE:
    case INT:
    case RETURN:
    case VOID:
        fprintf(listing,
                "reserved word: %s\n", tokenString);
        break;
    case ASSIGN:
        fprintf(listing, "=\n");
        break;
    case LT:
        fprintf(listing, "<\n");
        break;
    case LE:
        fprintf(listing, "<=\n");
        break;
    case RT:
        fprintf(listing, ">\n");
        break;
    case RE:
        fprintf(listing, ">=\n");
        break;
    case EQ:
        fprintf(listing, "==\n");
        break;
    case NE:
        fprintf(listing, "!=\n");
        break;
    case LPAREN:
        fprintf(listing, "(\n");
        break;
    case RPAREN:
        fprintf(listing, ")\n");
        break;
    case SEMI:
        fprintf(listing, ";\n");
        break;
    case COMMA:
        fprintf(listing, ",\n");
        break;
    case PLUS:
        fprintf(listing, "+\n");
        break;
    case MINUS:
        fprintf(listing, "-\n");
        break;
    case TIMES:
        fprintf(listing, "*\n");
        break;
    case OVER:
        fprintf(listing, "/\n");
        break;
    case LBRACE:
        fprintf(listing, "{\n");
        break;
    case RBRACE:
        fprintf(listing, "}\n");
        break;
    case ENDFILE:
        fprintf(listing, "EOF\n");
        break;
    case NUM:
        fprintf(listing,
                "NUM, val= %s\n", tokenString);
        break;
    case ID:
        fprintf(listing,
                "ID, name= %s\n", tokenString);
        break;
    case ERROR:
        fprintf(listing,
                "ERROR: %s\n", tokenString);
        break;
    default: /* should never happen */
        fprintf(listing, "Unknown token: %d\n", token);
    }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode *newStmtNode(StmtKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
    {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

TreeNode *newNullStmtNode()
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
    {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->lineno = lineno;
    }
    return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode *newExpNode(ExpKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
    {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

TreeNode *newNullExpNode()
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
    {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char *copyString(char *s)
{
    int n;
    char *t;
    if (s == NULL)
        return NULL;
    n = strlen(s) + 1;
    t = malloc(n);
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
        strcpy(t, s);
    return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
int static indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno += 4
#define UNINDENT indentno -= 4

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{
    int i;
    for (i = 0; i < indentno; i++)
        fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode *tree)
{
    int i;
    INDENT;
    
    while (tree != NULL) {
        printSpaces();
        if (tree->nodekind == StmtK)
        {
            switch (tree->kind.stmt)
            {
            case SelectionK:
            {
                fprintf(listing, "If\n");
                break;
            }
            case WhileK:
            {
                fprintf(listing, "while\n");
                break;
            }
            case AssignK:
            {
                fprintf(listing, "Assign to: %s\n", tree->attr.name);
                break;
            }
            case ReturnK:
            {
                char *type;
                if (tree->type == Integer)
                {
                    type = "int";
                }
                else if (tree->type == Void)
                {
                    type = "void";
                }
                fprintf(listing, "Return type: %s\n", type);
                break;
            }
            case VarDeclarationK:
            {
                if (tree->type == Integer)
                {
                    fprintf(listing, "variable declaration: type: int, name: %s\n", tree->attr.name);
                }
                else if (tree->type == Void)
                {
                    fprintf(listing, "variable declaration: type: %s, name: %s\n", tree->attr.name);
                }
                break;
            }
            case FuncDeclarationK:
            {
                char *type;
                if (tree->type == Integer)
                {
                    type = "int";
                }
                else if (tree->type == Void)
                {
                    type = "void";
                }
                fprintf(listing, "function declaration: type: %s, name: %s\n", type, tree->attr.name);
                break;
            }
            case CompoundK:
            {
                fprintf(listing, "compound statement\n");
                break;
            }
            default:
                fprintf(listing, "Unknown ExpNode kind\n");
                break;
            }
        } else if (tree->nodekind == ExpK) {
            switch (tree->kind.exp)
            {
            case OpK: {
                fprintf(listing, "Op: ");
                printToken(tree->attr.op, "\0");
                break;
                }
            case ConstK:
                fprintf(listing, "Const: %d\n", tree->attr.val);
                break;
            case IdK:
                fprintf(listing, "Id: %s\n", tree->attr.name);
                break;
            case ParamK:
            {
                if (tree->type == Void)
                {
                    fprintf(listing, "Param: type: void\n");
                }
                else if (tree->type == Integer)
                {
                    fprintf(listing, "Param: type: int, name: %s\n", tree->attr.name);
                }
                break;
            }
            case CallK:
                fprintf(listing, "Call: %s\n", tree->attr.name);
                break;
            default:
                fprintf(listing, "Unknown ExpNode kind\n");
                break;
            }
        } else {
            fprintf(listing, "Unknown node kind\n");
        }

        for (int i = 0; i < MAXCHILDREN; i++)
        {
            if (tree->child[i] != NULL)
            {
                printTree(tree->child[i]);
            }
        }
        tree = tree->sibling;
    }
    UNINDENT;
}
