/*
 * Copyright (C) Rida Bazzi
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cstdio>

#include "inputbuf.h"

using namespace std;

bool InputBuffer::EndOfInput()
{
    if (!input_buffer.empty())
    {
        // cout << "input buffer is not empty\n";
        return false;
    }
    else
    {
        // cout << "input buffer is empty\n";
        return cin.eof();
    }
}

char InputBuffer::UngetChar(char c)
{
    if (c != EOF)
        input_buffer.push_back(c);

    // cout << "input buffer: " << "[";
    // for (int i = 0; i < input_buffer.size(); i++)
    // {
    //     cout << input_buffer[i] << " ";
    // }
    // cout << "]" << endl;

    return c;
}

void InputBuffer::GetChar(char &c)
{
    if (!input_buffer.empty())
    {
        // cout << "gets char from non-empty input buffer\n";
        c = input_buffer.back();
        input_buffer.pop_back();
    }
    else
    {
        // cout << "gets char from cin\n";
        cin.get(c);
    }
    // cout << "c: " << c << endl;
}

string InputBuffer::UngetString(string s)
{
    for (int i = 0; i < s.size(); i++)
        input_buffer.push_back(s[s.size() - i - 1]);
    return s;
}
