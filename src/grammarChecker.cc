#include "grammarChecker.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <istream>
#include <string>
#include <deque>
#include <unordered_set>
#include <map>

using namespace std;

GrammarChecker::GrammarChecker()
{
}

void GrammarChecker::getGrammar()
{
    parseProg();
}

void GrammarChecker::parseProg()
{
    parseGlbVars();

    parseScope();
    varAnalyzer();
}

void GrammarChecker::varAnalyzer()
{
    for (const auto &glb : glbVars)
    {
        bool isFound = false;
        for (const auto &data : statementArray)
        {

            if (data.first == glb)
            {
                isFound = true;

                // lets find if it's in scope of anythig
                bool found_glb = false;
                for (const auto &g : glbVars)
                {
                    if (g == data.second)
                    {
                        found_glb = true;
                    }
                }
                if (found_glb)
                {
                    std::cout << "::" << glb << "=" << "::" << data.second << "\n";
                }
                bool found_in_scope = false;
                for (const auto &g : scopedVars)
                {
                    std::string found;
                    for (const auto &scope : g.second)
                    {
                        if (scope == data.second)
                        {
                            found += scope;
                            found_in_scope = true;
                        }
                    }
                    if (found_in_scope)
                    {
                        std::cout << "::" << glb << "=" << g.first << "." << found << "\n";
                    }
                }
                if (not found_in_scope and not found_glb)
                {
                    std::cout << "::" << glb << "=?." << data.second << std::endl;
                }
            }
        }
    }

    // for (const auto &)
}

void GrammarChecker::parseGlbVars()
{
    bool sucess = false;
    if (peekTop().second == "ID")
    {
        sucess = parseVarsList();
        if (not sucess)
        {
            return;
        }
        auto semicolon = getToken();
        tempStorage.push_front(semicolon);
        if (semicolon.second == "SEMICOLON")
        {
            // //std::cout << "Printing out the glb variables\n";
            // std::for_each(tempStorage.rbegin(), tempStorage.rend(), [&](const auto &iter)
            //               { //std::cout << "(" << iter.first << ',' << iter.second << "), "; });

            for (const auto &iter : tempStorage)
            {
                if (iter.second == "ID")
                {                               // getting all global variables
                    glbVars.insert(iter.first); // this is a hash set we dont want to do double the work
                }
            }
            tempStorage.clear();
        }
        else
        {
            returnTokens();
            std::cout << "Syntax Error\n";
        }
    }
}

bool GrammarChecker::parseVarsList(std::string scope)
{
    bool success = false;
    auto id = getToken();
    tempStorage.push_front(id);

    if (id.second == "ID")
    {
        if (peekTop().second == "SEMICOLON")
        {
            if (scope != "")
            {
                scopedVars[scope].push_back(id.first);
            }
            return true;
        }

        if (peekTop().second != "COMMA")
        {
            returnToken(id);
        }

        while (peekTop().second == "COMMA" and peekTop().second != "SEMICOLON")
        {
            auto comma = getToken();
            auto id = getToken();

            tempStorage.push_front(comma);
            tempStorage.push_front(id);
            if (id.second == "ID")
            {
                success = true;
            }
            else
            {
                return true;
            }
        }
    }
    else
    {
        tempStorage.clear(); // we dont want anything on it
        returnToken(id);
        return success;
    }
    return success;
}

void GrammarChecker::parsePubVars(std::string scope)
{
    if (peekTop().second != "PUBLIC")
    {
        return;
    }

    auto pub = getToken();
    tempStorage.push_front(pub);

    auto colon = getToken();
    tempStorage.push_front(colon);

    if (colon.second != "COLON")
    {
        std::cout << "Syntax Error\n";
        returnTokens();
        return;
    }
    bool status = parseVarsList(scope);

    if (not status)
    {
        std::cout << "Syntax Error\n";
        returnTokens();
        return;
    }

    auto semicolon = getToken();
    tempStorage.push_front(semicolon);

    if (semicolon.second != "SEMICOLON")
    {
        std::cout << "Syntax Error\n";
        returnTokens();
        return;
    }

    // std::for_each(tempStorage.rbegin(), tempStorage.rend(), [&](const auto &iter)
    //   { //std::cout << "(" << iter.first << ',' << iter.second << "), "; });
}

