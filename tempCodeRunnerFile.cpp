#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <chrono> 
#include <unistd.h> 
#include <ctime>
#include <time.h>
using namespace std;
const int UNLOCKED = 0;
const int LOCKED = 1;
const int NUMBER_OF_PHILOSPHERS = 5;

class Chopstick {
private:
    mutex chopTex;
    int status;
public:
    Chopstick() {
    }
    void lockChopstick() {
    }
    void unlockChopstick() {
    }
};

class Syncro {
private:
    bool dining;
    Chopstick chopsticks[NUMBER_OF_PHILOSPHERS];
public:
    Syncro() {
    }
    void putDownChopstick(int id) {
    }
    void pickUpChopstick(int id) {
    }
};

class Philosopher: thread {
private:
    string name;
    double thinkTime;
    int id;
    thread mainThread;
    mutex outputMutex;
    Syncro &syncro;

public:
    Philosopher(string name, Syncro &t, int id) : mainThread(&Philosopher::run, this), syncro(t) {
        this->name = name;
        this->id = id;
        this->thinkTime = 0.0;
    }


    ~Philosopher() {
        mainThread.join();
    }

    void run() {
        cout << " start eating.\n";
        usleep(50000);
        cout << " finished eating.\n";
    }
};

 

const string nameArray[] = {"Yoda", "Obi-Wan", "Rey", "Kanan", "Leia", "Luke", "Ahsoka", 
                          "Mace Windu", "Ezra", "Palpatine", "Anakin", "Kylo Ren", "Dooku",
                          "Kit Fitso", "Luminara", "Plo Koon", "Revan", "Thrawn", "Zeb", "Sabine"};

void dine() {
    Syncro syncro;
    Philosopher* philosophers[NUMBER_OF_PHILOSPHERS];

    for(int i = 0; i < NUMBER_OF_PHILOSPHERS; i++) {
        philosophers[i] = new Philosopher(nameArray[i], syncro, i);
    }
    usleep(100000);
}

 

int main() {
    dine();
    return 0;
}