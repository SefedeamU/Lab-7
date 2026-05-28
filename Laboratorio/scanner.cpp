#include <iostream>
#include <cstring>
#include <fstream>
#include "token.h"
#include "scanner.h"

using namespace std;

Scanner::Scanner(const char* s): input(s), first(0), current(0) { }

bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token* Scanner::nextToken() {
    while (current < (int)input.length() && is_white_space(input[current]))
        current++;

    if (current >= (int)input.length())
        return new Token(Token::END);

    char c = input[current];
    first = current;

    if (isdigit(c)) {
        current++;
        while (current < (int)input.length() && isdigit(input[current]))
            current++;
        return new Token(Token::NUM, input, first, current - first);
    }

    if (isalpha(c) || c == '_') {
        current++;
        while (current < (int)input.length() && (isalnum(input[current]) || input[current] == '_'))
            current++;
        string lexema = input.substr(first, current - first);

        if (lexema == "sqrt")     return new Token(Token::SQRT,     input, first, current - first);
        if (lexema == "print")    return new Token(Token::PRINT,    input, first, current - first);
        if (lexema == "if")       return new Token(Token::IF,       input, first, current - first);
        if (lexema == "else")     return new Token(Token::ELSE,     input, first, current - first);
        if (lexema == "then")     return new Token(Token::THEN,     input, first, current - first);
        if (lexema == "endif")    return new Token(Token::ENDIF,    input, first, current - first);
        if (lexema == "do")       return new Token(Token::DO,       input, first, current - first);
        if (lexema == "while")    return new Token(Token::WHILE,    input, first, current - first);
        if (lexema == "endwhile") return new Token(Token::ENDWHILE, input, first, current - first);
        if (lexema == "var")      return new Token(Token::VAR,      input, first, current - first);
        if (lexema == "fun")      return new Token(Token::FUN,      input, first, current - first);
        if (lexema == "endfun")   return new Token(Token::ENDFUN,   input, first, current - first);
        if (lexema == "return")   return new Token(Token::RETURN,   input, first, current - first);
        if (lexema == "break")    return new Token(Token::BREAK,    input, first, current - first);
        if (lexema == "and")      return new Token(Token::AND,      input, first, current - first);
        if (lexema == "or")       return new Token(Token::OR,       input, first, current - first);
        if (lexema == "not")      return new Token(Token::NOT,      input, first, current - first);
        if (lexema == "true")     return new Token(Token::TRUE,     input, first, current - first);
        if (lexema == "false")    return new Token(Token::FALSE,    input, first, current - first);

        return new Token(Token::ID, input, first, current - first);
    }

    current++;

    switch (c) {
        case ',': return new Token(Token::COMA,    c);
        case ';': return new Token(Token::SEMICOL, c);
        case ':': return new Token(Token::COLON,   c);
        case '(': return new Token(Token::LPAREN,  c);
        case ')': return new Token(Token::RPAREN,  c);
        case '-': return new Token(Token::MINUS,   c);
        case '/': return new Token(Token::DIV,     c);

        case '+':
            if (current < (int)input.length() && input[current] == '+') {
                current++;
                return new Token(Token::PLUSPLUS, input, first, 2);
            }
            return new Token(Token::PLUS, c);

        case '*':
            if (current < (int)input.length() && input[current] == '*') {
                current++;
                return new Token(Token::POW, input, first, 2);
            }
            return new Token(Token::MUL, c);

        case '=':
            if (current < (int)input.length() && input[current] == '=') {
                current++;
                return new Token(Token::EQUALEQUAL, input, first, 2);
            }
            return new Token(Token::ASSIGN, c);

        case '<':
            if (current < (int)input.length() && input[current] == '=') {
                current++;
                return new Token(Token::LESSTHANEQUAL, input, first, 2);
            }
            return new Token(Token::LESSTHAN, c);

        default:
            return new Token(Token::ERR, c);
    }
}

Scanner::~Scanner() { }

void ejecutar_scanner(Scanner* scanner, const string& InputFile) {
    Token* tok;

    string OutputFileName = InputFile;
    size_t pos = OutputFileName.find_last_of(".");
    if (pos != string::npos)
        OutputFileName = OutputFileName.substr(0, pos);
    OutputFileName += "_tokens.txt";

    ofstream outFile(OutputFileName);

    if (!outFile.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << OutputFileName << endl;
        return;
    }

    outFile << "Scanner\n" << endl;

    while (true) {
        tok = scanner->nextToken();

        if (tok->type == Token::END) {
            outFile << *tok << endl;
            delete tok;
            outFile << "\nScanner exitoso" << endl;
            outFile.close();
            return;
        }

        if (tok->type == Token::ERR) {
            outFile << *tok << endl;
            delete tok;
            outFile << "Caracter invalido" << endl;
            outFile << "Scanner no exitoso" << endl;
            outFile.close();
            return;
        }

        outFile << *tok << endl;
        delete tok;
    }
}
