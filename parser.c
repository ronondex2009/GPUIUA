#include "parser.h"

// CONSTANTS //
const char pervasives[]        = "¬±¯⌵√ₑ∿⌊⌈⁅=≠<≤>≥+-×÷◿ⁿ↧↥∠"; // array of all pervasive symbols
const char monadic_modifiers[] = "⍥⟜⊸⤙⤚◡⋅⊙";                  // array of all monadic modifier symbols

// GLOBALS //
jmp_buf err;        // parse may except, jumps back to parse() with this buffer.

// FUNCTIONS //

// move cursor forward & return true if next character is in acceptable; else return false
bool accept(char* acceptable_characters, char** tokens, int* cursor, uint num_tokens) {
    if (strstr(acceptable_characters, tokens[*cursor]) != NULL) {
        *cursor += 1;
        if (*cursor >= num_tokens) {longjmp(err, EARLY_EOF);}
        return true;
    }
    return false;
}

// accept but throws error if next character isnt acceptable; doesnt return bool. Either returns or longjmps to parse.
void expect(char* acceptable_characters, char** tokens, int* cursor, uint num_tokens) {
    bool accepted = accept(acceptable_characters, tokens, cursor, num_tokens);
    if(accepted == false) {
        printf("Expected any of following: %s\nat character %d; got %s", acceptable_characters, *cursor, tokens[*cursor]);
        longjmp(err, EXPECT_ERR);
    }
}

// tokenize input into UTF-8 strings (2-5 bytes each, null terminated)
// output_tokens should have enough memory allocated
void tokenize(uint num_entries, char* input_string, char** output_tokens, uint* num_tokens) {
    char* head = input_string;
    for(int i = 0; i < num_entries && *head != '\0'; i++) {
        // leave out whitespace
        if (*head == ' ' || *head == '\n' || *head == '\r') {head++; continue;}
        printf("loop: %d", i);
        // figure out number of bytes in this utf-8
        uint size = 0;
        if ((*head & 0x80) == 0x00) {size = 1;} // 0xxx xxxx
        if ((*head & 0xE0) == 0xC0) {size = 2;} // 110x xxxxx
        if ((*head & 0xF0) == 0xE0) {size = 3;} // 1110 xxxxx
        if ((*head & 0xF8) == 0xF0) {size = 4;} // 1111 0xxxx
        if (size == 0) {longjmp(err, INVALID_UTF);}
        // allocate appropriate size to token and construct string there
        output_tokens[i] = (char*)malloc(sizeof(char) * (size + 1));
        for(int j = 0; j < size; j++) {output_tokens[i][j] = head[j];}
        output_tokens[i][size] = '\0'; // null terminate
        // increment values
        head += size;
        *num_tokens++;
    }
}

// root       := expression+                        <-- PARSE ENTERS HERE
// expression := modifier <expression> | function
// function   := '(' expression+ <')'> | Pervasive | literal
// modifier   := monmodifiers (subscript)?

bool _parse_modifier(char** tokens, int* cursor, uint num_tokens, Node* parent_node) {
    // modifier := monmodifiers (subscript)?
    // TODO: add
}

bool _parse_function(char** tokens, int* cursor, uint num_tokens, Node* parent_node) {
    // function := '(' root ')' | Pervasive | literal
    // TODO: add
}

bool _parse_expression(char** tokens, int* cursor, uint num_tokens, Node* parent_node) {
    // expression := modifier expression | function
    // TODO: add
    Node my_node; 
}

Node _parse_root(char** tokens, int* cursor, uint num_tokens) {
    // root := expression+
    // TODO: replace the node declarations in here to use proper ones defined in nodetree.h
    Node my_node; my_node.symbol_type = ROOT; // make ROOT node
    bool cont = true;
    while(cont) {cont = _parse_expression(tokens, cursor, num_tokens, &my_node);} // expression+
    return *cursor == num_tokens - 1; // return true if the cursor ends up on the last token
}

// enter parse, use this.
Node parse(char* input) {
    // error handling
    if (setjmp(err) != NO_ERROR) {
        printf("something crashed\n");
        return;
    }
    
    // tokenize
    char** tokens;      // array of strings (char = token)
    uint num_tokens;     // number of tokens
    tokens = (char**)calloc(sizeof(char*), 64);
    tokenize(64, "first×÷² test", tokens, &num_tokens);
    
    // parse
    int cursor = -1;    // cursor; zero indexed
    
}

// ⍥(ⁿ2) 10

int main(int arg, char* args[]) {
    
    Node node_a = new_node(ROOT, "test");
    Node node_b = new_node(LEAF_PERVASIVE, "yay");
    setparent(&node_b, &node_a);
    
    printf("\n%s\n", tree_structure_to_string(node_a));

    return 1;
}