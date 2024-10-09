CC=gcc
CFLAGS=-I.

main: mt-collatz.cpp
	$(CXX) $(CXXFLAGS) mt-collatz.cpp -o main

run: $(TARGET)
	./$(TARGET) $(ARGS)
