/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"

// user defined headers
#include <vector>
#include <unordered_map>

using namespace std;

LexicalAnalyzer lexer; // global variable for lexer
vector<Token> tokens;  // vector to store tokens

vector<Token> terminals;    // vector to store terminals
vector<Token> nonTerminals; // vector to store non-terminals

vector<Token> generatingRules;  // vector to store generating rules
vector<Token> reachableSymbols; // vector to store reachable symbols
vector<int> reachableIndices;   // vector to store indices of reachable production rules

unordered_map<string, vector<string>> firstSets; // map to store first sets

unordered_map<string, vector<string>> followSets; // map to store follow sets

// search for a token in a vector and return its index if found; otherwise, return -1
int searchToken(vector<Token> tokensVec, Token token)
{
    for (int i = 0; i < tokensVec.size(); i++)
    {
        if (tokensVec[i].lexeme == token.lexeme)
        {
            return i;
        }
    }
    return -1;
}

// search for an index in a vector and return true if found; otherwise, return false
bool searchIndex(vector<int> vec, int index)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] == index)
        {
            return true;
        }
    }
    return false;
}

int searchFirstSet(vector<string> firstSetVec, string lexeme)
{
    for (int i = 0; i < firstSetVec.size(); i++)
    {
        if (firstSetVec[i] == lexeme)
        {
            return i;
        }
    }
    return -1;
}

// print tokens
void printTokens(vector<Token> tokensVec)
{
    for (int i = 0; i < tokensVec.size(); i++)
    {
        tokensVec[i].Print();
    }
}

// print token T1 format
void printTokensT1(vector<Token> tokensVec)
{
    for (int i = 0; i < tokensVec.size(); i++)
    {
        cout << tokensVec[i].lexeme << " ";
    }
}

// read grammar
void ReadGrammar()
{
    // read input from stdin and store tokens in global vector
    Token token = lexer.GetToken();
    while (token.token_type != HASH)
    {
        tokens.push_back(token);
        token = lexer.GetToken();
    }
    tokens.push_back(token);
    // cout << "0\n";

    // printTokens(tokens); // print tokens
}

// function where if an element exists in vec1 and vec2, it removes that element from vec1
void removeCommonElements(vector<Token> &vec1, vector<Token> vec2) // only vec1 is passed by reference because we want to modify it
{
    for (int i = 0; i < vec2.size(); i++)
    {
        int index = searchToken(vec1, vec2[i]);
        if (index != -1)
        {
            vec1.erase(vec1.begin() + index);
        }
    }
}

void storeTerminalsAndNonTerminals()
{
    // iterate over tokens and store terminals and non-terminals
    int i = 0;
    bool lhs = true;
    bool rhs = false;
    while (tokens[i].token_type != HASH)
    {
        while (tokens[i].token_type != STAR)
        {
            if (tokens[i].token_type == ARROW)
            {
                lhs = false;
                rhs = true;
                // i++;
                // continue;
            }
            else if (lhs)
            {
                if (searchToken(nonTerminals, tokens[i]) == -1)
                {
                    nonTerminals.push_back(tokens[i]);
                }
                if (searchToken(terminals, tokens[i]) != -1)
                {
                    terminals.erase(terminals.begin() + searchToken(terminals, tokens[i]));
                }
            }
            else if (rhs)
            {
                if (searchToken(nonTerminals, tokens[i]) == -1 && searchToken(terminals, tokens[i]) == -1)
                {
                    nonTerminals.push_back(tokens[i]);
                    terminals.push_back(tokens[i]);
                }
            }
            i++;
        }
        lhs = true;
        rhs = false;
        i++; // skip STAR
    }
    removeCommonElements(nonTerminals, terminals); // remove terminals from non-terminals
}

// Task 1
void printTerminalsAndNoneTerminals()
{
    storeTerminalsAndNonTerminals();
    // cout << "TERMINALS:\n";
    printTokensT1(terminals);
    // cout << "NON-TERMINALS:\n";
    printTokensT1(nonTerminals);
    // cout << "1\n";
}

