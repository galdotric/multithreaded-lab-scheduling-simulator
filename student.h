#ifndef STUDENT_H
#define STUDENT_H

#include <vector>
#include <pthread.h>

struct Student {
    int studentID;
    pthread_t thread;
    int groupID;
};

void initializeStudent(Student& student, int studentID);

void* StudentRoutine(void* arg);

std::vector<Student> createStudent(int studentNumber);

#endif
