#ifndef THREADS_H
#define THREADS_H

#include <algorithm>
#include <thread>
#include <vector>

#include "group.h"
#include "student.h"

// Source - https://stackoverflow.com/a/9345144
// Posted by Michael Burr, modified by community. See post 'Timeline' for change history
// Retrieved 2026-09-05, License - CC BY-SA 3.0

template <class BidiIter>
BidiIter random_unique(BidiIter begin, BidiIter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        BidiIter r = begin;
        std::advance(r, rand() % left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}

// Randomly assigns each student a group id and returns the resulting
// groups. Student objects themselves are never moved or copied they stay
// owned by `students` for the whole program so their thread can be joined
// later; each Group only remembers its members' ids.
std::vector<Group> assignStudents(std::vector<Student>& students, int groupNumber, int studentNumber,
                                   ClassroomContext& ctx);

// Full lifetime of the teacher thread: wait for arrivals, assign groups,
// spawn tutors, call groups into labs in order, then wait for everyone
// (students and tutors) to finish before returning.
void teacherRoutine(ClassroomContext& ctx, int groupNumber, int studentNumber, int labNumber, int maxTime);

std::thread makeTeacher(ClassroomContext& ctx, int groupNumber, int studentNumber, int labNumber, int maxTime);

#endif // THREADS_H
