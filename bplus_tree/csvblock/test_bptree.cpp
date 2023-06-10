#include <bits/stdc++.h>
using namespace std;

int bucketSize = 5000;

// Create 2 classes, one for node and one for bptree;

class node {
public:
	string *key; // key is file name
	node** ptr;
	bool isLeaf;
    int size;

	node() {
        key = new string[bucketSize];
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

	string search(string);
	void insert(string);
	void shiftLevel(string, node*, node*);

	node* findParent(node*, node*);
	node* getRoot() {
        return root;
    }
};


void bptree::insert(string x) {
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
				if (x.compare(current->key[i]) < 0) {
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
			while (x.compare(current->key[i]) > 0 && i < current->size)
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
			string tempNode[bucketSize + 1]; // in order to copy the elements

			for (int i = 0; i < bucketSize; i++) {
				// all elements of this block copy
				tempNode[i] = current->key[i];
            }
			int i = 0, j;

			// find the right position of element to be inserted
			while (x.compare(current->key[i]) > 0 && i < bucketSize)
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

void bptree::shiftLevel(string x, node* current, node* child) { 
    // insert or create an internal node;
	if (current->size < bucketSize) { // if it is not full
		int i = 0;
		while (x.compare(current->key[i]) > 0 && i < current->size)
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
		string tempKey[bucketSize + 1];
		node* tempPtr[bucketSize + 2];

        // copy to temp
		for (int i = 0; i < bucketSize; i++)
			tempKey[i] = current->key[i];
		for (int i = 0; i < bucketSize + 1; i++)
			tempPtr[i] = current->ptr[i];

		int i = 0, j;
		while (x.compare(current->key[i]) > 0 && i < bucketSize)
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
string bptree::search(string x) {
	if (root == NULL)
		return NULL;
	else {
		node* current = root;
        // find the leaf node
		while (current->isLeaf == false) {
			for (int i = 0; i < current->size; i++) {
				if (x.compare(current->key[i]) < 0) {
					current = current->ptr[i];
					break;
				}
				if (i == current->size - 1) {
					current = current->ptr[i + 1];
					break;
				}
			}
		}

		return current->key[0];
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

int main() {
	bptree sid;
    bptree cid;

    // read file - sid
    FILE *fp;
    fp = fopen("SID.txt", "r");
    while (!feof(fp)) {
        char temp[11];
        fscanf(fp, "%s", temp);
        cout << temp << endl;
        sid.insert(temp);
    }


    string input;
    cin >> input;


	return 0;
}

