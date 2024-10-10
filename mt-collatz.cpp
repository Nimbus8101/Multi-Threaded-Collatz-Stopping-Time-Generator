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
int counter = 1;
mutex safe; 
vector<int> histogram(1000, 0);
bool useLock = true; 

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

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <N> <T> [-nolock]" << endl;
        return 1;
    }

    if (argc == 4 && strcmp(argv[3], "-nolock") == 0) {
        cout << "Running in no-lock mode" << endl;
        useLock = false; 
    }

    N = stoi(argv[1]);
    T = stoi(argv[2]);

    auto start = chrono::high_resolution_clock::now();

    vector<thread> TH;
    for (int i = 0; i < T; i++) {
        TH.emplace_back(createThread, i);
    }
    for (auto& threads : TH) {
        threads.join();
    }

    int total = 0;
    auto end = chrono::high_resolution_clock::now();
    for (int j = 0; j < histogram.size(); j++) {
        if (histogram[j] != 0) {
            cout << endl << histogram[j] << " frequency_of_stopping_time(" << j << ")";
            total = total + histogram[j];
        }

    }
    cout << endl << "Total: " <<  total;
    cout << endl;

    chrono::duration<double> secs = (end - start);
    cerr << N << ", " << T << ", " << fixed << setprecision(9) << secs.count() << endl;
    return 0;
}