void uselessSymbolsHelper()
{
    storeTerminalsAndNonTerminals();
    //  cout << "flag 0\n";
    //  printTokensT1(terminals);
    //  printTokensT1(nonTerminals);
    //   cout << "flag 1\n";
    // printTokens(tokens);
    vector<Token> generatingSymbols; // vector to store generating symbols
    int gsSize = -1;
    while (gsSize != generatingSymbols.size())
    {
        gsSize = generatingSymbols.size();
        // iterate over tokens and store generating symbols
        int i = 0;
        bool lhs = true;
        bool rhs = false;
        Token lhsToken;
        bool generating = true; // flag to check if a symbol is generating
        while (tokens[i].token_type != HASH)
        {
            while (tokens[i].token_type != STAR)
            {
                if (tokens[i].token_type == ARROW)
                {
                    lhs = false;
                    rhs = true;
                }
                else if (lhs)
                {
                    // store lhs non-terminal; if rhs is valid, then add it to generating symbols if it is not already there
                    lhsToken = tokens[i];
                }
                else if (rhs)
                {
                    if (searchToken(terminals, tokens[i]) == -1 && searchToken(generatingSymbols, tokens[i]) == -1)
                    {
                        generating = false;
                        // break;
                    }
                }
                i++;
            }
            if (generating && searchToken(generatingSymbols, lhsToken) == -1)
            {
                generatingSymbols.push_back(lhsToken);
            }
            lhs = true;
            rhs = false;
            generating = true;
            i++; // skip STAR
        }
    }
    // cout << "GENERATING SYMBOLS:\n";
    // printTokensT1(generatingSymbols);

    // remove production rules from tokens that have non-generating symbols. That means even if one symbol in the production rule is non-generating, then the whole production rule is non-generating, and we remove it. So, we store the indices of the production rules that are generating in a vector and just store those production rules in a new vector.
    if (generatingSymbols.size() == 0)
    {
        // cout << "NO GENERATING SYMBOLS\n";
        return;
    }

    vector<int> generatingIndices;
    int i = 0;
    bool lhs = true;
    bool rhs = false;
    int index = -1;
    bool generating = true; // flag to check if a symbol is generating
    while (tokens[i].token_type != HASH)
    {
        while (tokens[i].token_type != STAR)
        {
            if (tokens[i].token_type == ARROW)
            {
                lhs = false;
                rhs = true;
            }
            else if (lhs)
            {
                // store lhs non-terminal; if rhs is valid, then add it to generating symbols if it is not already there
                index = i;
                if (searchToken(terminals, tokens[i]) == -1 && searchToken(generatingSymbols, tokens[i]) == -1)
                {
                    generating = false;
                }
            }
            else if (rhs)
            {
                if (searchToken(terminals, tokens[i]) == -1 && searchToken(generatingSymbols, tokens[i]) == -1)
                {
                    generating = false;
                }
            }
            i++;
        }
        if (generating)
        {
            generatingIndices.push_back(index);
        }
        lhs = true;
        rhs = false;
        i++; // skip STAR
        generating = true;
    }

    if (generatingIndices.size() == 0)
    {
        // cout << "NO GENERATING INDICES\n";
        return;
    }

    for (int i = 0; i < generatingIndices.size(); i++)
    {
        int j = generatingIndices[i];
        while (tokens[j].token_type != STAR)
        {
            generatingRules.push_back(tokens[j]);
            j++;
        }
        generatingRules.push_back(tokens[j]); // add STAR
    }
    generatingRules.push_back(tokens[tokens.size() - 1]); // add HASH
    // cout << "GENERATING TOKENS:\n";
    // printTokens(generatingRules);
    Token mainGrammarStart = tokens[0];
    if (searchToken(generatingRules, mainGrammarStart) == -1)
    {
        // cout << "MAIN GRAMMAR START SYMBOL IS NOT GENERATING\n";
        return;
    }

    reachableSymbols.push_back(mainGrammarStart);

    bool rsLhs = true;
    bool rsRhs = false;
    bool lhsReachable = false;
    int rsSize = 0;
    int k = 0;
    while (rsSize != reachableSymbols.size())
    {
        rsSize = reachableSymbols.size();
        while (generatingRules[k].token_type != HASH)
        {
            while (generatingRules[k].token_type != STAR)
            {
                if (generatingRules[k].token_type == ARROW)
                {
                    rsLhs = false;
                    rsRhs = true;
                }
                else if (rsLhs)
                {
                    if (searchToken(reachableSymbols, generatingRules[k]) != -1)
                    {
                        lhsReachable = true;
                        if (searchIndex(reachableIndices, k) == false)
                        {
                            reachableIndices.push_back(k);
                        }
                    }
                    else
                    {
                        lhsReachable = false;
                    }
                }
                else if (rsRhs)
                {
                    if (lhsReachable && searchToken(nonTerminals, generatingRules[k]) != -1 && searchToken(reachableSymbols, generatingRules[k]) == -1)
                    {
                        reachableSymbols.push_back(generatingRules[k]);
                    }
                }
                k++;
            }
            k++; // skip STAR
            rsLhs = true;
            rsRhs = false;
        }
    }
    // cout << "REACHABLE SYMBOLS:\n";
    // printTokensT1(reachableSymbols);
    // cout << "REACHABLE INDICES:\n";
    // for (int i = 0; i < reachableIndices.size(); i++)
    // {
    //     cout << reachableIndices[i] << " ";
    // }
}

