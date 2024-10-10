#include <iostream>
#include <mutex> 
#include <vector>
#include <chrono>
#include <cstring>
#include <thread>
#include <iomanip>
#include <fstream>
using namespace std;

int N = 0;
int T = 0;
int counter = 0;
mutex safe;
bool locked = true;
vector<int> histogram = vector<int>(10000, 0);
ofstream histogramFile("histograms.csv");

void createLockingThread(int threadnum){
    unsigned int variable;
    int subI = 0;
    while(counter < N){
        if(subI > 0){
            safe.lock();
                histogram.at(subI)++;
                variable = counter + 1;
                counter++;
            safe.unlock();
        }else{
            safe.lock();
                variable = counter + 1;
                counter++;
            safe.unlock();
        }
        if(variable > N){ return; } //Prevents excess collatz sequences from being calculated

        subI = 0;
        while(true){
            if(variable == 1){
                break;
            }
            else if(variable % 2 == 0){
                variable = variable / 2;
                subI++;
            }
            else if(variable % 2 != 0){
                variable = ((variable * 3) + 1);
                subI++;
            }
        }
    }
    return;
};

void createNonlockingThread(int threadnum){
    unsigned int variable;
    while(counter < N){
        variable = counter + 1;
        counter++;

        int subI = 0;
        while(true){
            if(variable == 1){
                break;
            }
            else if(variable % 2 == 0){
                variable = variable / 2;
                subI++;
            }
            else if(variable % 2 != 0){
                variable = ((variable * 3) + 1);
                subI++;
            }
        }
        histogram.at(subI)++;
    }
    return;
};

string run_mt_collatz(int NVal, int TVal, bool lockedVal){
    chrono::duration<double> secs;
    locked = lockedVal;
    N = NVal;
    T = TVal;
    
    if(lockedVal){
        //Timer setup
        auto start = chrono::high_resolution_clock::now();

        //Thread setup, each one calculating a collatz sequence
        vector<thread> TH;
        for(int i = 0; i < T; i++){
            TH.emplace_back(createLockingThread, i);
        }
        for(auto& threads : TH){
            threads.join();
        }
        
        //timer end
        auto end = chrono::high_resolution_clock::now();
        secs = (end -start);
    }else{
        //Timer setup
        auto start = chrono::high_resolution_clock::now();

        //Thread setup, each one calculating a collatz sequence
        vector<thread> TH;
        for(int i = 0; i < T; i++){
            TH.emplace_back(createNonlockingThread, i);
        }
        for(auto& threads : TH){
            threads.join();
        }
        
        //timer end
        auto end = chrono::high_resolution_clock::now();
        secs = (end -start);
    }
    

    //Transfers the histogram info to the histogram file
    for(int j = 0; j < 1000; j++){
        if(histogram.at(j) != 0){
            cout << endl << histogram.at(j) << ", frequency_of_stopping_time(" << j << ")";
        }
    }
    histogramFile << "\n\n";

    //cout << "Threads counted / threads wanted: " << totalThreadNumber << " / " << N << endl;
    std::cerr << endl << N << ", " << T << ", " << fixed << setprecision(9) << secs.count()+1 << endl;

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

    run_mt_collatz(stoi(argv[1]), stoi(argv[2]), locked);

    return 0;
}