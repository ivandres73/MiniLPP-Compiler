#include <sstream>
#include <string>
#include "build/cfg.h"

extern CFGNodeType cfg;
int label_count = 0;

std::string gv_escape(const std::string& s) {
    std::ostringstream ss;

    for (auto ch : s) {
        if (ch == '\"') {
            ss << "\\\"";
        } else if (ch == '\n') {
            ss << "\\l";
        } else {
            ss << ch;
        }
    }

    return ss.str();
}

void CFGSetLabels(CFGStmt *root) {
    std::queue<CFGStmt *> q;
    std::unordered_set<CFGStmt *> visited;
    
    q.push(root);
    unsigned lbl_count = 0;
    while (!q.empty()) {
        CFGStmt *s = q.front();
        q.pop();

        if (visited.find(s) != visited.end()) {
            continue;
        }
        s->label = "LC" + std::to_string(lbl_count++);
        visited.insert(s);
        CFGStmtVector edges = s->getEdges();

        for (int i = 0; i < edges.size(); i++) {
            q.push(edges[i]);
        }
    }
}

static CFGStmt *CFGMakeBasicBlocksHelper(CFGStmt *root, std::unordered_map<CFGStmt *, CFGStmt *>& visited) {
    if (root == nullptr) {
        return nullptr;
    }
    if (visited.find(root) != visited.end()) {
        return visited[root];
    }
    CFGStmt *newStmt = root;
    CFGStmt *firstStmt = root;
    CFGStmt *lastStmt = root;

    //Try to merge statements
    CFGStmt *next = root->getNext();
    while (next != nullptr && next->pred_count <= 1) {
        lastStmt = next;
        next = next->getNext();
    }

    if (firstStmt != lastStmt) {
        newStmt = cfg.CFGBlockStmtCreate(firstStmt, lastStmt);
        newStmt->pred_count = firstStmt->pred_count;
    }
    visited[root] = newStmt;

    if (lastStmt->isA(CFGSimpleStmt_kind)) {
        CFGSimpleStmt *ss = static_cast<CFGSimpleStmt *>(lastStmt);
        ss->next = CFGMakeBasicBlocksHelper(ss->next, visited);
    } else if (lastStmt->isA(CFGCBRStmt_kind)) {
        CFGCBRStmt *cbrs = static_cast<CFGCBRStmt *>(lastStmt);
        cbrs->target_true = CFGMakeBasicBlocksHelper(cbrs->target_true, visited);
        cbrs->target_false = CFGMakeBasicBlocksHelper(cbrs->target_false, visited);
    }
    return newStmt;
}

CFGStmt *CFGMakeBasicBlocks(CFGStmt *root) {
    std::unordered_map<CFGStmt *, CFGStmt *> visited;

    return CFGMakeBasicBlocksHelper(root, visited);
}

std::string CFGToString(CFGStmt *root) 
{
    std::queue<CFGStmt *> q;
    std::unordered_set<CFGStmt *> visited;
    
    q.push(root);

    std::ostringstream out;
    while (!q.empty()) {
        CFGStmt *s = q.front();
        q.pop();

        if (visited.find(s) != visited.end()) {
            continue;
        }
        visited.insert(s);
        
        if (!s->label.empty()) {
            out << s->label << ": ";
        }
        out << s->toString();
        
        CFGStmtVector edges = s->getEdges();
        if (edges.size() == 1) {
            out << " : goto " << edges[0]->label;
        } else if (edges.size() == 0) {
            out << " : goto end"; 
        }
        out << '\n';

        for (int i = 0; i < edges.size(); i++) {
            q.push(edges[i]);
        }
    }
    out << "end:";

    return out.str();
}

static CFGStmt *CFGRemoveNopsHelper(CFGStmt *root, std::unordered_map<CFGStmt *, CFGStmt *>& visited) {
    if (root == nullptr) {
        return nullptr;
    }
    if (visited.find(root) != visited.end()) {
        return visited[root];
    }

    CFGStmt *stmt = root;
    if (root->isA(CFGNop_kind)) {
        if (root->getNext() == nullptr) {
            visited[root] = root;
            return root;
        }
        do {
            stmt = stmt->getNext();
        } while (stmt->isA(CFGNop_kind) && stmt->getNext() != nullptr);
        
        CFGStmt *stmt2 = root;
        do {
            visited[stmt2] = stmt;
            stmt2 = stmt2->getNext();
        } while (stmt2->isA(CFGNop_kind) && stmt2->getNext() != nullptr);
    }
    visited[root] = stmt;
    if (stmt->isA(CFGSimpleStmt_kind)) {
        CFGSimpleStmt *ss = static_cast<CFGSimpleStmt *>(stmt);
        ss->next = CFGRemoveNopsHelper(ss->next, visited);
    } else if (stmt->isA(CFGCBRStmt_kind)) {
        CFGCBRStmt *cbrs = static_cast<CFGCBRStmt *>(stmt);
        cbrs->target_true = CFGRemoveNopsHelper(cbrs->target_true, visited);
        cbrs->target_false = CFGRemoveNopsHelper(cbrs->target_false, visited);
    }
    return stmt;
}

CFGStmt *CFGRemoveNops(CFGStmt *root) 
{
    std::unordered_map<CFGStmt *, CFGStmt *> visited;

    return CFGRemoveNopsHelper(root, visited);
}