#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include "lexer.h"

// my includes
#include <map>
#include <iostream>

using namespace std;

LexicalAnalyzer lexer;

// my variable declarations
map<string, int> variables;
// vector<Token> tokenList;

// my function declarations
// void read_grammar();
InstructionNode *parse_generate_intermediate_representation();
void set_location_of_variable(string s);
int get_location_of_variable(string s);
void parse_var_section();
InstructionNode *parse_body();
InstructionNode *parse_assign_statement();
InstructionNode *parse_input_statement();
InstructionNode *parse_output_statement();
InstructionNode *parse_if_statement();
InstructionNode *parse_while_statement();
InstructionNode *parse_for_statement();
InstructionNode *parse_switch_statement();
void parse_inputs();

struct InstructionNode *parse_generate_intermediate_representation()
{
    /*
    // Sample program for demonstration purpose only
    // Replace the following with a parser that reads the program from stdin &
    // creates appropriate data structures to be executed by execute_program()
    // This is the imaginary input for the following construction:

    // a, b, c, d;
    // {
    //     input a;
    //     input b;
    //     c = 10;
    //
    //     IF c <> a
    //     {
    //         output b;
    //     }
    //
    //     IF c > 1
    //     {
    //         a = b + 900;
    //         input d;
    //         IF a > 10
    //         {
    //             output d;
    //         }
    //     }
    //
    //     d = 0;
    //     WHILE d < 4
    //     {
    //         c = a + d;
    //         IF d > 1
    //         {
    //             output d;
    //         }
    //         d = d + 1;
    //     }
    // }
    // 1 2 3 4 5 6

    // Assigning location for variable "a"
    int address_a = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "b"
    int address_b = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "c"
    int address_c = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "d"
    int address_d = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for constant 10
    int address_ten = next_available;
    mem[next_available] = 10;
    next_available++;

    // Assigning location for constant 1
    int address_one = next_available;
    mem[next_available] = 1;
    next_available++;

    // Assigning location for constant 900
    int address_ninehundred = next_available;
    mem[next_available] = 900;
    next_available++;

    // Assigning location for constant 3
    int address_three = next_available;
    mem[next_available] = 3;
    next_available++;

    // Assigning location for constant 0
    int address_zero = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for constant 4
    int address_four = next_available;
    mem[next_available] = 4;
    next_available++;

    struct InstructionNode * i1 = new InstructionNode;
    struct InstructionNode * i2 = new InstructionNode;
    struct InstructionNode * i3 = new InstructionNode;
    struct InstructionNode * i4 = new InstructionNode;
    struct InstructionNode * i5 = new InstructionNode;
    struct InstructionNode * i6 = new InstructionNode;
    struct InstructionNode * i7 = new InstructionNode;
    struct InstructionNode * i8 = new InstructionNode;
    struct InstructionNode * i9 = new InstructionNode;
    struct InstructionNode * i10 = new InstructionNode;
    struct InstructionNode * i11 = new InstructionNode;
    struct InstructionNode * i12 = new InstructionNode;
    struct InstructionNode * i13 = new InstructionNode;
    struct InstructionNode * i14 = new InstructionNode;
    struct InstructionNode * i15 = new InstructionNode;
    struct InstructionNode * i16 = new InstructionNode;
    struct InstructionNode * i17 = new InstructionNode;
    struct InstructionNode * i18 = new InstructionNode;
    struct InstructionNode * i19 = new InstructionNode;
    struct InstructionNode * i20 = new InstructionNode;
    struct InstructionNode * i21 = new InstructionNode;
    struct InstructionNode * i22 = new InstructionNode;

    i1->type = IN;                                      // input a
    i1->input_inst.var_index = address_a;
    i1->next = i2;

    i2->type = IN;                                      // input b
    i2->input_inst.var_index = address_b;
    i2->next = i3;

    i3->type = ASSIGN;                                  // c = 10
    i3->assign_inst.left_hand_side_index = address_c;
    i3->assign_inst.op = OPERATOR_NONE;
    i3->assign_inst.operand1_index = address_ten;
    i3->next = i4;

    i4->type = CJMP;                                    // if c <> a
    i4->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    i4->cjmp_inst.operand1_index = address_c;
    i4->cjmp_inst.operand2_index = address_a;
    i4->cjmp_inst.target = i6;                          // if not (c <> a) skip forward to NOOP
    i4->next = i5;

    i5->type = OUT;                                     // output b
    i5->output_inst.var_index = address_b;
    i5->next = i6;

    i6->type = NOOP;                                    // NOOP after IF
    i6->next = i7;

    i7->type = CJMP;                                    // if c > 1
    i7->cjmp_inst.condition_op = CONDITION_GREATER;
    i7->cjmp_inst.operand1_index = address_c;
    i7->cjmp_inst.operand2_index = address_one;
    i7->cjmp_inst.target = i13;                         // if not (c > 1) skip forward to NOOP (way down)
    i7->next = i8;

    i8->type = ASSIGN;                                  // a = b + 900
    i8->assign_inst.left_hand_side_index = address_a;
    i8->assign_inst.op = OPERATOR_PLUS;
    i8->assign_inst.operand1_index = address_b;
    i8->assign_inst.operand2_index = address_ninehundred;
    i8->next = i9;

    i9->type = IN;                                      // input d
    i9->input_inst.var_index = address_d;
    i9->next = i10;

    i10->type = CJMP;                                   // if a > 10
    i10->cjmp_inst.condition_op = CONDITION_GREATER;
    i10->cjmp_inst.operand1_index = address_a;
    i10->cjmp_inst.operand2_index = address_ten;
    i10->cjmp_inst.target = i12;                        // if not (a > 10) skipp forward to NOOP
    i10->next = i11;

    i11->type = OUT;                                    // output d
    i11->output_inst.var_index = address_d;
    i11->next = i12;

    i12->type = NOOP;                                   // NOOP after inner IF
    i12->next = i13;

    i13->type = NOOP;                                   // NOOP after outer IF
    i13->next = i14;

    i14->type = ASSIGN;                                 // d = 0
    i14->assign_inst.left_hand_side_index = address_d;
    i14->assign_inst.op = OPERATOR_NONE;
    i14->assign_inst.operand1_index = address_zero;
    i14->next = i15;

    i15->type = CJMP;                                   // if d < 4
    i15->cjmp_inst.condition_op = CONDITION_LESS;
    i15->cjmp_inst.operand1_index = address_d;
    i15->cjmp_inst.operand2_index = address_four;
    i15->cjmp_inst.target = i22;                        // if not (d < 4) skip whole WHILE body
    i15->next = i16;

    i16->type = ASSIGN;                                 // c = a + d
    i16->assign_inst.left_hand_side_index = address_c;
    i16->assign_inst.op = OPERATOR_PLUS;
    i16->assign_inst.operand1_index = address_a;
    i16->assign_inst.operand2_index = address_d;
    i16->next = i17;

    i17->type = CJMP;                                   // if d > 1
    i17->cjmp_inst.condition_op = CONDITION_GREATER;
    i17->cjmp_inst.operand1_index = address_d;
    i17->cjmp_inst.operand2_index = address_one;
    i17->cjmp_inst.target = i19;                        // if not (d > 1) skip body of IF
    i17->next = i18;

    i18->type = OUT;                                    // output d
    i18->output_inst.var_index = address_d;
    i18->next = i19;

    i19->type = NOOP;                                   // NOOP after body of IF
    i19->next = i20;

    i20->type = ASSIGN;                                 // d = d + 1
    i20->assign_inst.left_hand_side_index = address_d;
    i20->assign_inst.op = OPERATOR_PLUS;
    i20->assign_inst.operand1_index = address_d;
    i20->assign_inst.operand2_index = address_one;
    i20->next = i21;

    i21->type = JMP;
    i21->jmp_inst.target = i15;
    i21->next = i22;

    i22->type = NOOP;                                   // NOOP after body of WHILE
    i22->next = NULL;

    // Inputs
    inputs.push_back(1);
    inputs.push_back(2);
    inputs.push_back(3);
    inputs.push_back(4);
    inputs.push_back(5);
    inputs.push_back(6);

    return i1;
    */

