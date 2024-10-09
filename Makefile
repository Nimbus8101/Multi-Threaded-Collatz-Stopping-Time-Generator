CC=gcc
CFLAGS=-I.

main: mt-collatz.cpp
	$(CXX) $(CXXFLAGS) mtCollatz.cpp -o main

experiment: experiment.cpp
	$(CXX) $(CXXFLAGS) experiment.cpp -o experiment

run: $(TARGET)
	./$(TARGET) $(ARGS)
