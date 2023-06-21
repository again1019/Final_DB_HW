#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dirent.h"

#define BLOCK_FOLDER "block/cid/"
#define MAX 5000
#define MAX_LENGTH 50

// balanced tree
typedef struct Node {
    int course_id;
    char student_id[MAX][12];
    int count;
    int height; // 新增的高度變量
    struct Node* left;
    struct Node* right;
    struct Node* myparent;
    
} Node;

// Calculate height
int height(Node *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}
// Right rotate
struct Node *rightRotate(struct Node *y) {
  struct Node *x = y->left;
  struct Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}

// Left rotate
struct Node *leftRotate(struct Node *x) {
  struct Node *y = x->right;
  struct Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y;
}

// Get the balance factor
int getBalance(struct Node *N) {
  if (N == NULL)
    return 0;
  return height(N->left) - height(N->right);
}


// insert node
Node* insert(Node* root, int course_id, const char* student_id) {
    Node* cur = root;
    Node* pre = NULL;
    Node* prepre = NULL;
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->course_id = course_id;
    newNode->count = 0;
    strcpy(newNode->student_id[newNode->count], student_id);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->myparent = NULL;
    newNode->height = 1; // 初始化高度為1
    
    if (cur == NULL) {
        root = newNode;
        return root;
    }
    else{
    	if (course_id < root->course_id)
		    root->left = insert(root->left, course_id,student_id);
		else if (course_id > root->course_id)
		    root->right = insert(root->right, course_id,student_id);
		else if (course_id == root->course_id){
			root->count = (root->count)+1;
			strcpy(root->student_id[root->count], student_id);
			return root;
		}
	
	// Update the balance factor of each node and
  	// Balance tree
	  root->height = 1 + max(height(root->left),height(root->right));
	
	  int balance = getBalance(root);
	  if (balance > 1 && course_id < root->left->course_id)//平衡因子大於 1，且插入的節點位於左子節點的左子樹上，執行右旋
	    return rightRotate(root);
	
	  if (balance < -1 && course_id > root->right->course_id)//平衡因子小於 -1，且插入的節點位於右子節點的右子樹上，執行左旋
	    return leftRotate(root);
		
	 //平衡因子大於 1，且插入的節點位於左子節點的右子樹上，先對當前節點的左子樹進行左旋，再對當前節點執行右旋
	  if (balance > 1 && course_id > root->left->course_id) {
	    root->left = leftRotate(root->left);
	    return rightRotate(root);
	  }
	//平衡因子小於 -1，且插入的節點位於右子節點的左子樹上，那麼先對當前節點的右子樹進行右旋，再對當前節點執行左旋
	  if (balance < -1 && course_id < root->right->course_id) {
	    root->right = rightRotate(root->right);
	    return leftRotate(root);
	  }
	
	  return root;
	}

}



Node* insert_nodes_from_blocks() {
    DIR* dir;
    struct dirent* entry;
    Node *root=NULL;

    // open folder
    dir = opendir(BLOCK_FOLDER);
    if (dir == NULL) {
        printf("Error opening block folder.\n");
    }
    int pass=0;
    int cc=0;
    while ((entry = readdir(dir)) != NULL) {
        if(pass==0 || pass==1){
    		pass++;
    		continue;
    	}
        else if (entry->d_type == DT_REG) {
            char filename[100];
            strcpy(filename, BLOCK_FOLDER);
            strcat(filename, entry->d_name);


            int course_id;
            char student_id[12];
            sscanf(entry->d_name, "%d_%[^.]", &course_id, student_id);
            root = insert(root, course_id, student_id);
           // printf("%d:%d,%s\n",cc, course_id, student_id);
        }
        cc++;
    }
    closedir(dir);
    return root;
}

void search_course_id(Node* root, int target_course_id) {
    Node* cur = root;
    if (root == NULL) {
        printf("This course does not exist!\n");
        return;
    }
    if (target_course_id < cur->course_id) {
        search_course_id(cur->left, target_course_id);
    } else if (target_course_id > cur->course_id) {
        search_course_id(cur->right, target_course_id);
    } else if(target_course_id == cur->course_id){
        //printf("Course ID: %d\n", cur->course_id);
        printf("Student ID:\n");
        int cc=1;
        for(int i = 0; i <= cur->count; i++)  {
            // 打開與course_id對應的block
            char block_filename[100];
            sprintf(block_filename, "%s%d_%s.csv", BLOCK_FOLDER, cur->course_id, cur->student_id[i]);
            FILE* block_file = fopen(block_filename, "r");
            char line[MAX_LENGTH];
            int firstLine = 1;

            if (block_file == NULL) {
                printf("Unable to open file.\n");
                break;
            }
            else{	
                while (fgets(line, sizeof(line), block_file)) {
                    char *token;
                    char *student_id;

                    if (firstLine) {
                        firstLine = 0;
                        continue;
                    }

                    token = strtok(line, ",");
                    student_id = token;


                    printf("%d:%s\n", cc,student_id);
                    cc++;
                }
                line[0]='\0';
            }
            fclose(block_file);
        }
    }
}
void preorder(Node* root)
{
	if (root != NULL) {
		printf("%d\n", root->course_id);
        preorder(root->left);
        preorder(root->right);
     }
     else
     	printf("NULL\n");
    	
}
 void inorder(Node* root) {
     if (root != NULL) {
         inorder(root->left);
         printf("%d\n", root->course_id);
         inorder(root->right);
     }
     else
     	printf("NULL\n");
 }

int main() {
    Node* root = NULL;

    root = insert_nodes_from_blocks();
   // preorder(root);
    //inorder(root);
    int target_course_id=1;
    while(target_course_id != 0)
    {
        printf("Enter course ID : ");
        scanf("%d", &target_course_id);
        if(target_course_id == 0)
            break;
        else
            search_course_id(root, target_course_id);
    }

    return 0;
}

