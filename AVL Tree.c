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
#define TEST    10

typedef Node*(*NodeFunction)(Node*, int);

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

int is_node_valid(Node *node){
    return 1 - (node == NULL);
}

int compare(int a, int b){
    return a*(a>b) + b*(b>a);
}

int is_greater(int a, int b){
    return a > b;
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

Node *counter_rotate(Node *node, int direction){
    node->child[1-direction] = rotate_node(node->child[1-direction], 1-direction);
    return rotate_node(node, direction);
}

Node *dummy(Node* node, int i){
    return node;
}

Node *insert_node(Node *node, int value){
    if(!is_node_valid(node))
        return new_node(value);
    //node->height = compare(node_height(node->left), node_height(node->right)) + 1;
    
    int direction = is_greater(value, node->value);
    
    node->child[direction] = insert_node(node->child[direction], value);

    int balance = node_balance(node);
    
    if(balance > 1){
        if(value > node->left->value)
            node->left = rotate_node(node->left, LEFT);
        return rotate_node(node, RIGHT);
    }
    /*int index = (balance > 1) + 2*(value > node->left->value);
    NodeFunction functions[4] = {dummy, rotate_node, dummy, counter_rotate};
    node = functions[index](node, 1);*/

    if(balance < -1){
        if(value < node->right->value)
            node->right = rotate_node(node->right, RIGHT);
        return rotate_node(node, LEFT);
    }
    
    return node;
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

void print_tree_inorder(Node *node){
    if(node != NULL){
        print_tree_inorder(node->left);
        printf("\n%d\t|\t%d", node->value, node->height);
        print_tree_inorder(node->right);
    }
}

void print_tree_preorder(Node *node){
    if(node != NULL){
        printf("\n%d", node->value);
        print_tree_preorder(node->left);
        print_tree_preorder(node->right);
    }
}

void print_tree_postorder(Node *node){
    if(node != NULL){
        print_tree_postorder(node->left);
        print_tree_postorder(node->right);
        printf("\n%d", node->value);
    }
}

int main(){
    srand(time(NULL));
    Node *root = NULL;
    for(int i = TEST; i > 0; i--)
        root = insert_node(root, rand()%(TEST*10));
    print_tree_inorder(root);
    /*printf("\n");
    print_tree_preorder(root);*/
    /*printf("\n");
    print_tree_postorder(root);*/
    delete_tree(root);
    return 0;
}
