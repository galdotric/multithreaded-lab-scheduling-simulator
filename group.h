#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include "student.h"
struct Group {
    int groupCapacity;
    int groupID;
    std::vector<Student> students;
};

int setCapacity(int& studentNumber, int& groupNumber);

void initializeGroup(Group& group, int groupID, int capacity);

std::vector<Group> createGroups(int groupNumber, int studentNumber);

#endif
