
#include <sstream>
#include <memory>  //sharedptr
#include <iomanip> //setprecision
#include <utility> //exchange
#include <aux.hpp>
using namespace std;

class Lead {
    float thickness;
    string hardness;
    int size;

public:
    Lead(float thickness, string hardness, int size) {
        this->thickness = thickness;
        this->hardness = hardness;
        this->size = size;
    }

    int usagePerSheet() const {
        if (hardness[0] == 'H') 
            return 1;
        return int(hardness[0] - 48);
    }

    float getThickness() const {
        return thickness; 
    }

    string getHardness() const {
        return hardness; 
    }

    int getSize() const {
        return size; 
    }

    void setSize(int size) {
        this->size = size;
    }
    string str() const {
        stringstream ss;
        ss << fixed << setprecision(1) 
           << thickness << ":" << hardness << ":" << size;
        return ss.str();
    }
};

ostream& operator<<(ostream& ss, Lead gr) {
    return ss << gr.str();
}

struct Pencil {
    float thickness {0.f};
    shared_ptr<Lead> tip {nullptr};
    Pencil(float thickness = 0.0) {  
        this-> thickness = thickness;
    }

    bool hasGrafite() {
        if (tip != nullptr)
            return true;
        return false; 
    }

    bool insert(shared_ptr<Lead> grafite) {
        if (this->thickness != grafite->getThickness()){
            cout << "fail: calibre incompativel" << endl;
            return 1;
        }
        if (hasGrafite()){
            cout << "fail: ja existe grafite" << endl;
            return 1;
        }
        tip = grafite;
        return 0;
    }

    shared_ptr<Lead> remove() {
        if (!hasGrafite()){
            cout << "fail: nao existe grafite" << endl;
            return nullptr;
        }
        shared_ptr<Lead> aux = tip;
        tip = nullptr;
        return aux;
    }

    void writePage() {
        if (!hasGrafite()){
            cout << "fail: nao existe grafite" << endl;
            return;
        }
        int size = tip->getSize();
        if (size <= 10){
            cout << "fail: tamanho insuficiente" << endl;
            return;
        }
        size -= tip->usagePerSheet();
        if (size < 10){
            cout << "fail: folha incompleta" << endl;
            size = 10;
        }
        tip->setSize(size);
    }

    string str() {
        stringstream ss;
        ss << "calibre: " << thickness << ", grafite: ";
        if (tip != nullptr)
            ss << "[" << *tip << "]";
        else
            ss << "null";
        return ss.str();
    }
};

ostream& operator<<(ostream& os, Pencil l) {
    return os << l.str();
}

int main() {
    aux::Chain chain;
    aux::Param ui;

    Pencil pencil;

    auto tofloat = aux::to<float>;
    auto toint   = aux::to<int>;

    chain["show"]   = [&]() { cout << pencil << endl; };
    chain["init"]   = [&]() { pencil = Pencil(tofloat(ui[1])); };
    chain["insert"] = [&]() { pencil.insert(make_shared<Lead>(tofloat(ui[1]), ui[2], toint(ui[3]))); };
    chain["remove"] = [&]() { pencil.remove(); };
    chain["write"]  = [&]() { pencil.writePage(); };

    aux::execute(chain, ui);
}