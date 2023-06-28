
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

struct Calculator {
    int batteryMax;
    int battery;
    float display;

    //Inicia os atributos, battery e display começam com o zero.
    Calculator(int batterymax){
        batteryMax = batterymax;
        battery = 0;
        display = 0;
    }

    //Aumenta a bateria, porém não além do máximo.
    void chargeBattery(int value){
        battery += value;
        if (battery > batteryMax)
            battery = batteryMax;
    }

    //Tenta gastar uma unidade da bateria e emite um erro se não conseguir.
    bool useBattery(){
        if (battery <= 0){
            cout << "fail: bateria insuficiente" << endl;
            return false;
        }
        return true;
    }

    //Se conseguir gastar bateria, armazene a soma no atributo display.
    void sum(int a, int b){
        if (useBattery()){
            display = a + b;
            battery -= 1;
        }
        return;
    }  

    //Se conseguir gastar bateria e não for divisão por 0.
    void division(float num, float den){
        if (!useBattery())
            return;
        battery -= 1;
        if (den == 0){
            cout << "fail: divisao por zero" << endl;
            return;
        }
        display = num/den;
        
        return;
    }

    //Retorna o conteúdo do display com duas casas decimais.
    string str() { 
        stringstream ss;
        ss << "display = " << fixed << setprecision(2) << this->display;
        ss << ", battery = " << this->battery;
        return ss.str();
    }
};

ostream& operator<<(ostream& os, Calculator c) {
    return (os << c.str());
}

int main(){
    Calculator c(0);

    while(true) {
        string line{}, cmd{};
        getline(cin, line);
        stringstream ss(line);
        ss >> cmd;
        cout << "$" << line << '\n';

        if (cmd == "end") {
            break;
        } else if(cmd == "init") {
            int batteryMax {};
            ss >> batteryMax;
            c = Calculator(batteryMax);
        } else if(cmd == "show") {
            cout << c << '\n';
        } else if(cmd == "charge") {
            int value {};
            ss >> value;
            c.chargeBattery(value);
        } else if(cmd == "sum") {
            int a{}, b{};
            ss >> a >> b;
            c.sum(a, b);
        } else if(cmd == "div") {
            int a{}, b{};
            ss >> a >> b;
            c.division(a, b);
        } else {
            cout << ("fail: comando invalido") << '\n';
        }
    }
    return 0;
}