    // read_grammar();
    parse_var_section();
    InstructionNode *body = parse_body();
    parse_inputs();
    return body;
}

// void read_grammar()
// {
//     Token token = lexer.GetToken();
//     while (token.token_type != END_OF_FILE)
//     {
//         token.Print();
//         tokenList.push_back(token);
//         token = lexer.GetToken();
//     }
//     token.Print();
//     tokenList.push_back(token);
// }

void set_location_of_variable(string s)
{
    if (variables.find(s) == variables.end())
    {
        variables[s] = next_available;
        mem[next_available] = 0;
        next_available++;
    }
}

int get_location_of_variable(string s)
{
    if (variables.find(s) == variables.end())
    {
        return -1;
    }
    return variables[s];
}

void parse_var_section()
{
    Token token = lexer.GetToken();
    while (token.token_type != SEMICOLON)
    {
        if (token.token_type == ID)
        {
            set_location_of_variable(token.lexeme);
        }
        token = lexer.GetToken();
    }
    // ends with SEMICOLON so no need to unget token
}

InstructionNode *parse_body()
{
    InstructionNode *head = nullptr;
    InstructionNode *tail = nullptr;
    Token token = lexer.GetToken();
    while (token.token_type != RBRACE) // loop until RBRACE is seen
    {
        if (token.token_type == ID)
        {
            lexer.UngetToken(1);
            InstructionNode *assign = parse_assign_statement();
            if (head == nullptr)
            {
                head = assign;
                tail = assign;
            }
            else
            {
                tail->next = assign;
                tail = assign;
            }
        }
        else if (token.token_type == INPUT)
        {
            lexer.UngetToken(1);
            InstructionNode *input = parse_input_statement();
            if (head == nullptr)
            {
                head = input;
                tail = input;
            }
            else
            {
                tail->next = input;
                tail = input;
            }
        }
        else if (token.token_type == OUTPUT)
        {
            lexer.UngetToken(1);
            InstructionNode *output = parse_output_statement();
            if (head == nullptr)
            {
                head = output;
                tail = output;
            }
            else
            {
                tail->next = output;
                tail = output;
            }
        }
        else if (token.token_type == IF)
        {
            lexer.UngetToken(1);
            InstructionNode *if_stmt = parse_if_statement();
            if (head == nullptr)
            {
                head = if_stmt;
                tail = if_stmt;
            }
            else
            {
                tail->next = if_stmt;
                tail = if_stmt;
            }
            // Find the new tail (last node in the IF statement)
            while (tail->next != nullptr)
            {
                tail = tail->next;
            }
        }
        else if (token.token_type == WHILE)
        {
            lexer.UngetToken(1);
            InstructionNode *while_stmt = parse_while_statement();
            if (head == nullptr)
            {
                head = while_stmt;
                tail = while_stmt;
            }
            else
            {
                tail->next = while_stmt;
                tail = while_stmt;
            }
            // Find the new tail (last node in the WHILE statement)
            while (tail->next != nullptr)
            {
                tail = tail->next;
            }
        }
        else if (token.token_type == FOR)
        {
            lexer.UngetToken(1);
            InstructionNode *for_stmt = parse_for_statement();
            if (head == nullptr)
            {
                head = for_stmt;
                tail = for_stmt;
            }
            else
            {
                tail->next = for_stmt;
                tail = for_stmt;
            }
            // Find the new tail (last node in the FOR statement)
            while (tail->next != nullptr)
            {
                tail = tail->next;
            }
        }
        else if (token.token_type == SWITCH)
        {
            // parse_switch_statement() is not implemented
            lexer.UngetToken(1);
            InstructionNode *switch_stmt = parse_switch_statement();
            if (head == nullptr)
            {
                head = switch_stmt;
                tail = switch_stmt;
            }
            else
            {
                tail->next = switch_stmt;
                tail = switch_stmt;
            }
            // Find the new tail (last node in the SWITCH statement)
            while (tail->next != nullptr)
            {
                tail = tail->next;
            }
        }
        token = lexer.GetToken(); // if ID, IF, etc., it is getting parsed; if LBRACE/RBARCE, it is getting ignored
    }
    return head;
}

