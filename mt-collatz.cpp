#include <iostream>
#include <mutex> 
#include <vector>
#include <chrono>
#include <cstring>
#include <thread>
#include <iomanip>
using namespace std;

int N = 0;
int T = 0;
int counter = 0;
mutex safe;
bool locked = true;
vector<int> histogram = vector<int>(10000, 0);

void createThread(int threadnum){
    int variable;
    while(counter < N){
        if(locked == true){
            safe.lock();
                variable = counter + 1;
                cout << ".";
                counter++;
            safe.unlock();
            if(variable > N){ return; } //Prevents excess collatz sequences from being calculated
        }else{
            variable = counter + 1;
            counter++;
        }

        int subI = 0;
        while(true){
            if(variable == 1){
                break;
            }

            if(variable % 2 == 0){
                variable = variable / 2;
                subI++;
            }
            else if(variable % 2 != 0){
                variable = ((variable * 3) + 1);
                subI++;
            }
        }

        if(locked == true){
            safe.lock();
                histogram.at(subI)++;
            safe.unlock();
        }else{
            histogram.at(subI)++;
        }
    }
    return;
};

string run_mt_collatz(int NVal, int TVal, bool lockedVal){
    locked = lockedVal;
    N = NVal;
    T = TVal;
    
    //Timer setup
    auto start = chrono::high_resolution_clock::now();

    //Thread setup, each one calculating a collatz sequence
    vector<thread> TH;
    for(int i = 0; i < T; i++){
        TH.emplace_back(createThread, i);
    }
    for(auto& threads : TH){
        threads.join();
    }
    
    //timer end
    auto end = chrono::high_resolution_clock::now();

    //Reads the histogram
    int totalThreadNumber = 0;
    for(int j = 0; j < 1000; j++){
        totalThreadNumber += histogram.at(j);
        if(histogram.at(j) != 0){
            cout << endl << histogram.at(j) << ", frequency_of_stopping_time(" << j << ")";
        }
    }
    cout << endl;
    chrono::duration<double> secs = (end -start);

    //cout << "Threads counted / threads wanted: " << totalThreadNumber << " / " << N << endl;
    std::cerr << N << ", " << T << ", " << fixed << setprecision(9) << secs.count()+1 << endl;

    //Returns the information as a string, to be used in a csv file
    return to_string(N) + ", " + to_string(T) + ", " + to_string(secs.count()+1);
};

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