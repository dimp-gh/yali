#ifndef LEXER_H
#define LEXER_H

enum Token_type {
  token_open_paren,
  token_close_paren,
  token_id,
  token_integer,
  token_float,
  token_bool,
  token_nil
};

typedef struct {
  enum Token_type type;
  char *strval;
} Token;

typedef struct token_el {
  Token *token;
  struct token_el *next;
} Token_node;


enum Lex_state {
  lex_in_comment,
  lex_reading,
  lex_in_token
};
  

Token_node *lex(char *text);
void _print_tokens(Token_node *start);

#endif
