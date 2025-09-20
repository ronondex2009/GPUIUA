#include "parser.h"

// CONSTANTS //
const char pervasives[]        = "¬±¯⌵√ₑ∿⌊⌈⁅=≠<≤>≥+-×÷◿ⁿ↧↥∠"; // array of all pervasive symbols
const char monadic_modifiers[] = "⍥⟜⊸⤙⤚◡⋅⊙";                  // array of all monadic modifier symbols

// GLOBALS //
jmp_buf err;        // parse may except, jumps back to parse() with this buffer.

// FUNCTIONS //

// move cursor forward & return true if next character is in acceptable; else return false
bool accept(const char* acceptable_characters, char** tokens, int* cursor, int num_tokens) {
    if(*cursor >= num_tokens - 1) {return false;} // if next token is out of array, always return false
    if (strstr(acceptable_characters, tokens[*cursor + 1]) != NULL) {
        *cursor += 1;
        return true;
    }
    return false;
}

// accept but throws error if next character isnt acceptable; doesnt return bool. Either returns or longjmps to parse.
void expect(const char* acceptable_characters, char** tokens, int* cursor, int num_tokens) {
    bool accepted = accept(acceptable_characters, tokens, cursor, num_tokens);
    if(accepted == false) {
        printf("Expected any of following: %s\nat character %d; got %s", acceptable_characters, *cursor, tokens[*cursor]);
        longjmp(err, EXPECT_ERR);
    }
}

// tokenize input into UTF-8 strings (2-5 bytes each, null terminated)
// output_tokens should have enough memory allocated
void tokenize(uint num_entries, char* input_string, char** output_tokens, int* num_tokens) {
    char* head = input_string;
    *num_tokens = 0;
    for(int i = 0; i < num_entries && *head != '\0'; i++) {
        printf("loop: %d\n", i);
        // figure out number of bytes in this utf-8
        uint size = 0;
        if ((*head & 0x80) == 0x00) {size = 1;} // 0xxx xxxx
        if ((*head & 0xE0) == 0xC0) {size = 2;} // 110x xxxx
        if ((*head & 0xF0) == 0xE0) {size = 3;} // 1110 xxxx
        if ((*head & 0xF8) == 0xF0) {size = 4;} // 1111 0xxx
        if (size == 0) {longjmp(err, INVALID_UTF);}
        // allocate appropriate size to token and construct string there
        output_tokens[i] = (char*)malloc(sizeof(char) * (size + 1));
        for(int j = 0; j < size; j++) {output_tokens[i][j] = head[j];}
        output_tokens[i][size] = '\0'; // null terminate
        printf("%s\n", output_tokens[i]);
        // increment values
        head += size;
        *num_tokens += 1;
        // skip remaining whitespace
        while (*head == ' ' || *head == '\n' || *head == '\r') {head++;}
    }
}

// modifier := monmodifiers (subscript)?
bool _parse_modifier(char** tokens, int* cursor, int num_tokens, Node* parent_node) {
    // TODO: add
    return false;
}

// function := '(' root ')' | Pervasive | literal
bool _parse_function(char** tokens, int* cursor, int num_tokens, Node* parent_node) {
    if(accept("(", tokens, cursor, num_tokens)) 
    { 
        // inline
        puts("parsing function -> ( root )");
        Node* my_node = _parse_root(tokens, cursor, num_tokens);
        setparent_left(my_node, parent_node);
        expect(")", tokens, cursor, num_tokens);
        return true;
    } 
    else if (accept(pervasives, tokens, cursor, num_tokens)) 
    { 
        // pervasive
        puts("parsing function -> pervasive [LEAF]");
        Node* my_node = new_node(LEAF_PERVASIVE, tokens[*cursor]);
        setparent_left(my_node, parent_node);
        return true;
    } 
    else 
    { 
        // literal
        puts("expression <- function [backtrack]");
        return false; // we dont have number literals yet
    }
}

// expression := modifier expression | function
bool _parse_expression(char** tokens, int* cursor, int num_tokens, Node* parent_node) {
    Node* my_node = new_node(EXPRESSION, "<EXPRESSION>"); 
    setparent_left(my_node, parent_node);
    bool succ;
    if(_parse_modifier(tokens, cursor, num_tokens, my_node)) {
        puts("parsing expression -> modifier expression");
        succ = _parse_expression(tokens, cursor, num_tokens, my_node); 
        reverse_children(my_node); // modifier has to be first
    } else {
        puts("parsing expression -> function");
        succ = _parse_function(tokens, cursor, num_tokens, my_node);
    }
    return succ;
}

// root := expression+
Node* _parse_root(char** tokens, int* cursor, int num_tokens) {
    puts("parsing root");
    Node* parse_tree = new_node(ROOT, "<ROOT>");
    bool cont = true;
    while(cont) {cont = _parse_expression(tokens, cursor, num_tokens, parse_tree);} // expression+
    return parse_tree;
}

// enter parse, use this.
Node* parse(char* input) {
    // tokenize
    char** tokens;              // array of strings (char = token)
    int num_tokens;            // number of tokens
    tokens = (char**)calloc(sizeof(char*), 64);
    int cursor = -1;            // cursor; zero indexed

    // error handling
    error_t type_of_error = setjmp(err);
    if(type_of_error != NO_ERROR) {
        puts("parse(): something crashed");
        if(type_of_error == EXPECT_ERR) {puts("parser expected a token but didnt get it (did you forget closing paren or do you have a floating modifier?)");}
        if(type_of_error == INVALID_UTF) {puts("tokenizer got invalid UTF-8 text (is the input broken?)");}
        printf("error_t: %d\ncursor position: %d\n", type_of_error, cursor);
        return (Node*)NULL;
    }

    // tokenize && parse
    tokenize(64, input, tokens, &num_tokens);
    Node* parse_tree = _parse_root(tokens, &cursor, num_tokens);

    return parse_tree;
}

// ⍥(ⁿ2) 10

int main(int arg, char* args[]) {
    
    Node* root = parse("+(-×+-(+-×(÷))(××÷)×=)");
    

    puts("made it past parsing");

    printf("%s\n", tree_structure_to_string(*root));

    return 1;
}