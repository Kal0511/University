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

class Record {
public:

    string name;
    const char *type;
    bool isConst;
    bool isType;
    vector<string> inputType;

    Record(string name, string type, bool isConst, bool isType, vector<string> inputs = {}) : name(std::move(name)), type(std::move(type)), isConst(isConst), isType(isType), inputType(std::move(inputs)) {}
};

class SymbolTable {
public:
    vector<unordered_map<string, Record>> scopeStack;
    unordered_map<string, vector<vector<string>>> typeMap;

    SymbolTable() : scopeStack(), typeMap() {
        unordered_map<string, Record> universe;
        universe.insert(make_pair("void", Record("void", "void", false, true)));
        universe.insert(make_pair("bool", Record("bool", "bool", false, true)));
        universe.insert(make_pair("int", Record("int", "int", false, true)));
        universe.insert(make_pair("string", Record("string", "string", false, true)));

        universe.insert(make_pair("true", Record("true", "bool", true, false)));
        universe.insert(make_pair("false", Record("false", "bool", true, false)));

        universe.insert(make_pair("getchar", Record("getchar", "int", false, false)));
        universe.insert(make_pair("halt", Record("halt", "void", false, false)));
        universe.insert(make_pair("len", Record("len", "int", false, false, {"int"})));
        universe.insert(make_pair("printb", Record("printb", "void", false, false, {"bool"})));
        universe.insert(make_pair("printc", Record("printc", "void", false, false, {"int"})));
        universe.insert(make_pair("printi", Record("printi", "void", false, false, {"int"})));
        universe.insert(make_pair("prints", Record("prints", "void", false, false, {"string"})));
        scopeStack.emplace_back(universe);
        scopeStack.emplace_back();

        typeMap.insert(make_pair<string, vector<vector<string>>>("==", {
                {"int",    "int",    "bool"},
                {"string", "string", "bool"},
                {"bool",   "bool",   "bool"}
        }));
        typeMap.insert(make_pair<string, vector<vector<string>>>("!=", {
                {"int",    "int",    "bool"},
                {"string", "string", "bool"},
                {"bool",   "bool",   "bool"}
        }));

        typeMap.insert(make_pair<string, vector<vector<string>>>("&&", {
                {"int",  "int",  "bool"},
                {"bool", "bool", "bool"}
        }));
        typeMap.insert(make_pair<string, vector<vector<string>>>("||", {
                {"int",  "int",  "bool"},
                {"bool", "bool", "bool"}
        }));

        typeMap.insert(make_pair<string, vector<vector<string>>>("<=", {
                {"int", "int", "bool"},
        }));
        typeMap.insert(make_pair<string, vector<vector<string>>>(">=", {
                {"int", "int", "bool"},
        }));
        typeMap.insert(make_pair<string, vector<vector<string>>>("<", {
                {"int", "int", "bool"},
        }));
        typeMap.insert(make_pair<string, vector<vector<string>>>(">", {
                {"int", "int", "bool"},
        }));

        typeMap.insert(make_pair<string, vector<vector<string>>>("+", {{"int", "int", "int"}}));
        typeMap.insert(make_pair<string, vector<vector<string>>>("-", {{"int", "int", "int"}}));
        typeMap.insert(make_pair<string, vector<vector<string>>>("/", {{"int", "int", "int"}}));
        typeMap.insert(make_pair<string, vector<vector<string>>>("*", {{"int", "int", "int"}}));
    }

    void define(const string &name, const string &type, bool isConst, bool isType, int lineno) {
        auto record = scopeStack.back().find(name);
        if (record != scopeStack.back().end()) {
            cerr << "Error: redefining token {" << name << "} at line " << lineno << endl;
            exit(1);
        }
        scopeStack.back().insert(make_pair(name, Record(name, type, isConst, isType)));
    }

    Record lookup(const string &name) {
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
                cout << record.first << " " << record.second.type << endl;
            }
            cout << endl;
        }
    }
};

#endif //CPSC_411_SYMBOLTABLE_H
