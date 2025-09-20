#ifndef NODE_TREE_H
#define NODE_TREE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {ROOT, EXPRESSION, FUNCTION, LEAF_PERVASIVE, LEAF_MONADIC_MODIFIER} symbol_t;

typedef struct Node Node;

struct Node {
    char* token;            // the token
    symbol_t symbol_type;
    unsigned int is_subscripted;
    int subscript;
    unsigned int number_children_nodes;
    Node** children_nodes;
    Node* parent_node;
};

char* tree_structure_to_string(const Node root_node);

void setparent_right(Node* node, Node* parent);
void setparent_left(Node* node, Node* parent);
void reverse_children(Node* node);

Node* new_node(const symbol_t symbol_type, const char* token_label);

#endif