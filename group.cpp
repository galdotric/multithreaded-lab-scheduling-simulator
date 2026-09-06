#include <pthread.h>
#include <vector>
#include "student.h"
using namespace std;


int numberOfExtraStudents;
bool extraStudents = false;

struct Group {
    int groupCapacity; // C
    int groupID;
    vector<Student> students;
};

int setCapacity(int& studentNumber, int& groupNumber){
    int groupCapacity = 0;

    if( studentNumber % groupNumber == 0 ){
        groupCapacity = studentNumber/groupNumber;
    }
    else {
        // for first first (N%M groups)
        extraStudents = true;
        numberOfExtraStudents = studentNumber % groupNumber;
        groupCapacity = (studentNumber/groupNumber) + 1;
    }

    return groupCapacity;
}


void initializeGroup(Group& group, int groupID, int capacity){
    group.groupID = groupID;
    group.groupCapacity = capacity;
    group.students.reserve(capacity);
}



vector<Group> createGroups(int groupNumber, int studentNumber) {
    vector<Group> MegaGroup(groupNumber);

    //set capacity for each group in MegaGroup
    int capacity = setCapacity(studentNumber, groupNumber);

    for (int i = 0; i < groupNumber; i++) {
        int groupCapacity = capacity;
        if (extraStudents && i >= numberOfExtraStudents) {
            groupCapacity--;
        }

        initializeGroup(MegaGroup[i], i, groupCapacity);
    }

    return MegaGroup;
}
