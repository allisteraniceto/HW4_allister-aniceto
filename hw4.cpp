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

// #include <random>

// states
#define THINKING 1
#define HUNGRY 2
#define EATING 3

using namespace std;
using namespace std::chrono;

const int UNLOCKED = 0;
const int LOCKED = 1;
const int N = 5;

class Chopstick{
private:
    mutex chopTex;
    int id;
    int status;
public:
    Chopstick(){
        
    }
    Chopstick(int id){
        this->id=id;
    }
    void lockChopstick(){
        if (rand() % 2 == 0){ //if heads

        }
        chopTex.lock();
    };
    void unlockChopstick(){
        chopTex.unlock();
    };
};

//Syncro tracks all the states of the philosophers
class Syncro{
private:
    bool dining;
    Chopstick chopsticks[N];
public:
    Syncro(){
        this->dining = false;
    }
    //bool to start dine session or end it
    void setDining(bool food){ 
        this->dining = food;
    }
    bool getDining(){
        return this->dining;
    }
    void pickupChopstick(int id){
        //validate conditions and Pick up chopstick with id i
        chopsticks[id].lockChopstick();
        chopsticks[(id+1) % N].lockChopstick();
    }
    void putdownChopstick(int id){
        //validate conditions and Put down chopstick with id i
        chopsticks[id].unlockChopstick();
        chopsticks[(id+1) % N].unlockChopstick();
    }
    // void updateStatus(int i, int status){
    //     philosopherStates[i]=status;
    // }
    // int getState(int i){
    //     return 0;
    // }
};

class Philosopher: thread{
private:
    int state;
    int id;
    double thinkTime;
    double eatTime;
    string name;
    Chopstick *left, *right;
    thread mainThread;
    Syncro &syncro;
public:
    Philosopher(int state, int id, string name, Syncro &t, Chopstick &left, Chopstick &right): mainThread(&Philosopher::run, this),syncro(t){
        this->state = state;
        this->id = id;
        this->name = name;
        this->left = &left;
        this->right = &right;
    }
    ~Philosopher(){ //destructor
        mainThread.join();
    }
    void run(){
        do{
            cout << "Waiting to dine\n";
        }while(syncro.getDining() == false);

        while(syncro.getDining() == true){
            //[TOSS COIN HERE and TAKE CHOPSTICKS]
            thinking();
            //[TOSS COIN and RELEASE CHOPSTICKS]
            cout << "think time: " << this->thinkTime << "ms\n";
            eating();
        }
    }

    void thinking(){
        this->state = THINKING;
        cout << "Philosopher: " << this->name << " thinking...\n";
        auto start = high_resolution_clock::now();
        usleep(50000);
        if(tossCoin()){
            syncro.pickupChopstick(this->id);
            cout << this->name << " picked up chopsticks\n";
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop-start);

        this->thinkTime += duration.count();
    }
    void eating(){
        this->state = EATING;
        cout << "Philosopher: " << this->name << " eating...\n";
        usleep(50000);
        if(tossCoin()){
            syncro.putdownChopstick(this->id);
            cout << this->name << " done eating...\n";
        }else
            cout << this->name << " still eating...\n";
        }
    // //lock both chopsticks
    // void take_chopsticks(){
    //     //if heads, get left first, otherwise right
    //     if (tossCoin()){
    //         left.lockChopstick();
    //         right.lockChopstick();
    //     }else{
    //         right.lockChopstick();
    //         left.lockChopstick();
    //     }
    // }
    // //unlock both chopsticks
    // void release_chopsticks(){
    //     if(tossCoin()){
    //         left.unlockChopstick();
    //         right.unlockChopstick();
    //     }
    //     right.unlockChopstick();
    //     left.unlockChopstick();
    // }
    int tossCoin(){
        return rand()%2;
    }
    // void test(int i){
    //     // check for status of the neighbors philosopher and change status
    //     int leftNeighbor = (i + N - 1) % N;
    //     int rightNeighbor = (i + 1) % N;

    //     // if the left neighbor is eating or the right neighbor is eating, or both, HUNGRY
    //     if (syncro.getState(leftNeighbor) == EATING || syncro.getState(rightNeighbor) == EATING) {
    //         this->state = HUNGRY;
    //     } else {
    //         // if both neighbors are not eating, and the current philosopher is hungry, try to acquire chopsticks
    //         if (state == HUNGRY) {
    //             take_chopsticks();
    //             syncro.putdownChopstick(i); // Release the chopstick state after successfully acquiring chopsticks
    //         }
    //     }
    // }
};

const string nameArray[] = {"Yoda", "Obi-Wan", "Rey", "Kanan", "Leia", "Luke", "Ahsoka", 
                          "Mace Windu", "Ezra", "Palpatine", "Anakin", "Kylo Ren", "Dooku",
                          "Kit Fitso", "Luminara", "Plo Koon", "Revan", "Thrawn", "Zeb", "Sabine"};

int main(){
    srand(time(NULL)); //time as seed for rand();
    Philosopher* philosopher[N];
    Chopstick* chopsticks[N];
    Syncro syncro;


    for(int i=0; i<N; i++){
        chopsticks[i] = new Chopstick(i);
        philosopher[i] = new Philosopher(THINKING,i,nameArray[i], syncro, *chopsticks[i], *chopsticks[(i+1)%N]);
    }

    syncro.setDining(true);
    usleep(1000000);
    syncro.setDining(false);

    cout << endl;
    cout << "dine COMPLETE" << endl;
    return 0;
}