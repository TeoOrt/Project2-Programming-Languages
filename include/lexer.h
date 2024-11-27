/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include "grammarChecker.h"
#include "inputbuf.h"
#include <string>
#include <vector>

// ------- token types -------------------

static GrammarChecker grammar_checker;

typedef enum
{
  END_OF_FILE = 0,
  PUBLIC,
  PRIVATE,
  EQUAL,
  COLON,
  COMMA,
  SEMICOLON,
  LBRACE,
  RBRACE,
  ID,
  ERROR,
  COMMENT
} TokenType;

class Token
{
public:
  void Print();

  std::string lexeme;
  TokenType token_type;
  int line_no;
};

class LexicalAnalyzer
{
public:
  Token GetToken();
  TokenType UngetToken(Token);
  LexicalAnalyzer();

private:
  std::vector<Token> tokens;
  int line_no;
  Token tmp;
  InputBuffer input;

  bool SkipSpace();
  bool IsKeyword(std::string);
  TokenType FindKeywordIndex(std::string);
  Token ScanIdOrKeyword();
  Token GetComment();
  // Token ScanNumber();
};

#endif //__LEXER__H__
