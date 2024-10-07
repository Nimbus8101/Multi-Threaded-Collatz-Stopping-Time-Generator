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
vector<int> histogram(10000, 0);

void createThread(int threadnum){
int variable;
while(counter <= N){

if(locked == true){
    safe.lock();
variable = counter + 1;
counter++;
safe.unlock();
}
else{
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

histogram.at(subI)++;
}

return;
}

int main(int argc, char* argv[]){
    cout << argc << endl;
if(argc >= 3 && strcmp(argv[3], "-nolock") == 0){
    for(int j = 0; j < argc; j++){
        cout << argv[j] << endl;
    }
locked = false;
}
N = stoi(argv[1]);
T = stoi(argv[2]);

auto start = chrono::high_resolution_clock::now();

vector<thread> TH;
for(int i = 0; i < T; i++){
    TH.emplace_back(createThread, i);
}
for(auto& threads : TH){
    threads.join();
}

auto end = chrono::high_resolution_clock::now();
for(int j = 0; j < 1000; j++){
    if(histogram.at(j) != 0){
        cout << endl << histogram.at(j) << "frequncy_of_stopping_time(" << j << ")";
    }
}
cout << endl;

chrono::duration<double> secs = (end -start);

std::cerr << N << ", " << T << ", " << fixed << setprecision(9) << secs.count()+1 << endl;
return 0;
}