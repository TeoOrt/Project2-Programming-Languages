/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

std::string reserved[] = {"END_OF_FILE",
                          "PUBLIC",
                          "PRIVATE",
                          "EQUAL",
                          "COLON",
                          "COMMA",
                          "SEMICOLON",
                          "LBRACE",
                          "RBRACE",
                          "ID",
                          "ERROR", "NOTHING"};

#define KEYWORDS_COUNT 2
std::string keyword[] = {"public", "private"}; // should probably use a hash set if you plan on adding more keywords
// hashsets are O(1)
void Token::Print()
{
    if (this->token_type == COMMENT)
    {
        return;
    }

    grammar_checker.insertToken(this->lexeme, reserved[(int)this->token_type]);

    // cout
    //     << "{" << this->lexeme << " , "
    //     << reserved[(int)this->token_type] << " , "
    //     << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c))
    {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput())
    {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(std::string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++)
    {
        if (s == keyword[i])
        {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(std::string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++)
    {
        if (s == keyword[i])
        {
            return (TokenType)(i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c))
    {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c))
        {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    }
    else
    {
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);
    ;
    return tok.token_type;
}

Token LexicalAnalyzer::GetComment()
{

    char c;

    while (!input.EndOfInput() and c != '\n')
    {
        // cout << tmp.lexeme;
        tmp.lexeme += c;
        input.GetChar(c);
    }
    return tmp;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty())
    {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    tmp.lexeme += c;
    auto hey = input.getBuffer();
    switch (c)
    {
    case '=':
        tmp.token_type = EQUAL;
        return tmp;
    case ':':
        tmp.token_type = COLON;
        return tmp;
    case ',':
        tmp.token_type = COMMA;
        return tmp;
    case ';':
        tmp.token_type = SEMICOLON;
        return tmp;
    case '{':
        tmp.token_type = LBRACE;
        return tmp;
    case '}':
        tmp.token_type = RBRACE;
        for (const auto &data : hey)
        {
            cout << data;
        }
        return tmp;
    case '/':
        tmp.lexeme += c;
        input.GetChar(c);
        if (c == '/')
        {
            tmp.token_type = COMMENT;
            GetComment();
            return tmp;
        }
        tmp.token_type = ERROR;
        return tmp;
    // since it's comment we don't want to look at it
    default:
        if (isdigit(c))
        {
            input.UngetChar(c);
            // return ScanNumber();
        }
        else if (isalpha(c))
        {
            input.UngetChar(c);
            return ScanIdOrKeyword();
        }
        else if (input.EndOfInput())
            tmp.token_type = END_OF_FILE;
        else
            tmp.token_type = ERROR;

        return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    int i = 0;
    while (token.token_type != END_OF_FILE)
    {
        // if (i <= 10)
        // {
        //     std::cout << "\n";
        // }
        token = lexer.GetToken();
        token.Print();
        i++;
    }

    grammar_checker.getGrammar();

    // std::cout << "Finished\n";
}