InstructionNode *parse_assign_statement()
{
    InstructionNode *assign = new InstructionNode();
    assign->type = ASSIGN;
    assign->next = nullptr;

    Token token = lexer.GetToken(); // get left hand side of assignment
    assign->assign_inst.left_hand_side_index = get_location_of_variable(token.lexeme);

    token = lexer.GetToken(); // skip EQUAL

    token = lexer.GetToken();
    if (lexer.peek(1).token_type == SEMICOLON)
    {
        assign->assign_inst.op = OPERATOR_NONE;
        if (token.token_type == NUM) // if constant is seen again, it is being assigned a new location in memory
        {
            mem[next_available] = stoi(token.lexeme);
            assign->assign_inst.operand1_index = next_available;
            next_available++;
        }
        else if (token.token_type == ID)
        {
            assign->assign_inst.operand1_index = get_location_of_variable(token.lexeme);
        }
    }
    else
    {
        if (token.token_type == NUM) // if constant is seen again, it is being assigned a new location in memory
        {
            mem[next_available] = stoi(token.lexeme);
            assign->assign_inst.operand1_index = next_available;
            next_available++;
        }
        else if (token.token_type == ID)
        {
            assign->assign_inst.operand1_index = get_location_of_variable(token.lexeme);
        }

        token = lexer.GetToken(); // get the operator
        if (token.token_type == PLUS)
        {
            assign->assign_inst.op = OPERATOR_PLUS;
        }
        else if (token.token_type == MINUS)
        {
            assign->assign_inst.op = OPERATOR_MINUS;
        }
        else if (token.token_type == MULT)
        {
            assign->assign_inst.op = OPERATOR_MULT;
        }
        else if (token.token_type == DIV)
        {
            assign->assign_inst.op = OPERATOR_DIV;
        }

        token = lexer.GetToken();    // get the second operand
        if (token.token_type == NUM) // if constant is seen again, it is being assigned a new location in memory
        {
            mem[next_available] = stoi(token.lexeme);
            assign->assign_inst.operand2_index = next_available;
            next_available++;
        }
        else if (token.token_type == ID)
        {
            assign->assign_inst.operand2_index = get_location_of_variable(token.lexeme);
        }
    }

    token = lexer.GetToken(); // skip SEMICOLON

    return assign; // return the assign instruction
}

