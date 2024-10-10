#include "mt-collatz.cpp"
#include <iostream>
#include <mutex> 
#include <vector>
#include <chrono>
#include <cstring>
#include <thread>
#include <iomanip>
using namespace std;

int main(int argc, char* argv[]){
    bool locked;

    if(argc >= 4 && strcmp(argv[3], "-nolock") == 0){
        for(int j = 0; j < argc; j++){
            cout << argv[j] << endl;
        }
    locked = false;
    }

    cout << run_mt_collatz(stoi(argv[1]), stoi(argv[2]), locked) << endl;

    return 0;
}