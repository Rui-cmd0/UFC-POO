
#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <aux.hpp>
using namespace std;

class Pessoa {
    string nome;
public:
    Pessoa(string nome) { 
        this->nome = nome;
    }
    string getNome() const {
        return nome; 
    }
    string str() const {    
        return nome;
    }
};
ostream& operator<<(ostream& os, const Pessoa& p) {
    return (os << p.str());
}

class Mercantil {
    vector<shared_ptr<Pessoa>> caixas; 
    list  <shared_ptr<Pessoa>> esperando; 

    bool validarIndice(int indice) {
        if (indice < 0 || indice >= caixas.size()){
            return false;
        }
        return true;
    }

public:
    Mercantil(int qtd_caixas) { 
        for(int i = 0; i < qtd_caixas; i++){
            caixas.push_back(nullptr);
        }
    }
    
    void chegar(const shared_ptr<Pessoa>& person) {
        esperando.push_back(person);
    }

    bool chamarNoCaixa(int indice) {
        if(!validarIndice(indice)){
            cout << "fail: caixa inexistente" << endl;
            return false;
        }
        if(esperando.size() == 0){
            cout << "fail: sem clientes" << endl;
            return false;
        }
        if (caixas[indice] != nullptr){
           cout << "fail: caixa ocupado" << endl;
           return false;
       }
       
       caixas[indice] = esperando.front();
       esperando.pop_front();
       return true;
    }
    
    shared_ptr<Pessoa> finalizar(int indice) {
        if (!validarIndice(indice)){
            cout << "fail: caixa inexistente" << endl;
            return nullptr;
        }
        if (caixas[indice] == nullptr){
            cout << "fail: caixa vazio" << endl;
            return nullptr;
        }
        shared_ptr<Pessoa> aux = nullptr;
        swap(caixas[indice], aux);
        return aux; 
    }

    string str() const {
        int i = 0;
        auto fn = [&i](auto p) {
            stringstream ss; 
            ss << " " << i++ << ":" << (p == nullptr ? "-----" : p->getNome()) << " ";
            return ss.str();
        };
        stringstream os;
        os  << "Caixas: |" << (caixas | aux::MAP(fn) | aux::JOIN("|")) << "|\n"    
            << "Espera: " << (esperando | aux::MAP(FX(*x)) | aux::FMT());
        return os.str();
    }
};

ostream& operator<<(ostream& os, const Mercantil& b) {
    return (os << b.str());
}

int main() {
    aux::Chain chain;
    aux::Param par;
    
    Mercantil bank(0);

    chain["init"]   = [&]() {   bank = Mercantil(aux::to<int>(par[1])); };
    chain["call"]   = [&]() { bank.chamarNoCaixa(aux::to<int>(par[1])); };
    chain["finish"] = [&]() {     bank.finalizar(aux::to<int>(par[1])); };
    chain["arrive"] = [&]() { bank.chegar(make_shared<Pessoa>(par[1])); };
    chain["show"]   = [&]() { aux::show << bank; };

    aux::execute(chain, par);    
}
