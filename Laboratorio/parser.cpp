#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"

using namespace std;

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = nullptr;
    current  = scanner->nextToken();
    if (current->type == Token::ERR)
        throw runtime_error("Error lexico al inicio");
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) { advance(); return true; }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current  = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR))
            throw runtime_error("Error lexico");
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return current->type == Token::END;
}

bool Parser::isBodyEnd() {
    return isAtEnd()
        || check(Token::ENDIF)
        || check(Token::ELSE)
        || check(Token::ENDWHILE)
        || check(Token::ENDFUN);
}

Programa* Parser::parseProgram() {
    Programa* ast = parseP();
    if (!isAtEnd())
        throw runtime_error("Error sintactico: tokens inesperados al final");
    cout << "Parseo exitoso" << endl;
    return ast;
}

Programa* Parser::parseP() {
    Programa* p = new Programa();
    while (check(Token::VAR)) {
        p->globalVars.push_back(parseVarDec());
        match(Token::SEMICOL);
    }
    while (check(Token::FUN)) {
        p->funciones.push_back(parseFunDec());
    }
    return p;
}

FunDec* Parser::parseFunDec() {
    match(Token::FUN);
    FunDec* fun = new FunDec();

    if (!match(Token::ID))
        throw runtime_error("Error sintactico: se esperaba tipo de retorno en fun");
    fun->retorno = previous->text;

    if (!match(Token::ID))
        throw runtime_error("Error sintactico: se esperaba nombre de funcion");
    fun->nombre = previous->text;

    if (!match(Token::LPAREN))
        throw runtime_error("Error sintactico: se esperaba '('");

    if (!check(Token::RPAREN)) {
        if (!match(Token::ID))
            throw runtime_error("Error sintactico: se esperaba tipo de parametro");
        string pTipo = previous->text;
        if (!match(Token::ID))
            throw runtime_error("Error sintactico: se esperaba nombre de parametro");
        fun->params.push_back(new Param(pTipo, previous->text));

        while (match(Token::COMA)) {
            if (!match(Token::ID))
                throw runtime_error("Error sintactico: se esperaba tipo de parametro");
            pTipo = previous->text;
            if (!match(Token::ID))
                throw runtime_error("Error sintactico: se esperaba nombre de parametro");
            fun->params.push_back(new Param(pTipo, previous->text));
        }
    }

    if (!match(Token::RPAREN))
        throw runtime_error("Error sintactico: se esperaba ')'");
    if (!match(Token::COLON))
        throw runtime_error("Error sintactico: se esperaba ':'");

    fun->cuerpo = parseBody();

    if (!match(Token::ENDFUN))
        throw runtime_error("Error sintactico: se esperaba 'endfun'");

    return fun;
}

Body* Parser::parseBody() {
    Body* b = new Body();
    while (check(Token::VAR)) {
        b->vdlist.push_back(parseVarDec());
        match(Token::SEMICOL);
    }
    if (!isBodyEnd()) {
        b->slist.push_back(parsestmt());
        while (match(Token::SEMICOL)) {
            if (isBodyEnd()) break;
            b->slist.push_back(parsestmt());
        }
    }
    return b;
}

VarDec* Parser::parseVarDec() {
    match(Token::VAR);
    VarDec* decl = new VarDec();
    if (!match(Token::ID))
        throw runtime_error("Error sintactico: se esperaba tipo en var");
    decl->tipo = previous->text;
    while (match(Token::ID)) {
        decl->chicharron.push_back(previous->text);
        match(Token::COMA);
    }
    return decl;
}

