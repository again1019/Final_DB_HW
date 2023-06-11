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
} Node;
// 計算節點的高度
int getHeight(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}
//更新節點的高度
void updateHeight(Node* node) {
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    if (leftHeight > rightHeight) {
	    node->height = leftHeight + 1;
	} else {
	    node->height = rightHeight + 1;
	}
}

//平衡因子
int getBalanceFactor(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

Node* rotateRight(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    // 更新高度
    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

Node* rotateLeft(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    // 更新高度
    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

Node* balance(Node* node) {
    // 更新節點的高度
    updateHeight(node);

    // 檢查平衡因子
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1) {
        // 右旋

        if (getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    else if (balanceFactor < -1) {
        // 左旋
        if (getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }

    // 節點平衡，不需要進行旋轉操作
    return node;
}
// insert node
void insert(Node** root, int course_id, const char* student_id) {
    Node* cur = *root;
    Node* pre = NULL;
    Node* prepre = NULL;
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->course_id = course_id;
    newNode->count = 0;
    strcpy(newNode->student_id[newNode->count], student_id);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // 初始化高度為1
    
    if (cur == NULL) {
        *root = newNode;
    }
    else{
    	while(cur){
    		if (course_id < cur->course_id) {
    			prepre=pre;
	            pre=cur;
	            cur=cur->left;
	        }
	        else if (course_id > cur->course_id)
	        {
	        	prepre=pre;
	            pre = cur;
				cur = cur->right;
	        }
	        else if (course_id == cur->course_id)
	        {
	        	prepre=pre;
	        	pre = cur;
				break;
	        }	
		}
		if (course_id > pre->course_id) {
			if(pre->height==0)
				pre->height=1;
			else
				pre->height=0;
			pre->right = newNode;
		} else if(course_id < pre->course_id){
			if(pre->height==0)
				pre->height=1;
			else
				pre->height=0;
			pre->left = newNode;
		}
		else if(course_id == pre->course_id)
		{
			free(newNode);
			pre->count = (pre->count)+1;
			strcpy(pre->student_id[pre->count], student_id);	  
		} 
		*root = balance(*root);
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
            insert(&root, course_id, student_id);
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
 }

int main() {
    Node* root = NULL;

    root = insert_nodes_from_blocks();
	//printf("%d\n", root->course_id);
//	printf("%d\n", root->left->course_id);
//	printf("%d\n", root->right->course_id);
	//inorder(root);
	//preorder(root);
    int target_course_id=1;
    while(target_course_id != 0)
    {
        printf("Enter course ID to search: ");
        scanf("%d", &target_course_id);
        if(target_course_id == 0)
            break;
        else
            search_course_id(root, target_course_id);
    }

    return 0;
}


