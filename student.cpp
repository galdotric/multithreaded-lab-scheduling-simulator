#include "student.h"

#include <iostream>

void initializeStudent(Student& student, int studentID) {
    student.studentID = studentID;
    student.groupID = -1;
}

void StudentRoutine(Student& student, ClassroomContext& ctx) {
    std::cout << "Student[" << student.studentID
              << "]: I have arrived and wait for being assigned to a group." << std::endl;

    std::unique_lock<std::mutex> lock(ctx.door);

    // check in with the teacher
    ctx.arrivedCount++;
    if (ctx.arrivedCount == ctx.totalStudents) {
        ctx.cvStudentsArrived.notify_one();
    }

    // wait to be assigned a group id
    ctx.cvStudentAssigned.wait(lock, [&] { return student.groupID != -1; });
    std::cout << "Student [" << student.studentID << "]: OK, I'm in group [" << student.groupID
              << "] and waiting for my turn to enter a lab room." << std::endl;
    ctx.assignedAckCount++;
    if (ctx.assignedAckCount == ctx.totalStudents) {
        ctx.cvAllAssigned.notify_one();
    }

    // wait until this group has been called into a lab
    const int g = student.groupID;
    ctx.cvGroupCalled.wait(lock, [&] { return ctx.groupAssignedLab[g] != -1; });
    const int labID = ctx.groupAssignedLab[g];

    std::cout << "Student [" << student.studentID << "] in group [" << g
              << "]: My group is called. I will enter the lab room [" << labID << "] now." << std::endl;
    ctx.enteredCount[labID]++;
    if (ctx.enteredCount[labID] == ctx.groupSize[g]) {
        ctx.cvRoomFull.notify_one();
    }

    // wait for the tutor to finish running the exercise
    ctx.cvExerciseDone.wait(lock, [&] { return ctx.exerciseDone[labID]; });
    std::cout << "Student [" << student.studentID << "] in group [" << g
              << "]: Thanks Tutor [" << labID << "]. Bye!" << std::endl;
    ctx.leftCount[labID]++;
    if (ctx.leftCount[labID] == ctx.groupSize[g]) {
        ctx.cvRoomVacated.notify_one();
    }
}

std::vector<Student> createStudent(int studentNumber, ClassroomContext& ctx) {
    ctx.totalStudents = studentNumber;
    std::vector<Student> classroom(studentNumber);

    for (int id = 0; id < studentNumber; id++) {
        initializeStudent(classroom[id], id);
    }
    for (int id = 0; id < studentNumber; id++) {
        classroom[id].thread = std::thread(StudentRoutine, std::ref(classroom[id]), std::ref(ctx));
    }
    return classroom;
}