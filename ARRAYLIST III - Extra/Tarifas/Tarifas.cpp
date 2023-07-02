

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <iomanip>
#include <utility>
#include <aux.hpp>
using namespace std;

enum LabelOp {DEPOSIT, FEE, OPENING, REVERSE, WITHDRAW};

string getDef(LabelOp op) {
    switch (op) {
        case DEPOSIT:  return "deposit";
        case FEE:      return "fee";
        case OPENING:  return "opening";
        case REVERSE:  return "reverse";
        case WITHDRAW: return "withdraw";
    }
    return "";
}

class Operation {
    int index;
    LabelOp op;
    int value;
    int balance;
    
public:
    Operation (int index, LabelOp op, int value, int balance){
        this->index = index;
        this->op = op;
        this->value = value;
        this->balance = balance;
    }
    
    int getBalance() {
        return balance;}
    
    int getIndex() {
        return index;}
    
    LabelOp getLabel() {
        return op;}
    
    int getValue() {
        return value;}
        
    string OptoString() {
        stringstream ss;
        ss << setw(2) << index << ":" << setw(9) << getDef(op) << ":" 
        << setw(5) << value << ":" << setw(5) << balance;
        return ss.str();
    }
};

class BalanceManager {
    int balance{0};
    int nextId {1};
    vector<Operation> extract;
    
public:
    BalanceManager(){
        extract.push_back(Operation(0, OPENING, 0, 0));}
        
    void addOperation(int value, LabelOp op){
        balance += value;
        extract.push_back(Operation(nextId, op, value, balance));
        nextId += 1;
    }
    int getBalance() {
        return balance;}
        
    vector<Operation> getExtract (int qtdOP) {
        if (qtdOP == 0){
            return extract;}
        vector<Operation> extslice(extract.end() - qtdOP, extract.end());
        return extslice;
    }
};

class Account {
    BalanceManager manager;
    int id;
public:
    Account (int id = 0) {
        this->id = id;
    }
    void deposit (int value) {
        if (value < 0) {
            cout << "fail: invalid value\n";
            return;
        }
        manager.addOperation(value, DEPOSIT);
    }
    void fee (int value) {
        manager.addOperation(-value, FEE);}
    
    void reverse (int index) {
        if (index < 0 || index >= manager.getExtract(0).size()){
            cout << "fail: index " << index << " invalid\n";
            return;
        }
        Operation opera = manager.getExtract(0)[index];
        if (opera.getLabel() != FEE){
            cout << "fail: index " << index << " is not a fee\n";
            return;
        }
        int value = opera.getValue();
        manager.addOperation(-value, REVERSE);
    }
    
    void withdraw (int value) {
        if (manager.getBalance() < value){
            cout << "fail: insufficient balance\n";
            return;
        }
        manager.addOperation(-value, WITHDRAW);
    }
    
    string toString() {
        stringstream ss;
        ss << "account:" << id << " balance:" << manager.getBalance() << endl;
        return ss.str();
    }
    BalanceManager getBalMan() {
        return manager;
    }
};

int main () {
    Account acc;
    while(true){
        string action; int amount;
        getline(cin, action);
        cout << "$" << action << endl;
        stringstream ss(action);
        ss >> action >> amount;

        if (action == "init") acc = Account(amount);
        if (action == "show") cout << acc.toString();
        if (action == "deposit") acc.deposit(amount);
        if (action == "fee") acc.fee(amount);
        if (action == "withdraw") acc.withdraw(amount);
        if (action == "reverse"){
            acc.reverse(amount);
            while (ss >> amount) acc.reverse(amount);
        }
        if (action == "extract") {
            vector<Operation> ext = acc.getBalMan().getExtract(amount);
            for(auto elem : ext) cout << elem.OptoString() << endl;
        }
        if (action == "end") 
            return 0;
    }
}