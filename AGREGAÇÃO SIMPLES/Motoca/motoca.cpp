
#include <iostream>
#include <sstream>
#include <memory>
#include <aux.hpp>
using namespace std;

class Person {
    string name; 
    int age;
public:
    Person (string name = "", int age = 0){
        this->name = name;
        this->age = age;
    }
    Person (int age = 0){
        this->name = "";
        this->age = age;
    }
    
    int getAge() {
        return age;
    }
    string getName() {
        return name;
    }
    string str() {
        stringstream ss;
        ss << name << ":" << age;
        return ss.str();
    }
};
ostream& operator<<(ostream& os, Person& p) {
    return os << p.str();
}

class Motorcycle {
    shared_ptr<Person> person {nullptr};
    int time {0};
    int power {1};

public:
    Motorcycle(int power = 1) {
          this->power = power;
    }

    bool insertPerson(shared_ptr<Person> p) {
        if (person != nullptr){
            cout << "fail: busy motorcycle" << endl;
            return 1;
        }
        person = p;
        return 0;
    }

    string honk() {
        stringstream ss;
        ss << 'P' << string(power, 'e') << 'm';
        return ss.str();
    }

    shared_ptr<Person> remove() {
        if (person == nullptr){
            cout << "fail: empty motorcycle" << endl;
            return nullptr;
        }
        shared_ptr<Person> aux = person;
        person = nullptr;
        return aux; 
    }

    void buyTime(int time) {
        this->time += time;
        return;
    }

    void drive(int time) {
        if (this->time == 0){
            cout << "fail: buy time first" << endl;
            return;
        }
        if (person == nullptr){
            cout << "fail: empty motorcycle" << endl;
            return;
        }
        if (person->getAge() > 10){
            cout << "fail: too old to drive" << endl;
            return;
        }
        int drive = this->time - time;
        if (drive < 0){
            cout << "fail: time finished after " << this->time << " minutes" << endl;
            drive = 0;
        }
        this->time = drive;
        return;
    }

    

    string str() {
        ostringstream os;
        os << "power:" << power << ", time:" << time;
        os << ", person:(" << (person == nullptr ? "empty" : person->str()) << ")";
        return os.str();
    }
};

ostream& operator<<(ostream& os, Motorcycle m) {
    return os << m.str();
}


int main() {
    aux::Chain chain;
    aux::Param param;

    Motorcycle m(1);

    auto INT = aux::to<int>;

    chain["show"]  = [&]() { aux::show << m; };
    chain["leave"] = [&]() { 
        auto person = m.remove(); 
        if (person != nullptr) {
            aux::show << *person;
        }
    };
    chain["honk"]  = [&]() { aux::show << m.honk(); };
    chain["init"]  = [&]() { m = Motorcycle(INT(param.at(1)));};
    chain["enter"] = [&]() { m.insertPerson(make_shared<Person>(param.at(1), INT(param.at(2)))); };
    chain["buy"]   = [&]() { m.buyTime(INT(param.at(1))); };
    chain["drive"] = [&]() { m.drive  (INT(param.at(1))); };

    aux::execute(chain, param);
}