#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <algorithm>
#include "help.h"
using namespace std;

vector<int> get_men(vector<int> vet) {
    vector <int> men {};
    for (int i = 0; i < vet.size(); i++){
        if(vet[i] > 0)
            men.push_back(vet[i]);
    }
    return men;
}

vector<int> get_calm_women(vector<int> vet) {
    vector <int> calm {};
    for (int i = 0; i < vet.size(); i++)
    {
        if (vet[i] > -10 && vet[i] < 0)
            calm.push_back(vet[i]);
    }
    return calm;
}

vector<int> sort(vector<int> vet) {
    for (int i = 0; i < vet.size()-1; i++)
    {
        for (int j = i+1; j < vet.size(); j++)
        {
            if (vet[i] > vet[j])
                swap(vet[i], vet[j]);
        }
    }
    return vet;
}

vector<int> sort_stress(vector<int> vet) {  
    for (int i = 0; i < vet.size(); i++)
    {
        for (int j = i+1; j < vet.size()-1; j++)
        {
            if (abs(vet[i]) > abs(vet[j]))
                swap(vet[i], vet[j]);
        }
    }
    return vet;
}

vector<int> reverse(vector<int> vet) {
    vector <int> reversed {};
    for (int i = vet.size()-1; i >= 0; i--){
        reversed.push_back(vet[i]);
    }
    return reversed;
}

void reverse_inplace(vector<int>& vet) {
    int size = vet.size();
    for (int i = size-1; i >= 0; i--)
        vet.push_back(vet[i]);
    for (int i = 0; i < size; i++)
        vet.erase(vet.begin());
}

bool exists(vector<int> vet, int index){
    for (int i = 0; i < index; i++){
        if (vet[i] == vet[index])
            return true;
    }
    return false;
}

vector <int> unique(vector<int> vet) {
    vector <int> unique = {};
    for (int i = 0; i < vet.size(); i++){
        if (!exists(vet, i)){
            unique.push_back(vet[i]);
        }
    }
    return unique;
}

vector <int> repeated(vector<int> vet) {   
    vector <int> repeat = {};
    for (int i = 0; i < vet.size(); i++){
        if (exists(vet, i))
            repeat.push_back(vet[i]);
    }
    return repeat;
}

//loop principal
int main(){
    while(true) {
        string line{};
        getline(cin, line);
        print("$" + line);
        auto ui = split(line);
        auto cmd = ui[0];
        if (cmd == "end") {
            break;
        } else if (cmd == "get_men") {
            print(fmt(get_men(to_vet(ui[1]))));
        } else if (cmd == "get_calm_women") {
            print(fmt(get_calm_women(to_vet(ui[1]))));
        } else if (cmd == "sort") {
            print(fmt(sort(to_vet(ui[1]))));
        } else if (cmd == "sort_stress") {
            print(fmt(sort_stress(to_vet(ui[1]))));
        } else if (cmd == "reverse") {
            print(fmt(reverse(to_vet(ui[1]))));
        } else if (cmd == "reverse_inplace") {
            auto vet = to_vet(ui[1]);
            reverse_inplace(vet);
            print(fmt(vet));
        } else if (cmd == "unique") {
            print(fmt(unique(to_vet(ui[1]))));
        } else if (cmd == "repeated") {
            print(fmt(repeated(to_vet(ui[1]))));
        } else {
            cout << "unknown command: " << cmd << '\n';
        }
    }
    return 0;
}
