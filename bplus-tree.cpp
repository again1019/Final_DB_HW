#include<bits/stdc++.h>

using namespace std;

#define ORDER 5 

// 以 student id 為 key
class sid {
    public:
        string id;
        vector<int> course;
};

class node_sid {
    private:
        node_sid* parent;
    
    public:
        bool is_leaf;
        sid key[ORDER - 1];
        node_sid* child[ORDER];
        int key_num;
};

class bptree_sid {
    private:
        node_sid* root;
    
    public:
        bptree_sid();
        void insert(sid* s);
        sid* search(string id);
        void print_sid();
};

// 以 course id 為 key
class cid {
    public:
        string id;
        vector<int> student;
};

class node_cid {
    private:
        node_cid* parent;
    
    public:
        bool is_leaf;
        cid key[ORDER - 1];
        node_cid* child[ORDER];
        int key_num;
};

class bptree_cid {
    private:
        node_cid* root;
    
    public:
        bptree_cid();
        void insert(cid* c);
        cid* search(string id);
        void print_cid();
};

int main(){
   
    return 0;
}
