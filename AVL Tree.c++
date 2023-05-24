#include <iostream>
#include <cmath>
#include<cstdlib>
#define LEFT    0    
#define RIGHT   1
#define left    getChild(LEFT)    
#define right   getChild(RIGHT)
#define LIMIT   20

using namespace std;

class Node {
    private:
        int value;
        Node *child[2];
        int height;
    public:
        Node(int value){
            this->value = value;
            this->child[LEFT] = NULL;
            this->child[RIGHT] = NULL;
            this->height = 1;
        };
        int getValue(){
            if(this == NULL)
                return -1;
            return this->value;
        };
        void setValue(int value){
            this->value = value;
        };
        Node* getChild(int direction){
            return this->child[direction];
        };
        void setChild(int direction, Node *node){
            this->child[direction] = node;
        }
        int getHeight(){
            if(this == NULL)
                return 0;
            return this->height;
        };
        void setHeight(int height){
            this->height = height;
        };
        int getBalance(){
            if(this == NULL)
                return 0;
            return (this->left->getHeight() - this->right->getHeight());
        };
        Node *newNode(int value) {
            Node *node = (Node*) malloc(sizeof(Node));
            node->setValue(value);
            node->child[LEFT] = NULL;
            node->child[RIGHT] = NULL;
            node->height = 1;
            return (node);
        };
        Node *rotate(int direction){
            Node *aux = this->child[1-direction];
            Node *temp = aux->child[direction];
    
            aux->child[direction] = this;
            this->child[1-direction] = temp;
    
            this->height = 1 + max(this->left->height, this->right->height);
            aux->height = 1 + max(aux->left->height, aux->right->height);
    
            return aux;
        };
        Node *append(int value) {
            if(this == NULL)
                return newNode(value);
    
            // if(this->getValue() == value)
            //     return this;
            
            // int direction = value > this->value;
            // this->child[direction] = this->child[direction]->append(value);
            
            if(value < this->getValue())
                this->child[LEFT] = this->child[LEFT]->append(value);
            else if(value > this->value)
                this->child[RIGHT] = this->child[RIGHT]->append(value);
            else
                return this;
            
            this->setHeight(1 + max(this->left->getHeight(), this->right->getHeight()));

            int balance = this->getBalance();
            //cout<<this->getValue()<<"|"<<balance<<endl;
    
            // if(balance > 1){
            //     if(value > this->left->value)
            //         this->setChild(LEFT, this->left->rotate(LEFT));
            //     return this->rotate(RIGHT);
            // }

            // if(balance < -1){
            //     if(value < this->right->getValue())
            //         this->setChild(RIGHT, this->right->rotate(RIGHT));
            //     return this->rotate(LEFT);
            // }
            
            if(balance > 1){
                if(value < this->left->getValue()){
                    return this->rotate(RIGHT);
                }else if (value > this->left->value){
                    this->setChild(LEFT, this->left->rotate(LEFT));
                return this->rotate(RIGHT);
                }
            }
            if (balance < -1) {
                if (value > this->right->getValue()) {
                    return this->rotate(LEFT);
                } else if (value < this->right->getValue()) {
                    this->setChild(RIGHT, this->right->rotate(RIGHT));
                return this->rotate(LEFT);
                }       
            }
    
            return new Node(value);
        };
};

int is_node_valid(Node *node){
    return 1 - (node == NULL);
}

// Node with minimum value
Node *nodeWithMimumValue(Node *node) {
  Node *current = node;
  while (current->left != NULL)
    current = current->left;
  return current;
}

// Delete a node
Node *deleteNode(Node *root, int value) {
  // Find the node and delete it
    if (root == NULL)
        return root;
    if (value < root->getValue())
        // root->child[LEFT] = deleteNode(root->left, value);
        root->setChild(LEFT, deleteNode(root->left, value));
    else if (value > root->getValue())
        // root->child[RIGHT] = deleteNode(root->right, value);
        root->setChild(RIGHT, deleteNode(root->right, value));
    else{
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            Node *temp = nodeWithMimumValue(root->right);
            root->setValue(temp->getValue());
            // root->child[RIGHT] = deleteNode(root->right, temp->getValue());
            root->setChild(RIGHT, deleteNode(root->right, temp->getValue()));
        }
    }

    if (root == NULL)
        return root;

  // Update the balance factor of each node and
  // balance the tree
    root->setHeight(1 + max(root->left->getHeight(), root->right->getHeight()));
    int balanceFactor = root->getBalance();
    if (balanceFactor > 1) {
        if (root->left->getBalance() >= 0)
            return root->rotate(RIGHT);
        else {
            // root->child[LEFT] = rotateNode(root->left, LEFT);
            root->setChild(LEFT, root->left->rotate(LEFT));
            return root->rotate(RIGHT);
        }
    }
    if (balanceFactor < -1) {
        if (root->right->getBalance() <= 0)
            return root->rotate(LEFT);
        else {
            // root->child[RIGHT] = rotateNode(root->right, RIGHT);
            root->setChild(RIGHT, root->right->rotate(RIGHT));
            return root->rotate(LEFT);
        }
    }
    
    return root;
}

// Print the tree
void printTree(Node *root, string indent, bool last) {
  if (root != nullptr) {
    cout << indent;
    if (last) {
      cout << "R----";
      indent += "   ";
    } else {
      cout << "L----";
      indent += "|  ";
    }
    cout << root->getValue() << endl;
    printTree(root->left, indent, false);
    printTree(root->right, indent, true);
  }
}

void printInOrder(Node* node){
    if(node != NULL){
        printInOrder(node->left);
        cout << node->getValue() << endl;
        printInOrder(node->left);
    }
}

int main() {
    srand((unsigned) time(NULL));
    Node *root = new Node(rand()%(10*LIMIT));
    // printTree(root, "", true);
    for(int i = 0; i < LIMIT; i++){
        root->append(rand()%(LIMIT*10));
    }
    printTree(root, "", true);
    //printInOrder(root);
    //root = deleteNode(root, 13);
    // << "After deleting " << endl;
}
