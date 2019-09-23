#include "x86.h"

string x86::CFGtox86(CFGStmt *root, Context& ctx) {
    bool prologo = false;
    string s(";main program\n\n");
    if (!ctx.strTable.empty()) {
        s += "section .data\n";
        for (auto i : ctx.strTable) {
            s += '\t' + i.second + ": db " + "\"" +
                 i.first + "\"" + ", 0\n";
        }
    }
    s += "section .text\n";
    s += "extern printf\n";
    s += "global main\n";
    s += "\nmain:\n";
    std::queue<CFGStmt *> q;
    std::unordered_set<CFGStmt *> visited;
    
    q.push(root);
    while (!q.empty()) {
        CFGStmt *curr = q.front();
        q.pop();

        if (visited.find(curr) != visited.end()) {
            continue;
        }
        visited.insert(curr);

        s += curr->genCode(ctx);

        CFGStmtVector edges = curr->getEdges();
        for (int i = 0; i < edges.size(); i++) {
            q.push(edges[i]);
        }
    }

    s += (prologo) ? "\tleave\n" : "";
    s += "\tret\n";

    ofstream out("testo.asm", ofstream::out | ofstream::trunc);
    out << s;
    out.close();
    
    return s;
}