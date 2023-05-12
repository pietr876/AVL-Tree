#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node{
    int value;
    struct node *child[2];
    int height;
}Node;

#define LEFT    0
#define RIGHT   1
#define left    child[LEFT]
#define right   child[RIGHT]
#define TEST    1000

Node *new_node(int value){
    Node *node = (Node *) malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;

    return (node);
}

int node_height(Node *n){
    if(n == NULL)
        return 0;
    return n->height;
}

int node_balance(Node *n){
    if(n == NULL)
        return 0;
    return node_height(n->left) - node_height(n->right);
}

int compare(int a, int b){
    return a*(a>b) + b*(b>a);
}

Node *rotate_node(Node *n, int direction){
    Node *aux = n->child[1-direction];
    Node *temp = aux->child[direction];

    aux->child[direction] = n;
    n->child[1-direction] = temp;

    aux->height = compare(node_height(aux->left), node_height(aux->right)) + 1;
    n->height = compare(node_height(n->left), node_height(n->right)) + 1;

    free(temp);

    return aux;
}

Node *insert_node(Node *n, int value){
    if(n == NULL)
        return new_node(value);
    
    //==============================================================//
    
    int direction = compare(value, n->value) == value ? RIGHT : LEFT;
    
    n->child[direction] = insert_node(n->child[direction], value);    
    n->height = compare(node_height(n->left), node_height(n->right)) + 1;
    
    int balance = node_balance(n);
    
    if(balance > 1 || balance < -1){
        /***/
    }
    //==============================================================//

    /*if(value < n->value)
        n->left = insert_node(n->left, value);
    else if(value > n->value)
        n->right = insert_node(n->right, value);
    else
        return n;

    int balance = node_balance(n);

    if(balance > 1){
        if(value > n->left->value)
            n->left = rotate_node(n->left, LEFT);
        return rotate_node(n, RIGHT);
    }

    if(balance < -1){
        if(value < n->right->value)
            n->right = rotate_node(n->right, RIGHT);
        return rotate_node(n, LEFT);
    }*/

    return n;
}

Node *min_value_node(Node *n) {
  Node *current = n;

  while (current->left != NULL)
    current = current->left;

  return current;
}

Node *delete_node(Node *n, int value){
    if(n == NULL)
        return n;

    if(value < n->value)
        n->left = delete_node(n->left, value);
    else if(value > n->value)
        n->right = delete_node(n->right, value);
    else{
        if(n->left == NULL || n->right == NULL){
            Node *temp = n->left ? n->left : n->right;

            if (temp == NULL) {
                temp = n;
                n = NULL;
            }else
                *n = *temp;
            free(temp);
        }else{
            Node *temp = min_value_node(n->right);

            n->value = temp->value;

            n->right = delete_node(n->right, temp->value);
        }
    }

    if(n == NULL)
        return n;

    n->height = compare(node_height(n->left), node_height(n->right)) + 1;

    int balance = node_balance(n);

    if(balance > 1){
        if(node_balance(n->left) < 0)
            n->left = rotate_node(n->left, LEFT);
        return rotate_node(n, RIGHT);
    }

    if(balance < -1){
        if(node_balance(n->right) > 0)
            n->right = rotate_node(n->right, RIGHT);
        return rotate_node(n, LEFT);
    }

    return n;
}

void delete_tree(Node *n){
    if(n == NULL)
        return;
    delete_tree(n->left);
    delete_tree(n->right);
    free(n);
}

void print_tree_inorder(Node *n){
    if(n != NULL){
        print_tree_inorder(n->left);
        printf("\n%d", n->value);
        print_tree_inorder(n->right);
    }
}

void print_tree_preorder(Node *n){
    if(n != NULL){
        printf("\n%d", n->value);
        print_tree_preorder(n->left);
        print_tree_preorder(n->right);
    }
}

void print_tree_postorder(Node *n){
    if(n != NULL){
        print_tree_postorder(n->left);
        print_tree_postorder(n->right);
        printf("\n%d", n->value);
    }
}

int main(){
    srand(time(NULL));
    Node *root = NULL;
    for(int i = TEST; i > 0; i--)
        root = insert_node(root, rand());
    print_tree_inorder(root);
    delete_tree(root);
    return 0;
}
