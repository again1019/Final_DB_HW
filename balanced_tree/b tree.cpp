#include<stdio.h>  
#include<stdlib.h>  
#define number 5   
struct tree  
{  
struct tree*left; //左子樹   
int data;         //資料   
struct tree*right;//右子樹   
};  
typedef struct tree node; //簡化 struct tree 為 node   
typedef node*bnode;       //bnode 指向 node   
   
bnode binary_tree(bnode,int);// 二元樹建立  
void inorder(bnode);   //   中序
void print(bnode);   //   印出執行結果
   
  
int main()  
{  
int array[number],i;  
bnode root = NULL;  //令根節點為空   
printf("依序輸入 %d 筆資料\n\n",number);  
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
  //把 bnode* sizeof(node)大小的記憶體位置，轉換成node指標，放到 root中  
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
        if(current->data > val) //如果根節點的值比當前放入的值大   
        {  
         current = current->left; //當前節點改為根節點左邊的點   
        }  
        else  
        {  
         current = current->right;  
        }  
      }  
      if(backup->data>val)  //如果根節點的值比當前放入的值大  
      {  
       backup->left = newnode; //給予根節點左邊的點配置記憶體空間(newnode)   
      }  
      else  
      {  
       backup->right = newnode;  
      }  
   }  
   return root;  
}  
