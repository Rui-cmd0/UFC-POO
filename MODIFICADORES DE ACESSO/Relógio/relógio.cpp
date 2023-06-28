
#include <iostream>
#include <sstream>
#include <aux.hpp>
using namespace std;

class Time {
private:
    int hour {0};
    int minute {0};
    int second {0};

public:
    Time (int hour, int minute, int second) {
        this->hour = hour;
        this->minute = minute;
        this->second = second;
    }

    void setHour (int hour) {
        if (hour > 23 || hour < 0){
            cout << "fail: hora invalida" << endl;
            return;
        }
        this->hour = hour;
        return;
    }
    
    void setMinute (int minute) {
        if (minute > 59 || minute < 0){
            cout << "fail: minuto invalido" << endl;
            return;
        }
        this->minute = minute;
        return;
    }

    void setSecond (int second) {
        if (second > 59 || second < 0){
            cout << "fail: segundo invalido" << endl;
            return;
        }
        this->second = second;
    }

    int getHour () {
        return hour; 
    }
    
    int getMinute () {
        return minute; 
    }
    
    int getSecond () {
        return second; 
    }
    
    void nextSecond() {
        second += 1;
        if (second > 59){
            setSecond(0);
            minute += 1;
        }
        if (minute > 59){
            setMinute(0);
            hour += 1;
        }
        if (hour > 23)
            setHour(0);
        return;
    }

    string str() {
        stringstream ss;
        auto sw = setw(2);
        ss  << setfill('0') << sw << this->hour << ":" << sw << this->minute << ":" << sw << this->second;
        return ss.str();
    }
};

int main() {
    aux::Chain chain;
    aux::Param ui;
    Time time(0, 0, 0);

    chain["set"] = [&] () {
        time.setHour(aux::to<int>(ui[1]));
        time.setMinute(aux::to<int>(ui[2]));
        time.setSecond(aux::to<int>(ui[3]));
    };
    chain["show"] = [&] () { cout << time.str() << endl; };
    chain["next"] = [&] () { time.nextSecond(); };

    aux::execute(chain, ui);
};
