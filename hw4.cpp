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
    bool locked;
public:
    Chopstick(){
        this->locked = false;
    }
    Chopstick(int id){
        this->id=id;
    }
    void lockChopstick(){
        chopTex.lock();
        this->locked = true;
    };
    void unlockChopstick(){
        chopTex.unlock();
        this->locked = false;
    };
    bool isLocked(){
        return locked;
    }
};

//Syncro tracks all the states of the philosophers
class Syncro{
private:
    bool dining;
    Chopstick chopsticks[N];
    mutex boolDining;
public:
    Syncro(){
        this->dining = false;
    }
    //bool to start dine session or end it
    void setDining(bool food){ 
        boolDining.lock();
        this->dining = food;
        boolDining.unlock();
    }
    bool getDining(){
        return this->dining;
    }
    bool pickupChopstick(int id){
        //validate conditions and Pick up chopstick with id i
        if (chopsticks[id].isLocked() || chopsticks[(id+1) % N].isLocked()){
            return false;
        }
        chopsticks[id].lockChopstick();
        chopsticks[(id+1) % N].lockChopstick();
        return true;
    }
    void putdownChopstick(int id){
        //validate conditions and Put down chopstick with id i
        chopsticks[id].unlockChopstick();
        chopsticks[(id+1) % N].unlockChopstick();
    }
};

class Philosopher: thread{
private:
    int state;
    int id;
    Chopstick *left, *right;
    Syncro &syncro;
public:
    thread mainThread;
    string name;
    double thinkTime;
    double starveTime;
    double eatTime;
    Philosopher(int state, int id, string name, Syncro &t, Chopstick &left, Chopstick &right): mainThread(&Philosopher::run, this),syncro(t){
        this->state = state;
        this->id = id;
        this->name = name;
        this->left = &left;
        this->right = &right;
        this->thinkTime = 0.0;
        this->starveTime = 0.0;
        this->eatTime = 0.0;
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
            //cout << "think time: " << this->thinkTime << "ms\n";
            if (this->state == EATING){
                eating();
            }
        }
    }

    void thinking(){
        bool isStarving = false;

        this->state = THINKING;
        cout << "Philosopher: " << this->name << " thinking...\n";
        auto start = high_resolution_clock::now();
        usleep(1000000); //1s think time
        if(tossCoin()){ //toss coin and try to pick up chopsticks
            this->state = HUNGRY;
            if(syncro.pickupChopstick(this->id)){ //if successful, pick up chopsticks
                this->state = EATING; //send signal to eating 
                cout << this->name << " picked up chopsticks\n";
            }else{
                auto startStarve = high_resolution_clock::now();
                while(!syncro.pickupChopstick(this->id)){ //keep trying to pick up chopstick until we do

                }
                auto stopStarve = high_resolution_clock::now();
                auto durationStarve = duration_cast<milliseconds>(stopStarve-startStarve);
                this->starveTime += durationStarve.count();
                this->state = EATING;
                cout << this->name << " picked up chopsticks\n";
            }
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop-start);

        this->thinkTime += duration.count();
    }
    void eating(){
        cout << "Philosopher: " << this->name << " eating...\n";
        auto start = high_resolution_clock::now(); //start eating time
        usleep(500000); //.5s eat time
        if(tossCoin()){ //toss coin and stop eating
            syncro.putdownChopstick(this->id);
            this->state = THINKING; //done eating, nowthinking again
            cout << this->name << " done eating...\n";
        }
        auto stop = high_resolution_clock::now(); //stop eating time
        auto duration = duration_cast<milliseconds>(stop-start);

        this->eatTime += duration.count();
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
};

const string nameArray[] = {"Yoda", "Obi-Wan", "Rey", "Kanan", "Leia", "Luke", "Ahsoka", 
                          "Mace Windu", "Ezra", "Palpatine", "Anakin", "Kylo Ren", "Dooku",
                          "Kit Fitso", "Luminara", "Plo Koon", "Revan", "Thrawn", "Zeb", "Sabine"};

int main(){
    srand(time(NULL)); //time as seed for rand();
    Philosopher* philosophers[N];
    Chopstick* chopsticks[N];
    Syncro syncro;


    for(int i=0; i<N; i++){
        chopsticks[i] = new Chopstick(i);
        philosophers[i] = new Philosopher(THINKING,i,nameArray[i], syncro, *chopsticks[i], *chopsticks[(i+1)%N]);
    }

    syncro.setDining(true);
    usleep(60000000); //1min
    syncro.setDining(false);

    cout << endl;
    cout << "dine COMPLETE" << endl;
    cout << endl;

    cout << "# of Philosophers: " << N << endl;
    cout << endl;

    cout << "name\t\tthinking\tstarving\teating" << endl;
    cout << endl;

    for(int i=0; i<N; i++){
        cout << philosophers[i]->name << "\t\t" << philosophers[i]->thinkTime << "ms\t\t" << philosophers[i]->starveTime << "ms\t\t" << philosophers[i]->eatTime << "ms" << endl;
    }

    // Join the threads after printing the time outputs
    for(int i=0; i<N; i++){
        philosophers[i]->mainThread.join();
    }

    return 0;
}
