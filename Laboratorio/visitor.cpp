#include <iostream>
#include <fstream>
#include <cmath>
#include "ast.h"
#include "visitor.h"

using namespace std;

int BinaryExp::accept(Visitor* v)      { return v->visit(this); }
int CBinaryExp::accept(Visitor* v)     { return v->visit(this); }
int NumberExp::accept(Visitor* v)      { return v->visit(this); }
int IdExp::accept(Visitor* v)          { return v->visit(this); }
int SqrtExp::accept(Visitor* v)        { return v->visit(this); }
int NotExp::accept(Visitor* v)         { return v->visit(this); }
int FunCallExp::accept(Visitor* v)     { return v->visit(this); }
void PrintStmt::accept(Visitor* v)     { v->visit(this); }
void AsignStmt::accept(Visitor* v)     { v->visit(this); }
void IncrementStmt::accept(Visitor* v) { v->visit(this); }
void ReturnStmt::accept(Visitor* v)    { v->visit(this); }
void BreakStmt::accept(Visitor* v)     { v->visit(this); }
void IfStmt::accept(Visitor* v)        { v->visit(this); }
void WhileStmt::accept(Visitor* v)     { v->visit(this); }
void VarDec::accept(Visitor* v)        { v->visit(this); }
void Body::accept(Visitor* v)          { v->visit(this); }
void Programa::accept(Visitor* v)      { v->visit(this); }

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(CBinaryExp* exp) {
    if (exp->negated) cout << "not (";
    exp->left->accept(this);
    cout << ' ' << Exp::CbinopToChar(exp->op) << ' ';
    exp->right->accept(this);
    if (exp->negated) cout << ")";
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) { cout << exp->value; return 0; }
int PrintVisitor::visit(IdExp* exp)     { cout << exp->value; return 0; }

int PrintVisitor::visit(SqrtExp* exp) {
    cout << "sqrt(";
    exp->value->accept(this);
    cout << ")";
    return 0;
}

int PrintVisitor::visit(NotExp* exp) {
    cout << "not ";
    exp->expr->accept(this);
    return 0;
}

int PrintVisitor::visit(FunCallExp* exp) {
    cout << exp->nombre << "(";
    bool first = true;
    for (auto a : exp->args) {
        if (!first) cout << ", ";
        a->accept(this);
        first = false;
    }
    cout << ")";
    return 0;
}

void PrintVisitor::visit(AsignStmt* stm) {
    cout << stm->variable << " = ";
    stm->exp->accept(this);
    cout << endl;
}

void PrintVisitor::visit(PrintStmt* stm) {
    cout << "print(";
    stm->exp->accept(this);
    cout << ")" << endl;
}

void PrintVisitor::visit(IncrementStmt* stm) { cout << stm->variable << "++" << endl; }

void PrintVisitor::visit(ReturnStmt* stm) {
    cout << "return";
    if (stm->exp) { cout << " "; stm->exp->accept(this); }
    cout << endl;
}

void PrintVisitor::visit(BreakStmt* stm) { (void)stm; cout << "break" << endl; }

void PrintVisitor::visit(IfStmt* stm) {
    cout << "if ";
    stm->condicion->accept(this);
    cout << " then" << endl;
    stm->cuerpodelif->accept(this);
    if (stm->hayelse) {
        cout << "else" << endl;
        stm->cuerpodelelse->accept(this);
    }
    cout << "endif" << endl;
}

void PrintVisitor::visit(WhileStmt* stm) {
    cout << "while ";
    stm->condicion->accept(this);
    cout << " do" << endl;
    stm->cuerpo->accept(this);
    cout << "endwhile" << endl;
}

void PrintVisitor::visit(VarDec* decl) {
    cout << "var " << decl->tipo << " ";
    bool first = true;
    for (auto& n : decl->chicharron) {
        if (!first) cout << ", ";
        cout << n;
        first = false;
    }
    cout << endl;
}

void PrintVisitor::visit(Body* b) {
    for (auto v : b->vdlist) v->accept(this);
    for (auto s : b->slist)  s->accept(this);
}

void PrintVisitor::visit(Programa* p) {
    for (auto v : p->globalVars) v->accept(this);
    for (auto f : p->funciones) {
        cout << "fun " << f->retorno << " " << f->nombre << "(";
        bool first = true;
        for (auto pr : f->params) {
            if (!first) cout << ", ";
            cout << pr->tipo << " " << pr->nombre;
            first = false;
        }
        cout << "):" << endl;
        f->cuerpo->accept(this);
        cout << "endfun" << endl;
    }
}

void PrintVisitor::imprimir(Programa* programa) {
    if (programa) {
        cout << "Codigo:" << endl;
        programa->accept(this);
        cout << endl;
    }
}

