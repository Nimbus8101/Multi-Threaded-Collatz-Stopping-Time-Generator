#include <iostream>
#include <mutex> 
#include <vector>
#include <chrono>
#include <cstring>
#include <thread>
#include <iomanip>
#include <fstream>
using namespace std;

unsigned int N = 0;
int T = 0;
int counter = 0;
mutex safe;
bool locked = true;
vector<int> histogram = vector<int>(10000, 0);

ofstream histogramFile("histograms.csv");

void createThread(int threadnum){
    unsigned int variable;
    int subI = 0;
    while(counter < N){
        if(locked == true){
            safe.lock();
                if(subI > 0){
                    histogram.at(subI)++;
                }
                variable = counter + 1;
                counter++;
            safe.unlock();
            if(variable > N){ return; } //Prevents excess collatz sequences from being calculated
        }else{
            variable = counter + 1;
            counter++;
        }

        subI = 0;
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

        if(locked != true){
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

    //Transfers the histogram info to the histogram file
    for(int j = 0; j < 1000; j++){
        if(histogram.at(j) != 0){
            cout << endl << histogram.at(j) << ", frequency_of_stopping_time(" << j << ")";
        }
    }
    histogramFile << "\n\n";

    chrono::duration<double> secs = (end -start);

    //cout << "Threads counted / threads wanted: " << totalThreadNumber << " / " << N << endl;
    std::cerr << N << ", " << T << ", " << fixed << setprecision(9) << secs.count()+1 << endl;

    //Returns the information as a string, to be used in a csv file
    return to_string(N) + ", " + to_string(T) + ", " + to_string(secs.count()+1);
};


int main(){
    int N_VALUE = 20000000;
    int MAX_T_VALUE = 8;
    //Run the collatz experiment with T values 1-8 (N = 10000)

    ofstream myFile("data.csv");

    for(int i = 1; i <= MAX_T_VALUE; i++){
        cout << "In loop" << endl;
        for(int j = 0; j < 10; j++){
            myFile << run_mt_collatz(N_VALUE, i, true) << "\n";
        }
    }

    myFile.close();
    
    return 0;
}