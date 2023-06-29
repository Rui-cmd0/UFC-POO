
#include <iostream>
#include <list>
#include <sstream>
#include <memory>
#include <aux.hpp>
using namespace std;

class Kid {
private:
    int age;
    string name;
public:
    Kid(string name, int age) {
        this->age = age;
        this->name = name;
    }
    int getAge() {
        return age;
    }
    string getName() {
        return name;
    }
    string str() {
        return name + ":" + to_string(age);
    }
};

using PtrKid = shared_ptr<Kid>;

ostream& operator<<(ostream& os,  PtrKid kid) {
    return (os << kid->str());
}

class Trampoline {
    list<PtrKid> waiting;
    list<PtrKid> playing;
    
    PtrKid removeFromList(string name, list<PtrKid>& lista) {
        for (PtrKid elem : lista){
            if (elem->getName() == name){
                PtrKid aux = elem;
                lista.remove(elem);
                return aux;
            }    
        }
        return nullptr;
    }

public:
    void arrive(PtrKid kid) {
        waiting.push_front(kid);
    }

    void enter() {
        if(waiting.size() == 0)
            return;
        playing.push_front(waiting.back());
        waiting.remove(waiting.back());
    }

    void leave() {
        if(playing.size() == 0)
            return;
        waiting.push_front(playing.back());
        playing.remove(playing.back());
    }

    PtrKid removeKid(string name) {
        PtrKid aux = removeFromList(name, waiting);
        if (aux == nullptr) 
            aux = removeFromList(name, playing);
        return aux;
    }
    string str() {
        return (waiting | aux::FMT()) + " => " + (playing | aux::FMT());
    }
};

int main() {
    aux::Chain chain;
    aux::Param param;
    Trampoline tr;
    chain["arrive"] = [&]() { tr.arrive(make_shared<Kid>(param[1], aux::to<int>(param[2]))); };
    chain["enter"]  = [&]() { tr.enter(); };
    chain["leave"]  = [&]() { tr.leave(); };
    chain["remove"] = [&]() { tr.removeKid(param[1]); };
    chain["show"]   = [&]() { cout << tr.str() << endl; };

    aux::execute(chain, param);
}