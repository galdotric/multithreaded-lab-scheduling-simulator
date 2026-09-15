#ifndef STUDENT_H
#define STUDENT_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

struct ClassroomContext {
    std::mutex door;

    std::condition_variable cvStudentsArrived;
    int totalStudents = 0;
    int arrivedCount = 0;

    std::condition_variable cvStudentAssigned;
    std::condition_variable cvAllAssigned;  // every student has printed its acknowledgment
    int assignedAckCount = 0;

    std::condition_variable cvGroupCalled;
    std::condition_variable cvRoomFull;
    std::condition_variable cvExerciseDone;
    std::condition_variable cvRoomVacated;
    std::condition_variable cvLabAvailable;
    std::condition_variable cvAllTutorsGone; 
    int totalGroups = 0;
    int totalLabs = 0;
    bool allGroupsCalled = false;
    int tutorsGoneHome = 0;

    std::queue<int> availableLabs;      
    std::vector<int> groupSize;
    std::vector<int> groupAssignedLab;
    std::vector<int> labAssignedGroup;
    std::vector<int> enteredCount;
    std::vector<bool> exerciseDone;
    std::vector<int> leftCount;
};

struct Student {
    int studentID;
    int groupID;
    std::thread thread;
};

void initializeStudent(Student& student, int studentID);

// Runs the whole lifetime of one student: arrival, waiting for a group id,
// waiting for that group's turn in a lab, entering, and leaving.
void StudentRoutine(Student& student, ClassroomContext& ctx);

// Creates and spawns all N student threads (each thread is started here,
// by this function — not by main) and returns them so the caller can join.
std::vector<Student> createStudent(int studentNumber, ClassroomContext& ctx);

#endif // STUDENT_H