// Task 2
void RemoveUselessSymbols()
{
    uselessSymbolsHelper();
    // print production rules that are reachable
    for (int l = 0; l < reachableIndices.size(); l++)
    {
        int f = reachableIndices[l];
        while (generatingRules[f].token_type != STAR)
        {
            if (generatingRules[f].token_type == ARROW)
            {
                cout << "->" << " ";
                if (generatingRules[f + 1].token_type == STAR)
                {
                    cout << "#";
                }
            }
            else
            {
                cout << generatingRules[f].lexeme << " ";
            }
            f++;
        }
        cout << endl; // new line after each production rule
    }
}

// function to print first sets from an unordered_map
void printFirstSets(unordered_map<string, vector<string>> map)
{
    string firstSet = "";
    for (int i = 0; i < nonTerminals.size(); i++)
    {
        if (map[nonTerminals[i].lexeme].size() == 0)
        {
            firstSet += "FIRST(" + nonTerminals[i].lexeme + ") = {  }\n";
            cout << firstSet;
            firstSet = "";
            continue;
        }

        firstSet += "FIRST(" + nonTerminals[i].lexeme + ") = { ";
        if (searchFirstSet(map[nonTerminals[i].lexeme], "#") != -1)
        {
            firstSet += "#, ";
        }
        for (int j = 0; j < terminals.size(); j++)
        {
            if (searchFirstSet(map[nonTerminals[i].lexeme], terminals[j].lexeme) != -1)
            {
                firstSet += terminals[j].lexeme + ", ";
            }
        }
        if (firstSet.length() >= 2)
        {
            firstSet = firstSet.substr(0, firstSet.length() - 2);
        }
        firstSet += " }\n";
        cout << firstSet;
        firstSet = "";
    }
}