int EVALVisitor::visit(BinaryExp* exp) {
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch (exp->op) {
        case PLUS_OP:  return v1 + v2;
        case MINUS_OP: return v1 - v2;
        case MUL_OP:   return v1 * v2;
        case DIV_OP:
            if (v2 == 0) { cerr << "Error: division por cero" << endl; return 0; }
            return v1 / v2;
        case POW_OP:   return (int)pow(v1, v2);
        case AND_OP:   return (v1 != 0 && v2 != 0) ? 1 : 0;
        case OR_OP:    return (v1 != 0 || v2 != 0) ? 1 : 0;
        default:       return 0;
    }
}

int EVALVisitor::visit(CBinaryExp* exp) {
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    int result;
    switch (exp->op) {
        case LESSTHAN_OP:   result = (v1 <  v2) ? 1 : 0; break;
        case LESSEQUAL_OP:  result = (v1 <= v2) ? 1 : 0; break;
        case EQUALEQUAL_OP: result = (v1 == v2) ? 1 : 0; break;
        default: result = 0;
    }
    return exp->negated ? (result ? 0 : 1) : result;
}

int EVALVisitor::visit(NumberExp* exp) { return exp->value; }
int EVALVisitor::visit(IdExp* exp)     { return memoria.lookup(exp->value); }

int EVALVisitor::visit(SqrtExp* exp) {
    return (int)floor(sqrt(exp->value->accept(this)));
}

int EVALVisitor::visit(NotExp* exp) {
    return exp->expr->accept(this) != 0 ? 0 : 1;
}

int EVALVisitor::visit(FunCallExp* call) {
    auto it = funciones.find(call->nombre);
    if (it == funciones.end()) {
        cerr << "Error: funcion no definida: " << call->nombre << endl;
        return 0;
    }
    FunDec* fun = it->second;

    list<int> argVals;
    for (auto a : call->args)
        argVals.push_back(a->accept(this));

    memoria.add_level();
    auto paramIt = fun->params.begin();
    auto valIt   = argVals.begin();
    for (; paramIt != fun->params.end() && valIt != argVals.end(); ++paramIt, ++valIt)
        memoria.add_var((*paramIt)->nombre, *valIt);

    int result = 0;
    try {
        fun->cuerpo->accept(this);
    } catch (const ReturnException& r) {
        result = r.value;
    } catch (...) {
        memoria.remove_level();
        throw;
    }
    memoria.remove_level();
    return result;
}

void EVALVisitor::visit(AsignStmt* stm) {
    int val = stm->exp->accept(this);
    if (!memoria.update(stm->variable, val))
        memoria.add_var(stm->variable, val);
}

void EVALVisitor::visit(PrintStmt* stm) {
    cout << stm->exp->accept(this) << endl;
}

void EVALVisitor::visit(IncrementStmt* stm) {
    memoria.update(stm->variable, memoria.lookup(stm->variable) + 1);
}

void EVALVisitor::visit(ReturnStmt* stm) {
    int val = stm->exp ? stm->exp->accept(this) : 0;
    throw ReturnException(val);
}

void EVALVisitor::visit(BreakStmt* stm) {
    (void)stm;
    throw BreakException();
}

void EVALVisitor::visit(IfStmt* stm) {
    if (stm->condicion->accept(this) != 0)
        stm->cuerpodelif->accept(this);
    else if (stm->hayelse)
        stm->cuerpodelelse->accept(this);
}

void EVALVisitor::visit(WhileStmt* stm) {
    while (stm->condicion->accept(this) != 0) {
        try {
            stm->cuerpo->accept(this);
        } catch (const BreakException&) {
            break;
        }
    }
}

void EVALVisitor::visit(VarDec* decl) {
    for (auto& n : decl->chicharron)
        memoria.add_var(n);
}

void EVALVisitor::visit(Body* p) {
    memoria.add_level();
    try {
        for (auto v : p->vdlist) v->accept(this);
        for (auto s : p->slist)  s->accept(this);
    } catch (...) {
        memoria.remove_level();
        throw;
    }
    memoria.remove_level();
}

void EVALVisitor::visit(Programa* p) {
    memoria.add_level();
    try {
        for (auto v : p->globalVars) v->accept(this);
        for (auto f : p->funciones)  funciones[f->nombre] = f;
        auto it = funciones.find("main");
        if (it != funciones.end()) {
            try {
                it->second->cuerpo->accept(this);
            } catch (const ReturnException&) {}
        } else {
            cerr << "Error: no se encontro la funcion main" << endl;
        }
    } catch (...) {
        memoria.remove_level();
        throw;
    }
    memoria.remove_level();
}

void EVALVisitor::interprete(Programa* programa) {
    if (programa) {
        cout << "Interprete:" << endl;
        programa->accept(this);
        cout << endl;
    }
}
