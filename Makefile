CC=gcc
CFLAGS=-I.

main: mt-collatz.cpp
	$(CXX) $(CXXFLAGS) mt-Collatz.cpp -o main

experiment: experiment.cpp
	$(CXX) $(CXXFLAGS) experiment.cpp -o experiment

run: $(TARGET)
	./$(TARGET) $(ARGS)