InstructionNode *parse_input_statement()
{
    InstructionNode *input = new InstructionNode();
    input->type = IN;
    input->next = nullptr;

    Token token = lexer.GetToken(); // skip "input" (INPUT)

    token = lexer.GetToken(); // get the variable
    input->input_inst.var_index = get_location_of_variable(token.lexeme);

    token = lexer.GetToken(); // skip SEMICOLON

    return input; // return the input instruction
}

InstructionNode *parse_output_statement()
{
    InstructionNode *output = new InstructionNode();
    output->type = OUT;
    output->next = nullptr;

    Token token = lexer.GetToken(); // skip "output" (OUTPUT)

    token = lexer.GetToken(); // get the variable
    output->output_inst.var_index = get_location_of_variable(token.lexeme);

    token = lexer.GetToken(); // skip SEMICOLON

    return output; // return the output instruction
}

InstructionNode *parse_if_statement()
{
    InstructionNode *if_statement = new InstructionNode();
    if_statement->type = CJMP;
    if_statement->next = nullptr;

    Token token = lexer.GetToken(); // skip "if" (IF)

    token = lexer.GetToken(); // get the left operand
    if_statement->cjmp_inst.operand1_index = get_location_of_variable(token.lexeme);

    token = lexer.GetToken(); // get the operator
    if (token.token_type == GREATER)
    {
        if_statement->cjmp_inst.condition_op = CONDITION_GREATER;
    }
    else if (token.token_type == LESS)
    {
        if_statement->cjmp_inst.condition_op = CONDITION_LESS;
    }
    else if (token.token_type == NOTEQUAL)
    {
        if_statement->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    }

    token = lexer.GetToken(); // get the right operand
    if_statement->cjmp_inst.operand2_index = get_location_of_variable(token.lexeme);

    // token = lexer.GetToken(); // skip LBRACE
    InstructionNode *body = parse_body();
    if_statement->next = body; // True condition continues to body

    InstructionNode *noop = new InstructionNode();
    noop->type = NOOP;
    noop->next = nullptr;

    // Find last node in body and connect it to NOOP
    InstructionNode *last = body;
    while (last->next != nullptr)
    {
        last = last->next;
    }
    last->next = noop;

    if_statement->cjmp_inst.target = noop; // False condition jumps to NOOP
    // token = lexer.GetToken(); // skip RBRACE

    return if_statement; // return the if statement
}

