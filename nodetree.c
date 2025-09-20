#include "nodetree.h"

const int BUFFER_SIZE = 1024; // <-- nothing bad could possibly happen!

// replace substring (string must have enough space in its buffer)
void substring_replace(char* string, const char* substring, const char* replacement) {
    size_t replacement_length = strlen(replacement);
    size_t substring_length = strlen(substring);
    char* substring_source = string; // substring_source is pointer; moves to next substring
    while(1) {
        substring_source = strstr(substring_source, substring); // move to next substring
        if (substring_source == NULL) {return;}                 // return if no next substring
        memmove(substring_source + replacement_length,          // shift everything right of substring left/right
            substring_source + substring_length,                // ...
            strlen(substring_source) - substring_length + 1     // ...
        );

        memcpy(substring_source, replacement, replacement_length); // copy replacement string into place where substring was

        substring_source += replacement_length;                    // move head right so we dont hit the replaced substring incase replacement string has substring in it
    }
}

// sets char* str to the string tree representation of the node tree starting at Node root_node
char* tree_structure_to_string(const Node root_node) {
    // uhh guhh uhmm uhhh
    char* str = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    snprintf(str, 
        BUFFER_SIZE, 
        "\nNODE:\n    %s\n    Type: %d",
        root_node.token,
        root_node.symbol_type
    );
    // add each child onto the branch
    unsigned int num_children = root_node.number_children_nodes;
    for(unsigned int i = 0; i < num_children; i++) {
        char* temp_buffer = tree_structure_to_string(*root_node.children_nodes[i]);
        substring_replace(temp_buffer, "\n", "\n| ");
        strcat(str, temp_buffer);
    }
    return str;
}

void setparent(Node* node, Node* parent) {
    // god isn't smiling down upon me as I write these functions
    parent->children_nodes = (Node**)realloc(parent->children_nodes, sizeof(Node*) * ++parent->number_children_nodes);
    parent->children_nodes[parent->number_children_nodes - 1] = node;
    node->parent_node = parent;
}

Node new_node(const symbol_t symbol_type, const char* token_label) {
    Node node;
    node.symbol_type = symbol_type;
    node.token = (char*)malloc(sizeof(char) * strlen(token_label));
    memcpy(node.token, token_label, strlen(token_label));
    node.children_nodes = malloc(sizeof(Node*));
    node.number_children_nodes = 0;
    return node;
}
