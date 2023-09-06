#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* left; // greater then head
    struct node* right; // less then head
} node;

void add(node* head, int val) {
    if (val >= head->data) {
        if (head->left == NULL) {
            node* n = (node*) malloc(sizeof(node));
            n->data = val;
            n->left = NULL;
            n->right = NULL;
            head->left = n;
            return;
        }
        add(head->left, val);
    }
    else {
        if (head->right == NULL) {
            node* n = (node*) malloc(sizeof(node));
            n->data = val;
            n->left = NULL;
            n->right = NULL;
            head->right = n;
            return;
        }
        add(head->right, val);
    }
}

void print(node* head, int depth) {
    if (head == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf(" ");
    }
    printf("%d\n", head->data);
    print(head->left, depth+1);
    print(head->right, depth+1);
}

int main() {
    node* n = (node*) malloc(sizeof(node));
    n->data = 5;
    n->left = NULL;
    n->right = NULL;
    add(n, 69);
    add(n, 1);
    add(n, 70);
    add(n, 2);
    add(n, 3);
    print(n, 0);
}
