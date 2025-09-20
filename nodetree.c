#include "nodetree.h"

// replace substring (string must have enough space in its buffer)
void substring_replace(char* string, const char* substring, const char* replacement) {
    size_t replacement_length = strlen(replacement);
    size_t substring_length = strlen(substring);
 
    char* substring_source = string; // substring_source is pointer; moves to next substring
    while(1) {
        substring_source = strstr(substring_source, substring);                      // move to next substring
        if (substring_source == NULL) {return;}                                      // return if no next substring
        memmove(substring_source + replacement_length,                               // shift everything right of substring left/right
            substring_source + substring_length,                                     // ...
            strlen(substring_source) - substring_length + 1                          // ...
        );

        memcpy(substring_source, replacement, replacement_length); // copy replacement string into place where substring was

        substring_source += replacement_length;                    // move head right so we dont hit the replaced substring incase replacement string has substring in it
    }
}

// sets char* str to the string tree representation of the node tree starting at Node root_node (heap allocated str)
char* tree_structure_to_string(const Node root_node) {
    // uhh guhh uhmm uhhh
    char* str = (char*)malloc(sizeof(char) * 64);
    snprintf(str, 
        64, 
        "\nNODE:\n %s\n Type: %d\n",
        root_node.token,
        root_node.symbol_type
    );
    // add each child onto the branch
    unsigned int num_children = root_node.number_children_nodes;
    for(unsigned int i = 0; i < num_children; i++) {
        char* temp_buffer = tree_structure_to_string(*root_node.children_nodes[i]);
        temp_buffer = (char*)realloc(temp_buffer, sizeof(char) * strlen(temp_buffer) * 2);
        substring_replace(temp_buffer, "\n", "\n| ");
        strcat(temp_buffer, "\n");
        str = (char*)realloc(str, strlen(str) + strlen(temp_buffer) + 2);
        strcat(str, temp_buffer);
    }
    return str;
}

// sets node as a child of parent node, to the right of all other nodes (node is executed after everyone else)
void setparent_right(Node* node, Node* parent) {
    // god isn't smiling down upon me as I write these functions
    parent->children_nodes = (Node**)realloc(parent->children_nodes, sizeof(Node*) * ++parent->number_children_nodes);
    parent->children_nodes[parent->number_children_nodes - 1] = node;
    node->parent_node = parent;
}

// sets node as a child of parent node, to the left of all other nodes (node is executed before everyone else)
void setparent_left(Node* node, Node* parent) {
    // god isn't smiling down upon me as I write these functions
    parent->children_nodes = (Node**)realloc(parent->children_nodes, sizeof(Node*) * ++parent->number_children_nodes);
    memcpy(parent->children_nodes + 1, parent->children_nodes, (parent->number_children_nodes - 1) * sizeof(Node*));
    parent->children_nodes[0] = node;
    node->parent_node = parent;
}

// reverses the order of the children of the node; things executed first are now executed last, and vice versa.
void reverse_children(Node* node) {
    unsigned int num_children = node->number_children_nodes;
    Node* buf[num_children * sizeof(Node*)];
    for(unsigned int i = 0; i < num_children; i++) {buf[num_children - i - 1] = node->children_nodes[i];}
    memcpy(node->children_nodes, buf, num_children);
}

// initialize a new node with malloced children array; token_label input is memcpy-ed to token attribute.
Node* new_node(const symbol_t symbol_type, const char* token_label) {
    Node* node = (Node*)calloc(sizeof(Node), 1);
    node->symbol_type = symbol_type;
    node->token = (char*)malloc(sizeof(char) * strlen(token_label));
    memcpy(node->token, token_label, strlen(token_label));
    node->children_nodes = (Node**)malloc(sizeof(Node*));
    node->number_children_nodes = 0;
    return node;
}
