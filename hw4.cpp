/* HW4 DINING PHILOSPHERS PROBLEM
5 philosophers
state: THINKING, HUNGRY, EATING
a single chopstick in between all of philosophers
NEED 2 chopsticks to eat (left and right)
*/

#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <chrono> 
#include <unistd.h> 
#include <ctime>
#include <time.h>

#include <random> //for tossing a coin

// states
#define THINKING 1
#define HUNGRY 2
#define EATING 3

using namespace std;

const int UNLOCKED = 0;
const int LOCKED = 1;
const int NUMBER_OF_PHILOSOPHERS = 5;

/*
Q1. Philosopher class
Inherits from thread or runnable class

Members:
- int state;
- int id;
- String name;
- Chopstick left, right;

Methods:
- constructor()
    - initalize name, id, and the 2 Chopsticks
- run()
    - contain a repitition process waiting for exact moment to rpint the status of
      the philosopher THINKING, EATING, or HUNGRY
- take_chopsticks()
    - get the two Chopsticks
- release_chopsticks()
    - relates the two Chopsticks
- test(int i)
    - check for status of the neighbors philospher and change status
      this can be used to know when to request the Chopsticks
*/

/*
Q2. Syncro class

METHODS
This class has at least two method Putdownchopstick and Pickupchopstick.
Putdownchopstick(int i):validate conditions and Put down chopstick with id i
Pickupchopstick(int i): validate conditions and Pick up chopstick with id i
*/


// class Syncro{
// private:
//     static Semaphore chopstick[N];
//     static int state[NUMBER_OF_PHILOSPHERS];
// public:
//     Syncro(){

//     }
//     void Putdownchopstick(int i){
//         //validate conditions and Put down chopstick with id i
//     }
//     void Pickupchopstick(int i){
//         //validate conditions and Pick up chopstick with id i
//     }
// }

class Chopstick{
private:
    mutex chopTex;
    int status;
public:
    Chopstick(){

    }
    void lockChopstick(){
        chopTex.lock();
    };
    void unlockChopstick(){
        chopTex.unlock();
    };
};


class Philosopher: thread{
private:
    int state;
    int id;
    double thinkTime;
    string name;
    Chopstick left, right;
    thread mainThread;
public:
    Philosopher(int state, int id, string name): mainThread(&Philosopher::run, this){
        this->state = state;
        this->id = id;
        this->name = name;
    }
    ~Philosopher(){ //destructor
        mainThread.join();
    }
    void run(){
        while(true){
            this->state = THINKING;
            cout << "Philosopher: " << this->id << " thinking...\n";
            //[TOSS COIN HERE and TAKE CHOPSTICKS]
            usleep(10000000);
            take_chopsticks();
            this->state = EATING;
            cout << "Philosopher: " << this->id << " eating...\n";
            //[TOSS COIN and RELEASE CHOPSTICKS]
            release_chopsticks();
            // usleep(10000000);
        }
    }
    //lock both chopsticks
    void take_chopsticks(){
        left.lockChopstick();
        right.lockChopstick();
    }
    //unlock both chopsticks
    void release_chopsticks(){
        left.unlockChopstick();
        right.unlockChopstick();
    }
    // void test(int i){
    //     // check for status of the neighbors philosopher and change status
    //     int leftNeighbor = (i + NUMBER_OF_PHILOSOPHERS - 1) % NUMBER_OF_PHILOSOPHERS;
    //     int rightNeighbor = (i + 1) % NUMBER_OF_PHILOSOPHERS;

    //     // if the left neighbor is eating or the right neighbor is eating, or both, HUNGRY
    //     if (Syncro::getState(leftNeighbor) == EATING || Syncro::getState(rightNeighbor) == EATING) {
    //         this.state = HUNGRY;
    //     } else {
    //         // if both neighbors are not eating, and the current philosopher is hungry, try to acquire chopsticks
    //         if (state == HUNGRY) {
    //             take_chopsticks();
    //             Syncro::putDownChopstick(i); // Release the chopstick state after successfully acquiring chopsticks
    //         }
    //     }
    // }
};

const string nameArray[] = {"Yoda", "Obi-Wan", "Rey", "Kanan", "Leia", "Luke", "Ahsoka", 
                          "Mace Windu", "Ezra", "Palpatine", "Anakin", "Kylo Ren", "Dooku",
                          "Kit Fitso", "Luminara", "Plo Koon", "Revan", "Thrawn", "Zeb", "Sabine"};

void dine(){
    Philosopher* philosopher[NUMBER_OF_PHILOSOPHERS];

    for(int i=0; i<NUMBER_OF_PHILOSOPHERS; i++){
        philosopher[i] = new Philosopher(THINKING,i,nameArray[i]);
    }
}

int main(){
    dine();
    return 0;
}