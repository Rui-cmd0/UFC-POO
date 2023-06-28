

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <aux.hpp>
using namespace std;

class Pet{
private:
    int energyMax, hungryMax, cleanMax;
    int energy, hungry, clean;
    int diamonds = 0;
    int age = 0;
    bool alive = true;

    bool testAlive() {
        if (energy > 0 && hungry > 0 && clean > 0)
            return true;
        cout << "fail: pet esta morto" << '\n';
        return false;
    }

    void setHungry(int value) {
        hungry = value;
        if (value <= 0){
            hungry = 0;
            cout << "fail: pet morreu de fome" << endl;
            alive = false;
        }
        if (value > hungryMax)
            hungry = hungryMax;
    }
    void setEnergy(int value) {
        energy = value;
        if (value <= 0){
            energy = 0;
            cout << "fail: pet morreu de fraqueza" << endl;
            alive = false;
        }
        if (value > energyMax)
            energy = energyMax;
    }

    void setClean(int value) {
        clean = value;
        if (value <= 0){
            clean = 0;
            cout << "fail: pet morreu de sujeira" << endl;
            alive = false;
        }
        if (value > cleanMax)
            clean = cleanMax;
    }

public:
    Pet(int energy = 0, int hungry = 0, int clean = 0) { 
        energyMax = energy;
        hungryMax = hungry;
        cleanMax = clean;
        this->energy = energy;
        this->hungry = hungry;
        this->clean = clean;
    }

    void play() {
        if (!testAlive())
            return;
        setEnergy(energy - 2);
        setHungry(hungry - 1);
        setClean(clean - 3);
        age += 1;
        diamonds += 1;
        return;
    }

    void shower() { 
        if (!testAlive())
            return;
        setEnergy(energy - 3);
        setHungry(hungry - 1);
        setClean(cleanMax);
        age += 2;
        return;
    }

    void eat() { 
        if (!testAlive())
            return;
        setEnergy(energy - 1);
        setHungry(hungry + 4);
        setClean(clean - 2);
        age += 1;
        return;
    }

    void sleep() { 
        if (!testAlive())
            return;
        if (energy > energyMax - 5){
            cout << "fail: nao esta com sono" << endl;
            return;
        }
        setEnergy(energyMax);
        setHungry(hungry - 1);
        age += 5;
    }

    string toString() {
        stringstream ss;
        ss <<  "E:" << energy << "/" << energyMax << ", "
           <<  "S:" << hungry << "/" << hungryMax << ", "
           <<  "L:" << clean << "/" << cleanMax << ", "
           <<  "D:" << diamonds << ", " << "I:"  << age;
        return ss.str();
    }

};

int main() {
    aux::Chain chain;
    aux::Param ui;
    Pet pet;
    auto toint = aux::to<int>;
    chain["show"] = [&]() { cout << pet.toString() << '\n'; };
    chain["init"] = [&]() { pet = Pet(toint(ui[1]), toint(ui[2]), toint(ui[3])); };
    chain["play"] = [&]() { pet.play(); };
    chain["eat"]  = [&]() { pet.eat(); };
    chain["clean"] =[&]() { pet.shower(); };
    chain["sleep"] =[&]() { pet.sleep(); };
    
    aux::execute(chain, ui);
};