InstructionNode *parse_while_statement()
{
    InstructionNode *while_statement = new InstructionNode();
    while_statement->type = CJMP;
    while_statement->next = nullptr;

    Token token = lexer.GetToken(); // skip "while" (WHILE)

    token = lexer.GetToken();    // get the left operand
    if (token.token_type == NUM) // if constant is seen again, it is being assigned a new location in memory
    {
        mem[next_available] = stoi(token.lexeme);
        while_statement->cjmp_inst.operand1_index = next_available;
        next_available++;
    }
    else if (token.token_type == ID)
    {
        while_statement->cjmp_inst.operand1_index = get_location_of_variable(token.lexeme);
    }

    token = lexer.GetToken(); // get the operator
    if (token.token_type == GREATER)
    {
        while_statement->cjmp_inst.condition_op = CONDITION_GREATER;
    }
    else if (token.token_type == LESS)
    {
        while_statement->cjmp_inst.condition_op = CONDITION_LESS;
    }
    else if (token.token_type == NOTEQUAL)
    {
        while_statement->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    }

    token = lexer.GetToken();    // get the right operand
    if (token.token_type == NUM) // if constant is seen again, it is being assigned a new location in memory
    {
        mem[next_available] = stoi(token.lexeme);
        while_statement->cjmp_inst.operand2_index = next_available;
        next_available++;
    }
    else if (token.token_type == ID)
    {
        while_statement->cjmp_inst.operand2_index = get_location_of_variable(token.lexeme);
    }

    // token = lexer.GetToken(); // skip LBRACE
    InstructionNode *body = parse_body();
    while_statement->next = body; // True condition continues to body

    InstructionNode *jmp = new InstructionNode();
    jmp->type = JMP;
    jmp->next = nullptr;                    // set next to nullptr for now
    jmp->jmp_inst.target = while_statement; // Jump back to the condition

    InstructionNode *temp = body;
    while (temp->next != nullptr)
    {
        temp = temp->next;
    }
    temp->next = jmp; // Connect last node in body to JMP

    InstructionNode *noop = new InstructionNode();
    noop->type = NOOP;
    noop->next = nullptr;

    // Find last node in jmp and connect it to NOOP
    // InstructionNode *last = jmp;
    // while (last->next != nullptr)
    //{
    //    last = last->next;
    //}
    jmp->next = noop;

    while_statement->cjmp_inst.target = noop; // False condition jumps to NOOP
    // token = lexer.GetToken(); // skip RBRACE

    return while_statement; // return the while statement
}

InstructionNode *parse_for_statement()
{
    InstructionNode *while_of_for_statement = new InstructionNode();
    while_of_for_statement->type = CJMP;
    while_of_for_statement->next = nullptr;

    Token token = lexer.GetToken(); // skip "for" (FOR)
    token = lexer.GetToken();       // skip LBRAC

    // Parse the first assign statement
    InstructionNode *assign1 = parse_assign_statement(); // should be just 1 node?

    assign1->next = while_of_for_statement; // connect assign1 to while_of_for_statement

    token = lexer.GetToken();    // left operand of condition
    if (token.token_type == NUM) // if constant is seen again, it is being assigned a new location in memory
    {
        mem[next_available] = stoi(token.lexeme);
        while_of_for_statement->cjmp_inst.operand1_index = next_available;
        next_available++;
    }
    else if (token.token_type == ID)
    {
        while_of_for_statement->cjmp_inst.operand1_index = get_location_of_variable(token.lexeme);
    }

    token = lexer.GetToken(); // get the operator
    if (token.token_type == GREATER)
    {
        while_of_for_statement->cjmp_inst.condition_op = CONDITION_GREATER;
    }
    else if (token.token_type == LESS)
    {
        while_of_for_statement->cjmp_inst.condition_op = CONDITION_LESS;
    }
    else if (token.token_type == NOTEQUAL)
    {
        while_of_for_statement->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    }

    token = lexer.GetToken();    // right operand of condition
    if (token.token_type == NUM) // if constant is seen again, it is being assigned a new location in memory
    {
        mem[next_available] = stoi(token.lexeme);
        while_of_for_statement->cjmp_inst.operand2_index = next_available;
        next_available++;
    }
    else if (token.token_type == ID)
    {
        while_of_for_statement->cjmp_inst.operand2_index = get_location_of_variable(token.lexeme);
    }

    token = lexer.GetToken(); // skip SEMICOLON

    // Parse the second assign statement
    InstructionNode *assign2 = parse_assign_statement(); // should be just 1 node?

    token = lexer.GetToken(); // skip RBRAC

    InstructionNode *body = parse_body();
    while_of_for_statement->next = body; // True condition continues to body

    InstructionNode *temp = body;
    while (temp->next != nullptr)
    {
        temp = temp->next;
    }
    temp->next = assign2; // Connect last node in body to second ASSIGN node (incrementer)

    InstructionNode *jmp = new InstructionNode();
    jmp->type = JMP;
    jmp->next = nullptr; // set next to nullptr for now
    jmp->jmp_inst.target = while_of_for_statement;

    assign2->next = jmp; // Connect second ASSIGN node to JMP

    InstructionNode *noop = new InstructionNode();
    noop->type = NOOP;
    noop->next = nullptr;

    jmp->next = noop; // Connect JMP to NOOP

    while_of_for_statement->cjmp_inst.target = noop; // False condition jumps to NOOP

    return assign1; // return the first assign statement
}

