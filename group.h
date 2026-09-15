#ifndef GROUP_H
#define GROUP_H

#include <vector>

struct Group {
    int groupID;
    int groupCapacity;
    std::vector<int> studentIDs;
};

// Capacity of group `groupIndex` (0-based) out of `groupNumber` groups
// sharing `studentNumber` students: the first N%M groups get one extra.
int groupCapacityFor(int groupIndex, int studentNumber, int groupNumber);

void initializeGroup(Group& group, int groupID, int capacity);

std::vector<Group> createGroups(int groupNumber, int studentNumber);

#endif // GROUP_H
