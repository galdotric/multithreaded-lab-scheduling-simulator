#include "threads.h"

#include <iostream>

#include "labs.h"

std::vector<Group> assignStudents(std::vector<Student>& students, int groupNumber, int studentNumber,
                                   ClassroomContext& ctx) {
    std::vector<Group> megaGroup = createGroups(groupNumber, studentNumber);
    size_t idx = 0;

    for (int i = 0; i < groupNumber; i++) {
        int capacity = megaGroup[i].groupCapacity;

        // shuffle the remaining, not-yet-assigned portion of the vector
        random_unique(students.begin() + idx, students.end(), capacity);

        for (int j = 0; j < capacity; j++) {
            Student& student = students[idx + j];
            student.groupID = megaGroup[i].groupID;
            megaGroup[i].studentIDs.push_back(student.studentID);
            std::cout << "Teacher: student [" << student.studentID << "] is in group [" << student.groupID
                      << "]." << std::endl;
        }

        ctx.groupSize[i] = capacity;
        idx += capacity;
    }
    return megaGroup;
}

void teacherRoutine(ClassroomContext& ctx, int groupNumber, int studentNumber, int labNumber, int maxTime) {
    std::cout << "Teacher: I'm waiting for all students to arrive." << std::endl;

    ctx.totalGroups = groupNumber;
    ctx.totalLabs = labNumber;
    ctx.groupSize.assign(groupNumber, 0);
    ctx.groupAssignedLab.assign(groupNumber, -1);
    ctx.labAssignedGroup.assign(labNumber, -1);
    ctx.enteredCount.assign(labNumber, 0);
    ctx.exerciseDone.assign(labNumber, false);
    ctx.leftCount.assign(labNumber, 0);

    std::vector<Student> students = createStudent(studentNumber, ctx);

    std::unique_lock<std::mutex> lock(ctx.door);
    ctx.cvStudentsArrived.wait(lock, [&] { return ctx.arrivedCount == studentNumber; });

    std::cout << "Teacher: All students have arrived. I start to assign group ids to students." << std::endl;
    std::vector<Group> megaGroup = assignStudents(students, groupNumber, studentNumber, ctx);
    ctx.cvStudentAssigned.notify_all();


    ctx.cvAllAssigned.wait(lock, [&] { return ctx.assignedAckCount == studentNumber; });

    std::cout << "Teacher: I'm waiting for lab rooms to become available." << std::endl;
    lock.unlock();

    // only now, with every student's acknowledgment printed, is it safe to
    // bring the tutors online
    std::vector<std::thread> tutors;
    tutors.reserve(labNumber);
    for (int labID = 0; labID < labNumber; labID++) {
        tutors.emplace_back(tutorRoutine, labID, std::ref(ctx), maxTime);
    }

    lock.lock();
    for (int g = 0; g < groupNumber; g++) {
        ctx.cvLabAvailable.wait(lock, [&] { return !ctx.availableLabs.empty(); });

        int labID = ctx.availableLabs.front();
        ctx.availableLabs.pop();

        ctx.groupAssignedLab[g] = labID;
        ctx.labAssignedGroup[labID] = g;
        ctx.enteredCount[labID] = 0;
        ctx.exerciseDone[labID] = false;
        ctx.leftCount[labID] = 0;

        std::cout << "Teacher: The lab [" << labID << "] is now available. Students in group [" << g
                  << "] can enter the room and start your lab exercise." << std::endl;
        ctx.cvGroupCalled.notify_all();
    }

    // every group has now been called; wake any still-idle tutors so they
    // can go home instead of waiting forever
    ctx.allGroupsCalled = true;
    ctx.cvGroupCalled.notify_all();

    ctx.cvAllTutorsGone.wait(lock, [&] { return ctx.tutorsGoneHome == labNumber; });
    lock.unlock();

    for (auto& student : students) {
        student.thread.join();
    }
    for (auto& tutor : tutors) {
        tutor.join();
    }

    std::cout << "Teacher: All students and tutors are left. I can now go home." << std::endl;
}

std::thread makeTeacher(ClassroomContext& ctx, int groupNumber, int studentNumber, int labNumber, int maxTime) {
    return std::thread(teacherRoutine, std::ref(ctx), groupNumber, studentNumber, labNumber, maxTime);
}