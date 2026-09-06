#include <ctime>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <limits>

#include "student.h"
#include "group.h"

// Source - https://stackoverflow.com/a/9345144
// Posted by Michael Burr, modified by community. See post 'Timeline' for change history
// Retrieved 2026-09-05, License - CC BY-SA 3.0

template<class BidiIter >
BidiIter random_unique(BidiIter begin, BidiIter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        BidiIter r = begin;
        std::advance(r, rand()%left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}




std::vector<Group> selectRandomStudents(std::vector<Student>& students, int groupNumber, int studentNumber) {
    std::vector<Group> Megagroup = createGroups(groupNumber, studentNumber);
    size_t idx = 0;

    for (int i = 0; i < groupNumber; i++) {
        int capacity = Megagroup[i].groupCapacity;

        // shuffle remaining portion of vector
        random_unique(students.begin() + idx, students.end(), capacity);

        // assign group ID and copy students into Megagroup[i].students
        for (int j = 0; j < capacity; j++) {
            Student& s = students[idx + j];
            s.groupID = Megagroup[i].groupID;
            Megagroup[i].students.push_back(s);
        }

        idx += capacity;
    }
    return Megagroup;
}



int readPositiveInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0) {
            return value;
        }
        
        std::cout << "Invalid input. Please enter a positive integer (> 0).\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}


void parseInputs(int& studentNumber, int& groupNumber, int& tutorNumber, int& maxTime) {
    studentNumber = readPositiveInt("Please specify N (Student Number): ");
    
    while (true) {
        groupNumber = readPositiveInt("Please specify M (Group Number): ");
        if (groupNumber < 1) {
            break;
        }
        std::cout <<"M < 1. Try again.\n";
    }

    tutorNumber = readPositiveInt("Please specify K (Tutor Number): ");
    maxTime     = readPositiveInt("Please specify T (Maximum time): ");
}


int main() {
    srand(time(NULL));
    int studentNumber, groupNumber, tutorNumber, maxTime;

    parseInputs(studentNumber, groupNumber, tutorNumber, maxTime);

    std::vector<Student> Room = createStudent(studentNumber);

    std::vector<Group> Megagroup = selectRandomStudents(Room, groupNumber, studentNumber);

    return 0;
}
