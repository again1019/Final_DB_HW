#include <bits/stdc++.h>
//#include <conio.h>   //getch

using namespace std;

int bucketSize = 10;

// Create 2 classes, one for node and one for bptree;

class node {
public:
	int *key; // key is file name
	node** ptr;
	bool isLeaf;
    int size;

	node() {
        key = new int[bucketSize];
        ptr = new node*[bucketSize + 1];
        isLeaf = false;
        size = 0;
    }
};

class bptree {
public:
	node* root; // Root of tree stored here;
	bptree() {
        root = NULL;
    }

	int search(int);
	void insert(int);
	void shiftLevel(int, node*, node*);

	node* findParent(node*, node*);
	node* getRoot() {
        return root;
    }
};


void bptree::insert(int x) {
    // insert a new node into the tree
	if (root == NULL) {
		root = new node;
		root->key[0] = x;
		root->isLeaf = true;
		root->size = 1;
	} else {
		node* current = root;
		node* parent;
        
        // if current node is not a leaf, then go to the leaf
		while (!current->isLeaf) {
			parent = current;
            // | < a | A | a <= x < b | B | >= b | 
            // |  i  | i |     i+1    |i+1| i+2  | 
			for (int i = 0; i < current->size; i++) {
				if (abs(x) < abs(current->key[i])) {
					current = current->ptr[i];
					break;
				}
				if (i == current->size - 1) {
					current = current->ptr[i + 1];
					break;
				}
			}
		}

		// reached leaf;
		if (current->size < bucketSize) { // if the node to be inserted is not filled
			int i = 0;

			// find the right position of element to be inserted
			while (abs(x) > abs(current->key[i]) && i < current->size)
				// go to ptr where needs to be inserted.
				i++;
			for (int j = current->size; j > i; j--)
				// adjust the elements to make space for new element
				current->key[j] = current->key[j - 1];
			current->key[i] = x;

			// size should be increased by 1
			current->size++;
             
            // ptr in leaf just need the end ptr to next leaf, other is null
			current->ptr[current->size] = current->ptr[current->size - 1];
			current->ptr[current->size - 1] = NULL;
		} else { // if block does not have enough space;
			node* newLeaf = new node;
			int tempNode[bucketSize + 1]; // in order to copy the elements

			for (int i = 0; i < bucketSize; i++) {
				// all elements of this block copy
				tempNode[i] = current->key[i];
            }
			int i = 0, j;

			// find the right position of element to be inserted
			while (abs(x) > abs(current->key[i]) && i < bucketSize)
				i++;
			for (int j = bucketSize + 1; j > i; j--)
				// adjust the elements to make space for new element
				// current->key[j] = current->key[j - 1];
				tempNode[j] = tempNode[j - 1];
			tempNode[i] = x;
			// inserted element in its rightful position;

			newLeaf->isLeaf = true;
			current->size = (bucketSize + 1) / 2;
			newLeaf->size = (bucketSize + 1) - (bucketSize + 1) / 2; 

            
            // ================= rerangement new leaf =================
			current->ptr[current->size] = newLeaf; // old end ptr is new (be divided size)
			newLeaf->ptr[newLeaf->size] = current->ptr[bucketSize]; // new end ptr is old's old size end (not be divided size)

			current->ptr[newLeaf->size] = current->ptr[bucketSize]; // if the size different, then the ptr is not the same
			current->ptr[bucketSize] = NULL;

			for (int i = 0; i < current->size; i++)
				current->key[i] = tempNode[i];

			for (int i = 0, j = current->size; i < newLeaf->size; i++, j++)
				newLeaf->key[i] = tempNode[j];


			// if this is root, create a new root;
			// else need to increase the height of tree;
			if (current == root) {
				node* newRoot = new node;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->ptr[0] = current;
				newRoot->ptr[1] = newLeaf;
				newRoot->isLeaf = false;
				newRoot->size = 1;
				root = newRoot;
			} else {
				shiftLevel(newLeaf->key[0], parent, newLeaf);
            }
		}
	}
}