void calculateFirstSetsHelper()
{
    storeTerminalsAndNonTerminals();

    // initialize unordered_map with non-terminals, terminals, and epsilon, and their corresponding first sets
    for (int i = 0; i < nonTerminals.size(); i++)
    {
        vector<string> firstSet;
        firstSets[nonTerminals[i].lexeme] = firstSet;
    }
    for (int i = 0; i < terminals.size(); i++)
    {
        vector<string> firstSet;
        firstSet.push_back(terminals[i].lexeme);
        firstSets[terminals[i].lexeme] = firstSet;
    }
    vector<string> firstSetHashInit;
    firstSetHashInit.push_back("#");
    firstSets["#"] = firstSetHashInit;

    // printUnorderedMap(firstSets);

    // iterate over input cfg and calculate first sets
    bool changed = true;
    /*
     epsilon flag will toggle to false if epsilon is not in the first set of a non-terminal in a production rule. If epsilon is in the first set of a non-terminal, then we will check the next symbol's first set. If epsilon never toggles to false, then we add epsilon to the first set of the lhs non-terminal.
     */
    while (changed)
    {
        changed = false;
        int i = 0;
        bool lhs = true;
        bool rhs = false;
        bool epsilon = true;
        string lhsNonTerminal;
        while (tokens[i].token_type != HASH)
        {
            while (tokens[i].token_type != STAR)
            {
                if (tokens[i].token_type == ARROW)
                {
                    lhs = false;
                    rhs = true;
                    if (tokens[i + 1].token_type == STAR) // rhs is epsilon
                    {
                        if (searchFirstSet(firstSets[lhsNonTerminal], "#") == -1)
                        {
                            firstSets[lhsNonTerminal].push_back("#");
                            changed = true;
                        }
                    }
                }
                else if (lhs)
                {
                    lhsNonTerminal = tokens[i].lexeme;
                }
                else if (rhs)
                {
                    string currSymbol = tokens[i].lexeme;
                    for (int j = 0; j < firstSets[currSymbol].size(); j++)
                    {
                        if (searchFirstSet(firstSets[lhsNonTerminal], firstSets[currSymbol][j]) == -1 && firstSets[currSymbol][j] != "#")
                        {
                            firstSets[lhsNonTerminal].push_back(firstSets[currSymbol][j]);
                            changed = true;
                        }
                    }
                    if (searchFirstSet(firstSets[currSymbol], "#") == -1)
                    {
                        epsilon = false;
                    }
                }
                if (epsilon == false)
                {
                    while (tokens[i].token_type != STAR)
                    {
                        i++;
                    }
                    i--; // decrement i to account for i++ in the outer while loop
                }
                i++;
            }
            if (epsilon)
            {
                if (searchFirstSet(firstSets[lhsNonTerminal], "#") == -1)
                {
                    firstSets[lhsNonTerminal].push_back("#");
                    changed = true;
                }
            }

            i++; // skip STAR
            lhs = true;
            rhs = false;
            epsilon = true; // reset epsilon flag for next production rule
        }
    }
}

// Task 3
void CalculateFirstSets()
{
    calculateFirstSetsHelper();
    // print first sets
    printFirstSets(firstSets);
    // cout << "3\n";
}

// function to print follow sets from an unordered_map
void printFollowSets(unordered_map<string, vector<string>> map)
{
    string followSet = "";
    for (int i = 0; i < nonTerminals.size(); i++)
    {
        if (map[nonTerminals[i].lexeme].size() == 0)
        {
            followSet += "FOLLOW(" + nonTerminals[i].lexeme + ") = {  }\n";
            cout << followSet;
            followSet = "";
            continue;
        }

        followSet += "FOLLOW(" + nonTerminals[i].lexeme + ") = { ";
        if (searchFirstSet(map[nonTerminals[i].lexeme], "$") != -1)
        {
            followSet += "$, ";
        }
        for (int j = 0; j < terminals.size(); j++)
        {
            if (searchFirstSet(map[nonTerminals[i].lexeme], terminals[j].lexeme) != -1)
            {
                followSet += terminals[j].lexeme + ", ";
            }
        }
        if (followSet.length() >= 2)
        {
            followSet = followSet.substr(0, followSet.length() - 2);
        }
        followSet += " }\n";
        cout << followSet;
        followSet = "";
    }
}

// function to check if all symbols in the rhs of a production rule have epsilon in their first set
bool allHaveEpsilon(vector<Token> rhsSymbols, int start, int end)
{
    for (int i = start; i <= end; i++)
    {
        if (searchFirstSet(firstSets[rhsSymbols[i].lexeme], "#") == -1)
        {
            return false;
        }
    }
    return true;
}