InstructionNode *parse_switch_statement()
{
    // in normal IF, if condition evaluated to true, it goes to body (through next pointer); if false, it goes to NOOP (through target pointer)
    // in switch, we reverse this. if condition (of non-equality) evaluated to true, it goes to NOOP (through next pointer); if false, it goes to body (through target pointer)

    Token token = lexer.GetToken(); // skip "switch" (SWITCH)

    token = lexer.GetToken(); // get the switch variable
    Token token_var = token;  // save the variable token

    token = lexer.GetToken(); // skip LBRACE

    // Create the main NOOP node that will be the end of the switch statement
    InstructionNode *noop = new InstructionNode();
    noop->type = NOOP;
    noop->next = nullptr;

    InstructionNode *head = nullptr; // Head node of switch statement
    InstructionNode *curr = nullptr; // Current node in switch statement
    InstructionNode *prev = nullptr; // Previous node in switch statement
    bool has_default = false;
    InstructionNode *default_body = nullptr;

    token = lexer.GetToken();
    while (token.token_type != RBRACE)
    {
        if (token.token_type == CASE)
        {
            // Create CJMP node
            InstructionNode *cjmp = new InstructionNode();
            cjmp->type = CJMP;
            cjmp->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
            cjmp->cjmp_inst.operand1_index = get_location_of_variable(token_var.lexeme);

            token = lexer.GetToken(); // get case number
            mem[next_available] = stoi(token.lexeme);
            cjmp->cjmp_inst.operand2_index = next_available;
            next_available++;

            token = lexer.GetToken(); // skip COLON

            // Parse case body
            InstructionNode *case_body = parse_body();

            // Create JMP node to jump to end after case body
            InstructionNode *jmp = new InstructionNode();
            jmp->type = JMP;
            jmp->jmp_inst.target = noop;

            // Connect case body to jmp
            InstructionNode *last = case_body;
            while (last->next != nullptr)
            {
                last = last->next;
            }
            last->next = jmp;

            // Set up CJMP connections
            cjmp->cjmp_inst.target = case_body; // if equal, execute case body
            cjmp->next = nullptr;               // if not equal, try next case (set later)

            if (head == nullptr)
            {
                head = cjmp;
            }
            if (prev != nullptr)
            {
                prev->next = cjmp;
            }

            prev = cjmp;
            curr = jmp;
        }
        else if (token.token_type == DEFAULT)
        {
            has_default = true;
            token = lexer.GetToken(); // skip COLON

            // Parse default body
            default_body = parse_body();

            // Create JMP node to jump to end after default body
            InstructionNode *jmp = new InstructionNode();
            jmp->type = JMP;
            jmp->jmp_inst.target = noop;

            // Connect default body to jmp
            InstructionNode *last = default_body;
            while (last->next != nullptr)
            {
                last = last->next;
            }
            last->next = jmp;

            curr = jmp;
        }
        token = lexer.GetToken();
    }

    if (has_default)
    {
        if (prev != nullptr)
        {
            prev->next = default_body;
        }
        curr->next = noop; // maybe fix this
    }
    else
    {
        if (prev != nullptr)
        {
            prev->next = noop;
        }
    }

    return head;
}

void parse_inputs()
{
    Token token = lexer.GetToken();
    while (token.token_type != END_OF_FILE)
    {
        if (token.token_type == NUM) // it should be NUM but good for error checking
        {
            inputs.push_back(stoi(token.lexeme));
        }
        token = lexer.GetToken();
    }
}