void bptree::shiftLevel(int x, node* current, node* child) { 
    // insert or create an internal node;
	if (current->size < bucketSize) { // if it is not full
		int i = 0;
		while (abs(x) > abs(current->key[i]) && i < current->size)
			i++;
		for (int j = current->size; j > i; j--) {
			current->key[j] = current->key[j - 1];
        }
		for (int j = current->size + 1; j > i + 1; j--) {
			current->ptr[j] = current->ptr[j - 1];
        }
		current->key[i] = x;
		current->size++;
		current->ptr[i + 1] = child;
	} else { 
        // shift up
		node* newInternal = new node;
		int tempKey[bucketSize + 1];
		node* tempPtr[bucketSize + 2];

        // copy to temp
		for (int i = 0; i < bucketSize; i++)
			tempKey[i] = current->key[i];
		for (int i = 0; i < bucketSize + 1; i++)
			tempPtr[i] = current->ptr[i];

		int i = 0, j;
		while (abs(x) > abs(current->key[i]) && i < bucketSize)
			i++;

		for (int j = bucketSize + 1; j > i; j--)
			tempKey[j] = tempKey[j - 1];
		tempKey[i] = x;
        

		for (int j = bucketSize + 2; j > i + 1; j--)
			tempPtr[j] = tempPtr[j - 1];
		tempPtr[i + 1] = child;
		newInternal->isLeaf = false;
		current->size = (bucketSize + 1) / 2;

		newInternal->size = bucketSize - (bucketSize + 1) / 2;

		for (int i = 0, j = current->size + 1; i < newInternal->size; i++, j++)
			newInternal->key[i] = tempKey[j];

		for (int i = 0, j = current->size + 1; i < newInternal->size + 1; i++, j++)
			newInternal->ptr[i] = tempPtr[j];

		if (current == root) {
			node* newRoot = new node;
			newRoot->key[0] = current->key[current->size];
			newRoot->ptr[0] = current;
			newRoot->ptr[1] = newInternal;
			newRoot->isLeaf = false;
			newRoot->size = 1;
			root = newRoot;
		}

		else {
            shiftLevel(current->key[current->size],findParent(root, current), newInternal);
        }
	}
}
int bptree::search(int x) {
	if (root == NULL)
		return -1;
	else {
		node* current = root;
        // find the leaf node
		while (current->isLeaf == false) {
			for (int i = 0; i < current->size; i++) {
				if (abs(x) < abs(current->key[i])) {
					current = current->ptr[i];
                    break;
				}
				if (i == current->size - 1) {
					current = current->ptr[i + 1];
				    break;
                }
			}
		}
        int result = 0;
        for (int i = 0; i < current->size; i++) {
            cout << x << ":" << current->key[i] << endl;  //in order to degug
            if (abs(x) < abs(current->key[i]) && abs(x) > abs(current->key[i - 1])) {
                result = current->key[i - 1];
            } else if (abs(x) == abs(current->key[i])) {
                result = current->key[i];
            } else if (abs(x) > abs(current->key[i]) && i == current->size - 1) {
                result = current->key[i];
            }
        }
        return result;
	}
}

node* bptree::findParent(node* current, node* child) {
	node* parent;
	if (current->isLeaf || (current->ptr[0])->isLeaf)
		return NULL;

	for (int i = 0; i < current->size + 1; i++) {
		if (current->ptr[i] == child) {
			parent = current;
			return parent;
		}
		else {
			parent = findParent(current->ptr[i], child);
			if (parent != NULL)
				return parent;
		}
	}
	return parent;
}

