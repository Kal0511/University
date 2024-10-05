//
// Created by kal on 18/03/23.
//

#ifndef CPSC_411_SYMBOLTABLE_H
#define CPSC_411_SYMBOLTABLE_H

#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;


// Used by the semantic checker to keep track of nodes in the AST
class Record {
public:

    string name;
    string type;
    bool isConst;
    bool isType;
    vector <string> inputType;
    int num;
    bool isGlobal;

    Record(string name, string type, bool isConst, bool isType, vector <string> inputs = {}, bool isGlobal = false) : name(std::move(name)), type(std::move(type)), isConst(isConst), isType(isType), inputType(std::move(inputs)), num(),
                                                                                                                      isGlobal(isGlobal) {}

    void print() {
        cout << name << " " << num << " " << type << isGlobal << endl;
    }
};

class SymbolTable {
public:
    vector <unordered_map<string, Record *>> scopeStack;
    unordered_map <string, vector<vector < string>>>
    typeMap;

    SymbolTable() : scopeStack(), typeMap() {
        unordered_map < string, Record * > universe;
        universe.insert(make_pair("void", new Record("void", "void", false, true)));
        universe.insert(make_pair("bool", new Record("bool", "bool", false, true)));
        universe.insert(make_pair("int", new Record("int", "int", false, true)));
        universe.insert(make_pair("string", new Record("string", "string", false, true)));

        universe.insert(make_pair("true", new Record("true", "bool", true, false)));
        universe.insert(make_pair("false", new Record("false", "bool", true, false)));

        universe.insert(make_pair("getchar", new Record("getchar", "int", false, false)));
        universe.insert(make_pair("halt", new Record("halt", "void", false, false)));
        universe.insert(make_pair("len", new Record("len", "int", false, false, {"string"})));
        universe.insert(make_pair("printb", new Record("printb", "void", false, false, {"bool"})));
        universe.insert(make_pair("printc", new Record("printc", "void", false, false, {"int"})));
        universe.insert(make_pair("printi", new Record("printi", "void", false, false, {"int"})));
        universe.insert(make_pair("prints", new Record("prints", "void", false, false, {"string"})));
        scopeStack.emplace_back(universe);
        scopeStack.emplace_back();

        typeMap.insert(make_pair < string, vector < vector < string>>>("==", {
                {"int",    "int",    "bool"},
                {"string", "string", "bool"},
                {"bool",   "bool",   "bool"}
        }));
        typeMap.insert(make_pair < string, vector < vector < string>>>("!=", {
                {"int",    "int",    "bool"},
                {"string", "string", "bool"},
                {"bool",   "bool",   "bool"}
        }));

        typeMap.insert(make_pair < string, vector < vector < string>>>("&&", {
                {"bool", "bool", "bool"}
        }));
        typeMap.insert(make_pair < string, vector < vector < string>>>("||", {
                {"bool", "bool", "bool"}
        }));

        typeMap.insert(make_pair < string, vector < vector < string>>>("<=", {
                {"int",    "int",    "bool"},
                {"string", "string", "bool"},
        }));
        typeMap.insert(make_pair < string, vector < vector < string>>>(">=", {
                {"int",    "int",    "bool"},
                {"string", "string", "bool"},
        }));
        typeMap.insert(make_pair < string, vector < vector < string>>>("<", {
                {"int",    "int",    "bool"},
                {"string", "string", "bool"},
        }));
        typeMap.insert(make_pair < string, vector < vector < string>>>(">", {
                {"int",    "int",    "bool"},
                {"string", "string", "bool"},
        }));

        typeMap.insert(make_pair < string, vector < vector < string>>>("+", {{"int", "int", "int"}}));
        typeMap.insert(make_pair < string, vector < vector < string>>>("-", {{"int", "int", "int"}}));
        typeMap.insert(make_pair < string, vector < vector < string>>>("/", {{"int", "int", "int"}}));
        typeMap.insert(make_pair < string, vector < vector < string>>>("*", {{"int", "int", "int"}}));
        typeMap.insert(make_pair < string, vector < vector < string>>>("%", {{"int", "int", "int"}}));
    }

    void define(const string &name, const string &type, bool isConst, bool isType, int lineno, bool isGlobal = false) {
        auto record = scopeStack.back().find(name);
        if (record != scopeStack.back().end()) {
            cerr << "Error: redefining token {" << name << "} at line " << lineno << endl;
            exit(1);
        }
        scopeStack.back().insert(make_pair(name, new Record(name, type, isConst, isType, {}, isGlobal)));
    }

    Record *lookup(const string &name) {
        for (int i = scopeStack.size() - 1; i >= 0; i--) {
            auto record = scopeStack[i].find(name);
            if (record != scopeStack[i].end()) {
                return record->second;
            }
        }

        cerr << "Error: cant find token {" << name << endl;
        exit(1);

    }

    void pushScope() {
        scopeStack.emplace_back();
    }

    void popScope() {
        scopeStack.pop_back();
    }

    void print() {
        for (const auto &scope: scopeStack) {
            for (const auto &record: scope) {
                cout << record.first << " " << record.second->type << endl;
            }
            cout << endl;
        }
    }
};

#endif //CPSC_411_SYMBOLTABLE_H
