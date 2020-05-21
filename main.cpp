#include <iostream>
#include <stdlib.h> //for dynamic allocation
#include <string.h> //for strings functions
#include <stdio.h>  //for gets function

using namespace std;

typedef char* ElementType;

//Tree Implementation

typedef struct node
{
    ElementType data;
    struct node* left;
    struct node* right;
}Node_t;

typedef Node_t* BTree;

void printNode(BTree T)
{
    cout<<T->data<<" ";
}

void preOrder(BTree T)
{
    if(T!=NULL)
    {
        printNode(T);
        preOrder(T->left);
        preOrder(T->right);
    }
}

void postOrder(BTree T)
{
    if(T!=NULL)
    {
        postOrder(T->left);
        postOrder(T->right);
        printNode(T);
    }
}

void inOrder(BTree T)
{
    if(T!=NULL)
    {
        inOrder(T->left);
        printNode(T);
        inOrder(T->right);
    }
}

Node_t* insert_left(BTree T,ElementType x)
{
    BTree temp;
    temp=(BTree)malloc(sizeof(Node_t));
    if(temp==NULL)
    {
        cout<<"insufficient Memory"<<endl;
        return T;
    }
    temp->left=temp->right=NULL;
    temp->data=x;
    if(T==NULL)
    {
        T=temp;
    }
    else if(T->left==NULL)
    {
        T->left=temp;
    }
    else
    {
        T->left=insert_left(T->left,x);
    }
    return T;
}

Node_t* insert_right(BTree T,ElementType x)
{
    BTree temp;
    temp=(BTree)malloc(sizeof(Node_t));
    if(temp==NULL)
    {
        cout<<"insufficient Memory"<<endl;
        return T;
    }
    temp->left=temp->right=NULL;
    temp->data=x;
    if(T==NULL)
    {
        T=temp;
    }
    else if(T->right==NULL)
    {
        T->right=temp;
    }
    else
    {
        T->right=insert_right(T->right,x);
    }
    return T;
}



//Stack Implementation

typedef struct
{
    ElementType *content;
    int topOfStack;
    int Size;
}stack_t;

int creatStack(stack_t *s,int Sizze)
{
    s->content=(ElementType*)malloc(Sizze*sizeof(ElementType));
    if(s->content==NULL)
    {
        cout<<"Insufficient Memory"<<endl;
        return 0;
    }
    s->topOfStack=-1;
    s->Size=Sizze;
    return 1;
}

void destroyStack(stack_t *s)
{
    free(s->content);
    s->content=NULL;
    s->topOfStack=-1;
    s->Size=0;
}

int isFull(stack_t s){return s.topOfStack>=s.Size-1;}

int isEmpty(stack_t s){return s.topOfStack<=-1;}

int StackPush(stack_t *s,ElementType val)
{
    if(!isFull(*s))
    {
        s->content[++s->topOfStack]=val;
        return 1;
    }
    cout<<"Stack overflew"<<endl;
    return 0;
}

int StackPop(stack_t *s,ElementType*val)
{
    if(!isEmpty(*s))
    {
        *val=s->content[s->topOfStack--];
        return 1;
    }
    cout<<"Stack is empty"<<endl;
    return 0;
}

//new functions

int countOperators(char exp[])
{
    int Count=0;
    for(int i=0;i<(signed)strlen(exp);i++)
    {
        if(exp[i]=='+'||exp[i]=='-'||exp[i]=='*'||exp[i]=='/')
            Count++;
    }
    return Count;
}

int maxLength(char exp[])
{
    int Max=0;
    int Count=0;
    for(int i=0;i<(signed)strlen(exp);i++)
    {
        if(exp[i]!='+'&&exp[i]!='-'&&exp[i]!='*'&&exp[i]!='/')
        {
            Count++;
        }
        else
        {
            if(Count>Max)
            {
                Max=Count;
            }
            Count=0;
        }
    }
    return Max;
}

int separateExp(char exp[],char** arr)
{
    int j=0,k=0;
    for(int i=0;i<(signed)strlen(exp);i++)
    {
        switch(exp[i])
        {
            case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
                arr[j][k]=exp[i];
                k++;
                arr[j][k]='\0';
                break;
            case'+':case'-':
                j++;
                arr[j][0]=exp[i];
                arr[j][1]='\0';
                j++;
                k=0;
                break;
            case' ':
                break;
            default:
                cout<<"invalid expression"<<endl;
                return 0;
        }

    }
    return 1;
}

Node_t* insertOperators(stack_t *s,BTree T)
{
    ElementType x;
    while(!isEmpty(*s))
    {
        StackPop(s,&x);
        T=insert_left(T,x);
    }

    return T;
}

Node_t* insertOperands(stack_t *s,BTree T)
{
    ElementType x;


    if(T->left==NULL&&T->right==NULL)
    {
        StackPop(s,&x);
        insert_right(T,x);
        StackPop(s,&x);
        insert_left(T,x);
        return T;
    }
    else
    {
        StackPop(s,&x);
        insert_right(T,x);
        insertOperands(s,T->left);
    }
    return T;
}

int main()
{
    BTree Tree=NULL;
    char expression[100];
    char** arr;
    stack_t operators;
    stack_t operands;

    cout<<"Enter infix expression with same priority (only + and -) : ";
    gets(expression);


    int m=countOperators(expression)*2+1;
    int n=maxLength(expression);

    arr=(char**)malloc(m*sizeof(char*));
    if(arr==NULL)
    {
        cout<<"insufficient Memory"<<endl;
        return 0;
    }
    for(int i=0;i<m;i++)
    {
        arr[i]=(char*)malloc(n*sizeof(char));
        if(arr[i]==NULL)
        {
            cout<<"insufficient Memory"<<endl;
            return 0;
        }
    }

    separateExp(expression,arr);

    creatStack(&operators,countOperators(expression));
    creatStack(&operands,countOperators(expression)+1);

    //separate operators and operands into two stacks
    for(int i=0;i<m;i++)
    {
        if(i%2==0)
            StackPush(&operands,arr[i]);
        else
            StackPush(&operators,arr[i]);
    }


    Tree=insertOperators(&operators,Tree);
    Tree=insertOperands(&operands,Tree);

    //display
    cout<<"Preorder Traverse: ";
    preOrder(Tree);
    cout<<endl;
    cout<<"Postorder Traverse: ";
    postOrder(Tree);
    cout<<endl;

    //free memory
    for(int i=0;i<m;i++)
    {
        free(arr[i]);
    }
    free(arr);
    destroyStack(&operands);
    destroyStack(&operators);
    return 0;
}
