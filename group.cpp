#include "group.h"

int groupCapacityFor(int groupIndex, int studentNumber, int groupNumber) {
    int base = studentNumber / groupNumber;
    int remainder = studentNumber % groupNumber;
    return base + (groupIndex < remainder ? 1 : 0);
}

void initializeGroup(Group& group, int groupID, int capacity) {
    group.groupID = groupID;
    group.groupCapacity = capacity;
    group.studentIDs.reserve(capacity);
}

std::vector<Group> createGroups(int groupNumber, int studentNumber) {
    std::vector<Group> megaGroup(groupNumber);
    for (int i = 0; i < groupNumber; i++) {
        initializeGroup(megaGroup[i], i, groupCapacityFor(i, studentNumber, groupNumber));
    }
    return megaGroup;
}
