
#ifndef GRAMMAR_CHECKER_H
#define GRAMMAR_CHECKER_H
// #pragma once
#include <vector>
#include <iostream>
#include <istream>
#include <string>
#include <deque>
#include <unordered_set>
#include <map>

namespace GrammarSpace
{
    constexpr auto def = std::make_pair("END_OF_FILE", "EOF");
    //   auto def = std::make_pair("END_OF_FILE", "EOF");
};

class GrammarChecker
{
public:
    GrammarChecker();

    void getGrammar();

    void insertToken(std::string lexeme, std::string token_type)
    {
        std::pair<std::string, std::string> pair = std::make_pair(lexeme, token_type);
        tokenArray.push_back(pair);
    }

private:
    void parseProg();
    void parseGlbVars();
    bool parseVarsList(std::string scope = "");
    void parseScope();
    void parsePubVars(std::string scope = "");
    void parsePrivVars();
    void parseStmtList(std::string scope);
    void parseStmt(std::string scope);
    // glb_vars

    inline std::pair<std::string, std::string> peekTop()
    {
        return *tokenArray.begin(); // return the pointer
    }

    inline std::pair<std::string, std::string> getToken()
    {
        auto def = GrammarSpace::def;
        if (not tokenArray.empty())
        {
            auto rtn = tokenArray.front();
            tokenArray.pop_front();
            return rtn;
        }
        return def;
    }

    inline const std::pair<std::string, std::string> peekQeue(const int index = 0)
    {
        if (index > tokenArray.size())
        {
            return GrammarSpace::def;
        }

        try
        {
            auto rtn = tokenArray.at(index);
            return rtn;
        }
        catch (const std::out_of_range &e)
        {
            return GrammarSpace::def;
        }
    }

    inline void returnToken(std::pair<std::string, std::string> pair)
    {
        tokenArray.push_front(pair);
    }

    inline void returnTokens()
    {
        for (const auto &pair : tempStorage)
        {
            tokenArray.push_front(pair);
        }
        tempStorage.clear();
    }

    void varAnalyzer();

    std::deque<std::pair<std::string, std::string>> tokenArray;

    std::deque<std::pair<std::string, std::string>> tempStorage;

    std::deque<std::pair<std::string, std::string>> statementArray;
    std::unordered_set<std::string> glbVars;
    // std::unordered_set<std::string> scopeVars;
    // std::unordered_set<std::string> scopeIDS;
    std::map<std::string, std::vector<std::string>> scopedVars;
};

#endif