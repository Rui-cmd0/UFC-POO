
#include <iostream>
#include <vector>
#include <utility>
#include <aux.hpp>
using namespace std;

enum Coin {M10, M25, M50, M100};
struct CoinDef {
    float value;
    int volume;
};

CoinDef getDef(Coin coin) {
    switch (coin) {
        case M10: return {0.1, 1};
        case M25: return {0.25, 2};
        case M50: return {0.50, 3};
        case M100: return {1, 4};
    }
    return {0, 0};
}

class Item {
    string label;
    int volume;
public:
    Item(string label, int volume) { 
        this->label = label;
        this->volume = volume;
    }
    int getVolume() const {
        return volume;
    }
    string getLabel() const {
        return label; 
    }
    void setVolume(int volume) {
        this->volume = volume;
    }
    void setLabel(string label) {
        this->label = label;
    }
    string str() const {
        // ??????
    }
};

ostream& operator<<(ostream& os, const Item& item) {
    return os << item.str();
}

class Pig {
    vector<string> itens;
    double value {0};
    int volume {0};
    int volumeMax {0};
    bool broken {false};

public:
    Pig(int volumeMax = 0) {
        this-> volumeMax = volumeMax;
    }
    
    bool isBroken (string cases){
        if (cases == "add" && broken)
            cout << "fail: the pig is broken" << endl;
        if (cases == "get" && !broken)
            cout << "fail: you must break the pig first" << endl;
        if (broken) return true;
        if (!broken) return false;
    }
    
    bool hasSpace (int addVol){
        if (addVol + volume <= volumeMax)
            return true;
        else {
            cout << "fail: the pig is full" << endl;
            return false;
        }
    }
    
    void addCoin(Coin coin) {
        if (isBroken("add"))
            return;
        if (!hasSpace(getDef(coin).volume))
            return;
        else
            value += getDef(coin).value;
            volume += getDef(coin).volume;
        return;
    }

    void addItem(Item item) {
        if (isBroken("add"))
            return;
        if (!hasSpace (item.getVolume()))
            return;
        else
            itens.push_back(item.getLabel());
            volume += item.getVolume();
        return;
    } 

    void breakPig() {
        volume = 0;
        broken = true;
    }

    double getCoins() {
        if (!isBroken("get"))
            return 0;
        double coins = value;
        value = 0;
        return coins;
    }

    string getItens() {
        if (!isBroken("get"))
            return "[]";
        string allitens = "[";
        for (int i = 0; i < itens.size(); i++){
            if (i != 0)
                allitens += ", ";
            allitens += itens[i];
        }
        itens.clear();
        allitens += "]";
        return allitens;
    }

    string str() const {
        stringstream ss;
        ss << aux::fmt(this->itens) << " : "
           << aux::fmt(value) << "$ : "
           << volume <<  "/" << volumeMax << " : " 
           <<  (broken ? "broken" : "unbroken");
        return ss.str();
    }
};

ostream& operator<<(ostream& os, const Pig& pig) {
    return os << pig.str();
}

int main() {
    aux::Chain chain;
    aux::Param par;

    Pig pig;
    
    auto toint = aux::to<int>;

    chain["addCoin"] = [&]() { 
        if      (par[1] == "10") pig.addCoin(M10);
        else if (par[1] == "25") pig.addCoin(M25);
        else if (par[1] == "50") pig.addCoin(M50);
        else if (par[1] == "100") pig.addCoin(M100);
    };
    chain["init"]     = [&]() { pig = Pig(toint(par[1])); };
    chain["addItem"]  = [&]() { pig.addItem(Item(par[1], toint(par[2]))); };
    chain["break"]    = [&]() { pig.breakPig(); };
    chain["getCoins"] = [&]() { aux::show << pig.getCoins(); };
    chain["getItens"] = [&]() { aux::show << pig.getItens(); };
    chain["show"]     = [&]() { aux::show << pig.str(); };

    aux::execute(chain, par);
}