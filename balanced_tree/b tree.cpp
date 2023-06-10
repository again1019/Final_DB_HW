#include<stdio.h>  
#include<stdlib.h>  
#define number 5   
struct tree  
{  
struct tree*left; //���l��   
int data;         //���   
struct tree*right;//�k�l��   
};  
typedef struct tree node; //²�� struct tree �� node   
typedef node*bnode;       //bnode ���V node   
   
bnode binary_tree(bnode,int);// �G����إ�  
void inorder(bnode);   //   ����
void print(bnode);   //   �L�X���浲�G
   
  
int main()  
{  
int array[number],i;  
bnode root = NULL;  //�O�ڸ`�I����   
printf("�̧ǿ�J %d �����\n\n",number);  
for(i=0;i<number;i++)  
{  
  scanf("%d",&array[i]);  
  root = binary_tree(root,array[i]);  
}  
printf("\n inorder:   ");  
inorder(root);   
printf("\n\n");  
system("pause");  
return 0;  
}  
  
void print(bnode ptr)  
{  
  printf("%d  ",ptr->data);  
}  
   
void inorder(bnode ptr)  
{  
  if(ptr!=NULL)  
  {  
   inorder(ptr->left);  
   print(ptr);  
   inorder(ptr->right);  
  }  
}  
   
bnode binary_tree(bnode root,int val)  
{  
  bnode newnode,current,backup;  
  newnode = (bnode)malloc(sizeof(node));
  //�� bnode* sizeof(node)�j�p���O�����m�A�ഫ��node���СA��� root��  
  newnode->left = NULL;  
  newnode->data = val;  
  newnode->right= NULL;  
    
  if(root == NULL)  
  {  
   root = newnode;  
   return root;  
  }  
  else  
  {  
      for(current = root;current!=NULL;)  
      {  
        backup = current;  
        if(current->data > val) //�p�G�ڸ`�I���Ȥ��e��J���Ȥj   
        {  
         current = current->left; //��e�`�I�אּ�ڸ`�I���䪺�I   
        }  
        else  
        {  
         current = current->right;  
        }  
      }  
      if(backup->data>val)  //�p�G�ڸ`�I���Ȥ��e��J���Ȥj  
      {  
       backup->left = newnode; //�����ڸ`�I���䪺�I�t�m�O����Ŷ�(newnode)   
      }  
      else  
      {  
       backup->right = newnode;  
      }  
   }  
   return root;  
}  
