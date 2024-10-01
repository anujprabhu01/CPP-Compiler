/*
 * Copyright (C) Rida Bazzi
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = {
    "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM", "BASE16NUM" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = {"IF", "WHILE", "DO", "THEN", "PRINT"};

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int)this->token_type] << " , "
         << this->line_no << "}\n";
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
    // cout << "gets char in SkipSpace()\n";
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c))
    {
        // cout << "goes in this loop\n";
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput())
    {
        // cout << "goes in unget char in SkipSpace()\n";
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
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

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
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

Token LexicalAnalyzer::ScanNumber()
{
    char c;
    std::string tempLexeme = "";
    bool base08 = false;
    bool base10 = false;
    bool base16 = false;
    // int base08Count = 0;
    int base10Count = 0;
    int base16Count = 0;

    input.GetChar(c);
    if (isdigit(c))
    {
        if (c == '0')
        {
            tmp.lexeme = "0";

            if (!input.EndOfInput())
            {
                input.GetChar(c);
                if (c == 'x')
                {
                    tempLexeme += c;
                    if (!input.EndOfInput())
                    {
                        // cout << "flag 1" << endl;
                        input.GetChar(c);
                        if (c == '0')
                        {
                            tempLexeme += c;
                            if (!input.EndOfInput())
                            {
                                input.GetChar(c);
                                if (c == '8')
                                {
                                    tempLexeme += c;
                                    tmp.lexeme += tempLexeme;
                                    tempLexeme = "";

                                    tmp.token_type = BASE08NUM;
                                    tmp.line_no = line_no;
                                    return tmp;
                                }
                                else
                                {
                                    // not 8 after 0
                                    if (!input.EndOfInput())
                                    {
                                        input.UngetChar(c);   // unget non-8 after 0
                                        input.UngetChar('0'); // unget 0
                                        input.UngetChar('x'); // unget x
                                        tempLexeme = "";
                                    }
                                }
                            }
                        }
                        else if (c == '1')
                        {
                            tempLexeme += c;
                            if (!input.EndOfInput())
                            {
                                input.GetChar(c);
                                if (c == '6')
                                {
                                    tempLexeme += c;
                                    tmp.lexeme += tempLexeme;
                                    tempLexeme = "";

                                    tmp.token_type = BASE16NUM;
                                    tmp.line_no = line_no;
                                    return tmp;
                                }
                                else
                                {
                                    // not 6 after 1
                                    if (!input.EndOfInput())
                                    {
                                        input.UngetChar(c);   // unget non-6 after 1
                                        input.UngetChar('1'); // unget 1
                                        input.UngetChar('x'); // unget x
                                        tempLexeme = "";
                                    }
                                }
                            }
                            else
                            {
                                // end of input
                                if (!input.EndOfInput())
                                {
                                    input.UngetChar('1'); // unget 1
                                    input.UngetChar('x'); // unget x
                                    tempLexeme = "";
                                }
                            }
                        }
                        else
                        {
                            // c is not 0 or 1
                            if (!input.EndOfInput())
                            {
                                // cout << "flag 2" << endl;
                                input.UngetChar(c);   // unget non-0 or non-1 after 0
                                input.UngetChar('x'); // unget x
                                // cout << "flag 3: both ungets done" << endl;
                                tempLexeme = "";
                            }
                        }
                    }
                }
                else
                {
                    // c is not x
                    if (!input.EndOfInput())
                    {
                        input.UngetChar(c); // unget non-x after 0
                        tempLexeme = "";
                    }
                }
            }
        }
        else
        {
            tmp.lexeme = "";
            while (!input.EndOfInput() && (isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F'))
            {
                if ((c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F') || base16Count >= 1)
                {
                    base16 = true;
                    base16Count++;
                }
                else if ((c == '8' || c == '9') || base10Count >= 1)
                {
                    base10 = true;
                    base10Count++;
                }
                else
                {
                    base08 = true;
                    // base08Count++;
                }
                // tmp.lexeme += c;
                tempLexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput())
            {
                if (c == 'x')
                {
                    tempLexeme += c;
                    if (!input.EndOfInput())
                    {
                        input.GetChar(c);
                        if (c == '0')
                        {
                            if (!base16 && !base10)
                            {
                                tempLexeme += c;
                                if (!input.EndOfInput())
                                {
                                    input.GetChar(c);
                                    if (c == '8')
                                    {
                                        tempLexeme += c;
                                        tmp.lexeme += tempLexeme;
                                        tempLexeme = "";

                                        tmp.token_type = BASE08NUM;
                                        tmp.line_no = line_no;

                                        return tmp;
                                    }
                                    else
                                    {
                                        // not 8 after 0 and before x, num is base08
                                        if (!input.EndOfInput())
                                        {
                                            input.UngetChar(c);                      // unget non-8 after 0
                                            input.UngetChar('0');                    // unget 0
                                            tempLexeme.erase(tempLexeme.size() - 1); // erase 0
                                            input.UngetChar('x');                    // unget x
                                            tempLexeme.erase(tempLexeme.size() - 1); // erase x

                                            tmp.lexeme += tempLexeme;
                                            tempLexeme = "";
                                        }
                                    }
                                }
                            }
                            else
                            {
                                // c is 0 after x and number is base16 or base10
                                if (!input.EndOfInput())
                                {
                                    // cout << "flag 4" << endl;
                                    input.UngetChar('0');                    // unget 0; fixme
                                    input.UngetChar('x');                    // unget x; fixme
                                    tempLexeme.erase(tempLexeme.size() - 1); // erase x
                                    if (base16)
                                    {
                                        // cout << "flag 5" << endl;
                                        input.UngetString(tempLexeme.substr(tempLexeme.size() - base16Count));
                                        tempLexeme = tempLexeme.substr(0, tempLexeme.size() - base16Count);
                                    }
                                    tmp.lexeme += tempLexeme;
                                    // cout << "tempLexeme: " << tempLexeme << endl;
                                    tempLexeme = "";
                                }
                            }
                        }
                        else if (c == '1')
                        {
                            tempLexeme += c;
                            if (!input.EndOfInput())
                            {
                                input.GetChar(c);
                                if (c == '6')
                                {
                                    tempLexeme += c;
                                    tmp.lexeme += tempLexeme;
                                    tempLexeme = "";

                                    tmp.token_type = BASE16NUM;
                                    tmp.line_no = line_no;

                                    return tmp;
                                }
                                else
                                {
                                    // not 6 after 1
                                    if (!input.EndOfInput())
                                    {
                                        input.UngetChar(c);                      // unget non-6 after 1
                                        input.UngetChar('1');                    // unget 1
                                        tempLexeme.erase(tempLexeme.size() - 1); // erase 1
                                        input.UngetChar('x');                    // unget x
                                        tempLexeme.erase(tempLexeme.size() - 1); // erase x

                                        if (base16)
                                        {
                                            input.UngetString(tempLexeme.substr(tempLexeme.size() - base16Count));
                                            tempLexeme = tempLexeme.substr(0, tempLexeme.size() - base16Count);
                                        }

                                        tmp.lexeme += tempLexeme;
                                        tempLexeme = "";
                                    }
                                }
                            }
                        }
                        else
                        {
                            // not 0 or 1 after x
                            if (!input.EndOfInput())
                            {
                                input.UngetChar(c);                      // unget non-0 or non-1 after x
                                input.UngetChar('x');                    // unget x
                                tempLexeme.erase(tempLexeme.size() - 1); // erase x
                                if (base16)
                                {
                                    input.UngetString(tempLexeme.substr(tempLexeme.size() - base16Count));
                                    tempLexeme = tempLexeme.substr(0, tempLexeme.size() - base16Count);
                                }
                                tmp.lexeme += tempLexeme;
                                tempLexeme = "";
                            }
                        }
                    }
                }
                else
                {
                    // not x after number
                    if (!input.EndOfInput())
                    {
                        input.UngetChar(c); // unget non-x after number
                        if (base16)
                        {
                            input.UngetString(tempLexeme.substr(tempLexeme.size() - base16Count));
                            tempLexeme = tempLexeme.substr(0, tempLexeme.size() - base16Count);
                        }
                        tmp.lexeme += tempLexeme;
                        // cout << "tmp.lexeme: " << tmp.lexeme << endl;
                        tempLexeme = "";
                    }
                }
            }
        }
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
        if (!input.EndOfInput())
        {
            input.GetChar(c); // not end of input and c is not a digit
            if (c == '.')
            {
                tempLexeme += c;

                if (!input.EndOfInput())
                {
                    input.GetChar(c);
                    if (isdigit(c))
                    {
                        tempLexeme += c;

                        input.GetChar(c);
                        while (!input.EndOfInput() && isdigit(c))
                        {
                            tempLexeme += c;
                            input.GetChar(c);
                        }
                        if (!input.EndOfInput())
                        {
                            input.UngetChar(c);
                        }
                        tmp.lexeme += tempLexeme;
                        tempLexeme = "";

                        tmp.token_type = REALNUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }
                    else
                    {
                        // cout << "flag 6" << endl;
                        if (!input.EndOfInput())
                        {
                            // cout << "flag 7" << endl;
                            input.UngetChar(c);   // unget non-digit after period
                            input.UngetChar('.'); // unget period
                            tempLexeme = "";

                            // cout << "tempLexeme: " << tempLexeme << endl;
                            // cout << "tmp.lexeme: " << tmp.lexeme << endl;
                        }
                        // not a digit after period
                        // tmp.token_type = NUM;
                        // tmp.line_no = line_no;
                        // return tmp;
                    }
                }
            }
            else
            {
                if (!input.EndOfInput())
                {
                    input.UngetChar(c);
                    tempLexeme = "";
                }
            }
        }

        tmp.token_type = NUM;
        tmp.line_no = line_no;
        return tmp;
    }
    else
    {
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
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

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty())
    {
        // cout << "tokens is not empty\n";
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c)
    {
    case '.':
        tmp.token_type = DOT;
        return tmp;
    case '+':
        tmp.token_type = PLUS;
        return tmp;
    case '-':
        tmp.token_type = MINUS;
        return tmp;
    case '/':
        tmp.token_type = DIV;
        return tmp;
    case '*':
        tmp.token_type = MULT;
        return tmp;
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
    case '[':
        tmp.token_type = LBRAC;
        return tmp;
    case ']':
        tmp.token_type = RBRAC;
        return tmp;
    case '(':
        tmp.token_type = LPAREN;
        return tmp;
    case ')':
        tmp.token_type = RPAREN;
        return tmp;
    case '<':
        input.GetChar(c);
        if (c == '=')
        {
            tmp.token_type = LTEQ;
        }
        else if (c == '>')
        {
            tmp.token_type = NOTEQUAL;
        }
        else
        {
            if (!input.EndOfInput())
            {
                input.UngetChar(c);
            }
            tmp.token_type = LESS;
        }
        return tmp;
    case '>':
        input.GetChar(c);
        if (c == '=')
        {
            tmp.token_type = GTEQ;
        }
        else
        {
            if (!input.EndOfInput())
            {
                input.UngetChar(c);
            }
            tmp.token_type = GREATER;
        }
        return tmp;
    default:
        if (isdigit(c))
        {
            input.UngetChar(c);
            return ScanNumber();
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
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