void GrammarChecker::parsePrivVars()
{
    if (peekTop().second != "PRIVATE")
    {
        return;
    }

    auto priv = getToken();
    tempStorage.push_front(priv);

    auto colon = getToken();
    tempStorage.push_front(colon);

    if (colon.second != "COLON")
    {
        std::cout << "Syntax Error\n";
        returnTokens();
        return;
    }
    bool status = parseVarsList();

    if (not status)
    {
        std::cout << "Syntax Error\n";
        returnTokens();
        return;
    }

    auto semicolon = getToken();
    tempStorage.push_front(semicolon);

    if (semicolon.second != "SEMICOLON")
    {
        std::cout << "Syntas Error\n";
        returnTokens();
        return;
    }
    // std::for_each(tempStorage.rbegin(), tempStorage.rend(), [&](const auto &iter)
    //               { //std::cout << "(" << iter.first << ',' << iter.second << "), "; });
}

void GrammarChecker::parseScope()
{
    if (peekTop().second != "ID")
    {
        return;
    }

    auto ID = getToken();
    tempStorage.push_front(ID);
    // we don't want the class ids
    std::vector<std::string> data;
    std::pair<std::string, std::vector<std::string>> scoped_pair = std::make_pair(ID.first, data);
    scopedVars.insert(scoped_pair); // creating vector

    if (peekTop().second != "LBRACE")
    {
        returnToken(ID);
        return;
    }
    auto LBRACE = getToken();
    tempStorage.push_front(LBRACE);

    // TODO!

    parsePubVars(ID.first);
    parsePrivVars();

    parseStmtList(ID.first);
    auto rbrace = getToken();

    if (rbrace.second == "RBRACE")
    {
        // //std::cout << "\n";
        // std::for_each(tempStorage.rbegin(), tempStorage.rend(), [&](const auto &iter)
        //               { //std::cout << "(" << iter.first << ',' << iter.second << "), "; });

        // for (const auto &iter : tempStorage)
        // {
        //     if (iter.second == "ID")
        //     {
        //         scopeVars.insert(iter.first);
        //     }
        // }
    }
    else
    {
        std::cout << "Syntax Error\n";
        return;
    }
}

// have a case statement that will go through here
void GrammarChecker::parseStmtList(std::string scope)
{
    while (peekTop().second != "RBRACE" and peekTop().second != "EOF")
    {
        parseStmt(scope);
    }
}

void GrammarChecker::parseStmt(std::string scope)
{
    if (peekTop().second == "ID" and peekQeue(1).second == "EQUAL")
    {
        // we will use scope
        auto ID = getToken();
        tempStorage.push_front(ID);

        if (ID.second != "ID")
        {
            returnToken(ID);
            return;
            // contine
        }

        auto EQUAL = getToken();
        tempStorage.push_front(EQUAL);

        if (EQUAL.second != "EQUAL")
        {
            returnToken(ID);
            returnToken(EQUAL);
            return;
        };

        auto ID2 = getToken();
        tempStorage.push_front(ID2);

        if (ID.second != "ID")
        {
            returnToken(ID);
            returnToken(EQUAL);
            returnToken(ID2);
            return;
            // contine
        }
        auto SEMICOLON = getToken();
        tempStorage.push_front(SEMICOLON);

        if (SEMICOLON.second != "SEMICOLON")
        {
            returnToken(ID);
            returnToken(EQUAL);
            returnToken(ID2);
            returnToken(SEMICOLON);
            cout << "Syntax Error\n";
            return;
        }
        std::pair<std::string, std::string> statement = std::make_pair(ID.first, ID2.first); // get both statements that are equal
        statementArray.push_front(statement);
        scopedVars[scope].push_back(ID.first);
    }

    else
    {
        parseScope();
    }
}