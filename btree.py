class Node(object):
    def __init__(self, data):
        self.data = data #根節點
        self.left = None 
        self.right = None

    def insert(self, data):
        #新增節點=根，False
        if self.data == data:
            return False
        #新增節點<根，新增左
        elif data < self.data:
            if self.left:
                return self.left.insert(data)
            else:
                self.left = Node(data)
                return True
        #新增節點>根，新增右
        else:
            if self.right:
                return self.right.insert(data)
            else:
                self.right = Node(data)
                return True

    def find(self, data):
        #新增節點=根，True
        if data == self.data:
            return True
        #新增節點<根
        elif data < self.data:
            if self.left:
                #左節點存在，繼續找
                return self.left.find(data)
            else:
                #無，False
                return False
        #新增節點>根
        else:
            #右節點存在，繼續找
            if self.right:
                return self.right.find(data)
            #無，False
            else:
                return False

    def in_order(self):
        if self:
            if self.left:
                self.left.in_order()
            print(str(self.data), end = ' ')
            if self.right:
                self.right.in_order()
            
class Tree(object):
    def __init__(self):
        self.root = None

    #新增節點
    def insert(self, data):
        #根存在，執行Node類別的insert()
        if self.root:
            return self.root.insert(data)
        #無，設根節點為新節點
        else:
            self.root = Node(data)
            return True

    #找節點
    def find(self, data):
        #根存在，執行Node類別的find()
        if self.root:
            return self.root.find(data)
        #無，False
        else:
            return False

    def in_order(self):
        print()
        if self.root is not None:
            print('In-order: ')
            self.root.in_order()