int convert(char* str) {
    if (str[0] == 'D') {
        bool flag = false;
        int num = 0;
        int size = 8;
        if (strlen(str) == 8) {
            flag = true;
        }
        for (int i = 1; i < strlen(str); i++) {
            int tmp = pow(10, size);
            num = num + (str[i] - '0') * tmp;
            size--;
        }
        if (flag) {
            num = num * -1;
        }
        return num;
    } else {
        int num = 0;
        int size = 3;
        for (int i = 0; i < strlen(str); i++) {
            int tmp = pow(10, size);
            num = num + (str[i] - '0') * tmp;
            size--;
        }
        return num;
    }
}

int inputConvert(char* str) {
    if (str[0] == 'D') {
        bool flag = false;
        int num = 0;
        int size = 8;
        if (strlen(str) == 8) {
            flag = true;
        }
        for (int i = 1; i < strlen(str); i++) {
            int tmp = pow(10, size);
            num = num + (str[i] - '0') * tmp;
            size--;
        }
        if (flag) {
            num = num * -1;
        }
        return num;
    } else if (strlen(str) == 4) {
        int num = 0;
        int size = 3;
        for (int i = 0; i < strlen(str); i++) {
            int tmp = pow(10, size);
            num = num + (str[i] - '0') * tmp;
            size--;
        }
        return num;
    } else {
        cout << "input error: please input student ID or course ID" << endl;
        return -1;
    }
}