// function to add elements from one vector to another
bool addToFollowSet(vector<string> &vec1, vector<string> vec2)
{
    bool changed = false;
    for (int i = 0; i < vec2.size(); i++)
    {
        string currTerminal = vec2[i];
        if (searchFirstSet(vec1, currTerminal) == -1 && currTerminal != "#")
        {
            vec1.push_back(currTerminal);
            changed = true;
        }
    }
    return changed;
}

void calculateFollowSetsHelper()
{
    calculateFirstSetsHelper();

    // initialize unordered_map with non-terminals, and their corresponding follow sets
    Token startSymbol = tokens[0];
    vector<string> followSetStart;
    followSetStart.push_back("$");
    followSets[startSymbol.lexeme] = followSetStart;

    for (int i = 0; i < nonTerminals.size(); i++)
    {
        if (nonTerminals[i].lexeme != startSymbol.lexeme)
        {
            vector<string> followSet;
            followSets[nonTerminals[i].lexeme] = followSet;
        }
    }

    int i = 0;
    // bool lhs = true;
    // bool rhs = false;
    while (tokens[i].token_type != HASH)
    {
        Token lhsToken = tokens[i];
        i++; // skip lhs
        i++; // skip arrow

        vector<Token> rhs;
        while (tokens[i].token_type != STAR)
        {
            rhs.push_back(tokens[i]);
            i++;
        }

        for (int j = 0; j < rhs.size(); j++)
        {
            if (searchToken(terminals, rhs[j]) != -1)
            {
                continue;
            }

            for (int k = j + 1; k < rhs.size(); k++)
            {
                addToFollowSet(followSets[rhs[j].lexeme], firstSets[rhs[k].lexeme]);

                if (searchFirstSet(firstSets[rhs[k].lexeme], "#") == -1)
                {
                    break;
                }
            }
        }

        i++; // skip STAR
    }

    /*
     * epsilon checking is done helper function to check if all symbols in the rhs of a production rule have epsilon in their first set
     */
    bool changed = true;
    while (changed)
    {
        changed = false;
        int i = 0;

        while (tokens[i].token_type != HASH)
        {
            Token lhsToken = tokens[i];
            i++; // skip lhs
            i++; // skip arrow

            vector<Token> rhs;
            while (tokens[i].token_type != STAR)
            {
                rhs.push_back(tokens[i]);
                i++;
            }

            for (int j = 0; j < rhs.size(); j++)
            {
                if (searchToken(terminals, rhs[j]) != -1)
                {
                    continue;
                }

                if (j == rhs.size() - 1)
                {
                    if (addToFollowSet(followSets[rhs[j].lexeme], followSets[lhsToken.lexeme]))
                    {
                        changed = true;
                    }
                }
                else if (allHaveEpsilon(rhs, j + 1, rhs.size() - 1))
                {
                    if (addToFollowSet(followSets[rhs[j].lexeme], followSets[lhsToken.lexeme]))
                    {
                        changed = true;
                    }
                }
            }

            i++; // skip STAR
        }
    }

    // print follow sets
    // printFollowSets(followSets);
}

// Task 4
void CalculateFollowSets()
{
    calculateFollowSetsHelper();
    // print follow sets
    printFollowSets(followSets);
    // cout << "4\n";
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{
    // if useless symbols, then no predictive parser
    uselessSymbolsHelper();
    if (reachableSymbols.size() != nonTerminals.size()) // this indicates that there are unreachable symbols, so no predictive parser
    {
        cout << "NO\n";
        return;
    }
    cout << "YES\n";
}

int main(int argc, char *argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);

    ReadGrammar(); // Reads the input grammar from standard input
                   // and represent it internally in data structures
                   // as described in project 2 presentation file

    switch (task)
    {
    case 1:
        printTerminalsAndNoneTerminals();
        break;

    case 2:
        RemoveUselessSymbols();
        break;

    case 3:
        CalculateFirstSets();
        break;

    case 4:
        CalculateFollowSets();
        break;

    case 5:
        CheckIfGrammarHasPredictiveParser();
        break;

    default:
        cout << "Error: unrecognized task number " << task << "\n";
        break;
    }
    return 0;
}
