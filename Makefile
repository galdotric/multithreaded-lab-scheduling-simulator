CXX = g++
CXXFLAGS = -Wall -Wextra

main: main.cpp student.cpp group.cpp
	$(CXX) $(CXXFLAGS) main.cpp student.cpp group.cpp -o main

run: main
	./main

clean:
	rm -f main