int main() {
	bptree sid;
    bptree cid;
    
    // ========== create b+ tree ==========
    FILE *fp;
    fp = fopen("SID.txt", "r");
    while (!feof(fp)) {
        char temp[11];
        fscanf(fp, "%s", temp);
        sid.insert(convert(temp));
    }
    fclose(fp);

    fp = fopen("CID.txt", "r");
    int record = 0;
    while (!feof(fp)) {
        char temp[16];
        fscanf(fp, "%s", temp);
        if (record != convert(temp)) {
            cid.insert(record);
        };
        record = convert(temp);
    }
    fclose(fp);


    // ============ input ============
    char input[216];
    bool errorinput = true;    

    while (errorinput) {
        cout << "Please input student ID or course ID : ";
        scanf("%11s", input);
        if (strlen(input) == 8 && input[0] == 'D') {
            errorinput = false;
        } else if (strlen(input) == 10 && input[0] == 'D') {
            errorinput = false;
        } else if (strlen(input) == 4) {
        	for (int i = 0; i < 4; ++i) {
        		if (!isdigit(input[i])){
        			errorinput = true;
        			break;
				}else{
					errorinput = false;	
				}	
			}
        } else {
            cout << "input error: " ;
        }
    }
    int ans;
    if (input[0] == 'D') {
        ans = sid.search(abs(inputConvert(input)));
    } else {
        ans = cid.search(inputConvert(input));
    } 

    // ============ get filename ============    
    int fileNum = 1;
    char filename[80][22];
    memset(filename, 0, sizeof(filename));
    if (input[0] == 'D') {
        filename[0][0] = 'D';
        if (ans < 0) {
            int size = 8;
            ans = ans * -1;
            for (int i = 1; i < 8; i++) {
                int tmp = pow(10, size);
                filename[0][i] = (char)((int)ans/tmp + '0');
                ans = ans % tmp;
                size--;
            } 
            filename[0][8] = '.';
            filename[0][9] = 'c';
            filename[0][10] = 's';
            filename[0][11] = 'v';
            filename[0][12] = '\0';
        } else {
            int size = 8;
            for (int i = 1; i < 10; i++) {
                int tmp = pow(10, size);
                filename[0][i] = ans/tmp + '0';
                ans = ans % tmp;
                size--;
            }
            filename[0][10] = '.';
            filename[0][11] = 'c';
            filename[0][12] = 's';
            filename[0][13] = 'v';
            filename[0][14] = '\0';
        }
    } else {
        char tmpstr[4]; 
        int size = 3;
        for (int i = 0; i < 4; i++) {
            int tmp = pow(10, size);
            tmpstr[i] = ans/tmp + '0';
            ans = ans % tmp;
            size--;
        }
        fp = fopen("CID.txt", "r");
        int i = 0;
        while (!feof(fp)) {
            char buf[16];
            fscanf(fp, "%s", buf);
            if (buf[0] == tmpstr[0] && buf[1] == tmpstr[1] && buf[2] == tmpstr[2] && buf[3] == tmpstr[3]) {
                int j = 0;
                for (j = 0; j < 16; j++) {
                    filename[i][j] = buf[j];
                    if (buf[j] == '\0') {
                        break;
                    }
                }
                filename[i][j] = '.';
                filename[i][j+1] = 'c';
                filename[i][j+2] = 's';
                filename[i][j+3] = 'v';
                filename[i][j+4] = '\0';
                i++;
            }
        }
        fileNum = 1 + i;
    }
    

    
    // ========= output file name & sequence search by csv =========
    const char* S_directory = "./block/sid";
	const char* C_directory = "./block/cid";

    char filepath[256];	
    if (input[0] == 'D') {
    	cout << "\n=====================================\nThe file name is: ";
		cout << filename[fileNum - 1] << endl;
		
		snprintf(filepath, sizeof(filepath), "%s/%s", S_directory, filename[fileNum - 1]);
		printf("\nSID:%s\nfilepath:%s\nCID:\n",input,filepath);
		FILE* file = fopen(filepath, "r");
	    char line[50];
	    int compare = 0;
	    char *field;
	    char fieldBuffer[100];
	    int j = 0;
	    if (fgets(line, 50, file) != NULL) {
//       				 printf("%s", line);
	    }

		int flag = 1;
		int out = 0;
	    while (fgets(line, 100, file) != NULL) {
	        field = strtok(line, ",");
	        while (field != NULL) {
	        	if (flag > 0){ 
	            	sscanf(field, " %255[^,\n]",  fieldBuffer);
	            	if (strcmp(fieldBuffer, input) == 0){
						out = 1;
					}
	            } else {
	            	if (out == 1){
	            		sscanf(field, " %255[^,\n]",  fieldBuffer);
	            		printf("%s\n",fieldBuffer);
	            		out = 9;
					}
				}

	            field = strtok(NULL, ",");
	            flag *= -1;
	        }
	    }
	    if(out == 0) {
        	printf("not found\n");
		}
		printf("\n");

        fclose(file);	
	} else {
		int check = 1;
		for (int j = 0; j < 4 ; ++j) {
			if (input[j] != filename[0][j]){
				check = 0;
				break;
			}
		}

		if (check){
			cout << "The file name is: ";
		    for (int i = 0; i < fileNum; i++) {
		        cout << filename[i] << endl;
		    }
			for (int i = 0; i < fileNum - 1; i++) {
				snprintf(filepath, sizeof(filepath), "%s/%s", C_directory, filename[i]);
	        	printf("\n=====================================\n\nCID:%s\nfilepath:%s\nSID:\n",input,filepath);
	    		FILE* file = fopen(filepath, "r");
			    char line[50];
			    int compare = 0;
			    char *field;
			    char fieldBuffer[100];
			    int j = 0;
			    if (fgets(line, 50, file) != NULL) {
	//       				 printf("%s", line);
			    }
	
				int flag = 1;
				int out = 0;
			    while (fgets(line, 100, file) != NULL) {
			        field = strtok(line, ",");
			        while (field != NULL) {
			        	if (flag > 0){ 
			            	sscanf(field, " %255[^,\n]",  fieldBuffer);
			           		printf("%s ",fieldBuffer);
	
			            } else {
		            		sscanf(field, " %255[^,\n]",  fieldBuffer);
	
						}
	
			            field = strtok(NULL, ",");
			            flag *= -1;
			        }
			    }

				printf("\n");
		        fclose(file);
			}
    	}
	}
	

	return 0;
}