Stmt* Parser::parsestmt() {
    Exp* e;

    if (match(Token::PRINT)) {
        match(Token::LPAREN);
        e = parseCEXP();
        match(Token::RPAREN);
        return new PrintStmt(e);
    }

    if (match(Token::IF)) {
        e = parseCEXP();
        IfStmt* ifs = new IfStmt(e);
        match(Token::THEN);
        ifs->cuerpodelif = parseBody();
        if (match(Token::ELSE)) {
            ifs->hayelse = true;
            ifs->cuerpodelelse = parseBody();
        }
        match(Token::ENDIF);
        return ifs;
    }

    if (match(Token::WHILE)) {
        e = parseCEXP();
        WhileStmt* ws = new WhileStmt(e);
        match(Token::DO);
        ws->cuerpo = parseBody();
        match(Token::ENDWHILE);
        return ws;
    }

    if (match(Token::RETURN)) {
        Exp* retval = nullptr;
        if (!isBodyEnd() && !check(Token::SEMICOL))
            retval = parseCEXP();
        return new ReturnStmt(retval);
    }

    if (match(Token::BREAK))
        return new BreakStmt();

    if (match(Token::ID)) {
        string name = previous->text;

        if (match(Token::PLUSPLUS))
            return new IncrementStmt(name);

        if (match(Token::ASSIGN)) {
            e = parseCEXP();
            return new AsignStmt(name, e);
        }

        if (match(Token::LPAREN)) {
            FunCallExp* call = new FunCallExp(name);
            if (!check(Token::RPAREN)) {
                call->args.push_back(parseCEXP());
                while (match(Token::COMA))
                    call->args.push_back(parseCEXP());
            }
            match(Token::RPAREN);
            return new PrintStmt(call);
        }

        throw runtime_error("Error sintactico: se esperaba =, ++ o ( despues de '" + name + "'");
    }

    throw runtime_error("Error sintactico: statement inesperado");
}

Exp* Parser::parseCEXP() {
    Exp* l = parseLogAnd();
    while (match(Token::OR)) {
        Exp* r = parseLogAnd();
        l = new BinaryExp(l, r, OR_OP);
    }
    return l;
}

Exp* Parser::parseLogAnd() {
    Exp* l = parseNot();
    while (match(Token::AND)) {
        Exp* r = parseNot();
        l = new BinaryExp(l, r, AND_OP);
    }
    return l;
}

Exp* Parser::parseNot() {
    if (match(Token::NOT)) {
        Exp* e = parseNot();
        return new NotExp(e);
    }
    return parseComp();
}

Exp* Parser::parseComp() {
    Exp* l = parseAdd();
    if (match(Token::LESSTHAN) || match(Token::LESSTHANEQUAL) || match(Token::EQUALEQUAL)) {
        CBinaryOp op;
        switch (previous->type) {
            case Token::LESSTHAN:      op = LESSTHAN_OP;   break;
            case Token::LESSTHANEQUAL: op = LESSEQUAL_OP;  break;
            case Token::EQUALEQUAL:    op = EQUALEQUAL_OP; break;
            default:                   op = EQUALEQUAL_OP;
        }
        Exp* r = parseAdd();
        return new CBinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseAdd() {
    Exp* l = parseE();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op = (previous->type == Token::PLUS) ? PLUS_OP : MINUS_OP;
        Exp* r = parseE();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseE() {
    Exp* l = parseT();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op = (previous->type == Token::MUL) ? MUL_OP : DIV_OP;
        Exp* r = parseT();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseT() {
    Exp* l = parseF();
    if (match(Token::POW)) {
        Exp* r = parseF();
        return new BinaryExp(l, r, POW_OP);
    }
    return l;
}

Exp* Parser::parseF() {
    if (match(Token::NUM))
        return new NumberExp(stoi(previous->text));

    if (match(Token::TRUE))  return new NumberExp(1);
    if (match(Token::FALSE)) return new NumberExp(0);

    if (match(Token::ID)) {
        string name = previous->text;
        if (match(Token::LPAREN)) {
            FunCallExp* call = new FunCallExp(name);
            if (!check(Token::RPAREN)) {
                call->args.push_back(parseCEXP());
                while (match(Token::COMA))
                    call->args.push_back(parseCEXP());
            }
            match(Token::RPAREN);
            return call;
        }
        return new IdExp(name);
    }

    if (match(Token::LPAREN)) {
        Exp* e = parseCEXP();
        match(Token::RPAREN);
        return e;
    }

    if (match(Token::SQRT)) {
        match(Token::LPAREN);
        Exp* e = parseCEXP();
        match(Token::RPAREN);
        return new SqrtExp(e);
    }

    throw runtime_error("Error sintactico: expresion inesperada");
}
