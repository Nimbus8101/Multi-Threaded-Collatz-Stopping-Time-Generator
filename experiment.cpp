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
bool useLock = true;
vector<int> histogram = vector<int>(10000, 0);

ofstream histogramFile("histograms.csv");

void createThread(int threadnum) {
    unsigned int variable;
    while (true) {
        if (useLock) {
            safe.lock();
            if (counter > N) {
                safe.unlock();
                break; 
            }
            variable = counter;
            counter++;
            safe.unlock();
        } else {
            variable = counter;
            if (variable > N) {
                break;
            }
            counter++;
        }

        int subI = 0;
        while (variable != 1) {
            if (variable % 2 == 0) {
                variable /= 2;
            } else {
                variable = (variable * 3) + 1;
            }
            subI++;
            if (subI >= histogram.size()) {
                break;
            }
        }

        if (subI < histogram.size()) {
            if (useLock) {
                safe.lock();
                histogram[subI]++;
                safe.unlock();
            } else {
                histogram[subI]++;
            }
        }
    }
}

string run_mt_collatz(int NVal, int TVal, bool lockedVal){
    chrono::duration<double> secs;
    useLock = lockedVal;
    N = NVal;
    T = TVal;
    
    if(lockedVal){
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
        secs = (end -start);
    }else{
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
        secs = (end -start);
    }
    

    //Transfers the histogram info to the histogram file
    for(int j = 0; j < 1000; j++){
        if(histogram.at(j) != 0){
            histogramFile << histogram.at(j) << "," << j << "\n";
            cout << endl << histogram.at(j) << ", frequency_of_stopping_time(" << j << ")";
        }
    }
    histogramFile << "\n\n";

    //cout << "Threads counted / threads wanted: " << totalThreadNumber << " / " << N << endl;
    std::cerr << N << ", " << T << ", " << fixed << setprecision(9) << secs.count()+1 << endl;

    //Returns the information as a string, to be used in a csv file
    return to_string(N) + ", " + to_string(T) + ", " + to_string(secs.count()+1);
};


int main(){
    int N_VALUE = 6678923;
    int MAX_T_VALUE = 8;
    //Run the collatz experiment with T values 1-8 (N = 10000)

    ofstream myFile("data.csv");

    for(int i = 1; i <= MAX_T_VALUE; i++){
        cout << "In loop" << endl;
        for(int j = 0; j < 10; j++){
            myFile << run_mt_collatz(N_VALUE, i, true) << "\n";
        }
    }

    myFile << "\n\n\n";

    for(int i = 1; i <= MAX_T_VALUE; i++){
        cout << "In loop" << endl;
        for(int j = 0; j < 10; j++){
            myFile << run_mt_collatz(N_VALUE, i, false) << "\n";
        }
    }

    myFile.close();
    
    return 0;
}