#include "ast.h"
#include <iostream>

using namespace std;

Exp::~Exp() {}

string Exp::binopToChar(BinaryOp op) {
    switch (op) {
        case PLUS_OP:  return "+";
        case MINUS_OP: return "-";
        case MUL_OP:   return "*";
        case DIV_OP:   return "/";
        case POW_OP:   return "**";
        case AND_OP:   return "and";
        case OR_OP:    return "or";
        default:       return "?";
    }
}

string Exp::CbinopToChar(CBinaryOp op) {
    switch (op) {
        case LESSTHAN_OP:   return "<";
        case LESSEQUAL_OP:  return "<=";
        case EQUALEQUAL_OP: return "==";
        default:            return "?";
    }
}

BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp o) : left(l), right(r), op(o) {}
BinaryExp::~BinaryExp() { delete left; delete right; }

CBinaryExp::CBinaryExp(Exp* l, Exp* r, CBinaryOp o) : left(l), right(r), op(o) {}
CBinaryExp::~CBinaryExp() { delete left; delete right; }

NumberExp::NumberExp(int v) : value(v) {}
NumberExp::~NumberExp() {}

IdExp::IdExp(string v) : value(v) {}
IdExp::~IdExp() {}

SqrtExp::SqrtExp(Exp* v) : value(v) {}
SqrtExp::~SqrtExp() { delete value; }

NotExp::NotExp(Exp* e) : expr(e) {}
NotExp::~NotExp() { delete expr; }

FunCallExp::FunCallExp(string n) : nombre(n) {}
FunCallExp::~FunCallExp() { for (auto a : args) delete a; }

Stmt::~Stmt() {}

PrintStmt::PrintStmt(Exp* e) : exp(e) {}
PrintStmt::~PrintStmt() { delete exp; }

AsignStmt::AsignStmt(string v, Exp* e) : variable(v), exp(e) {}
AsignStmt::~AsignStmt() { delete exp; }

IncrementStmt::IncrementStmt(string v) : variable(v) {}
IncrementStmt::~IncrementStmt() {}

ReturnStmt::ReturnStmt(Exp* e) : exp(e) {}
ReturnStmt::~ReturnStmt() { if (exp) delete exp; }

BreakStmt::BreakStmt() {}
BreakStmt::~BreakStmt() {}

IfStmt::IfStmt(Exp* e) : condicion(e), cuerpodelif(nullptr), cuerpodelelse(nullptr) {}
IfStmt::~IfStmt() {
    delete condicion;
    delete cuerpodelif;
    if (hayelse) delete cuerpodelelse;
}

WhileStmt::WhileStmt(Exp* e) : condicion(e), cuerpo(nullptr) {}
WhileStmt::~WhileStmt() { delete condicion; delete cuerpo; }

VarDec::VarDec() {}
VarDec::~VarDec() {}

Body::Body() {}
Body::~Body() {
    for (auto v : vdlist) delete v;
    for (auto s : slist)  delete s;
}

FunDec::FunDec() : cuerpo(nullptr) {}
FunDec::~FunDec() {
    for (auto p : params) delete p;
    delete cuerpo;
}

Programa::Programa() {}
Programa::~Programa() {
    for (auto v : globalVars) delete v;
    for (auto f : funciones)  delete f;
}
