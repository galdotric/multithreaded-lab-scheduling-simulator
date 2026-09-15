#include "labs.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

void tutorRoutine(int labID, ClassroomContext& ctx, int maxTime) {
    while (true) {
        std::unique_lock<std::mutex> lock(ctx.door);

        // announce the room is free and register it (covers both the very
        // first time and every time after a group has left)
        std::cout << "Tutor [" << labID << "]: The lab room [" << labID
                  << "] is vacated and ready for one group." << std::endl;
        ctx.availableLabs.push(labID);
        ctx.cvLabAvailable.notify_one();

        // wait for the teacher to send a group here, or for the signal that
        // no more groups will ever be called
        ctx.cvGroupCalled.wait(lock, [&] {
            return ctx.labAssignedGroup[labID] != -1 || ctx.allGroupsCalled;
        });

        if (ctx.labAssignedGroup[labID] == -1) {
            // woken only because there is nothing left to do
            std::cout << "Teacher: There are no students waiting. Tutor [" << labID
                      << "], you can go home now." << std::endl;
            std::cout << "Tutor [" << labID << "]: Thanks Teacher. Bye!" << std::endl;
            ctx.tutorsGoneHome++;
            if (ctx.tutorsGoneHome == ctx.totalLabs) {
                ctx.cvAllTutorsGone.notify_one();
            }
            return;
        }

        const int g = ctx.labAssignedGroup[labID];

        // wait for every student in the group to physically enter
        ctx.cvRoomFull.wait(lock, [&] { return ctx.enteredCount[labID] == ctx.groupSize[g]; });
        std::cout << "Tutor [" << labID << "]: All students in group [" << g
                  << "] have entered the room [" << labID << "]. You can start your exercise now." << std::endl;

        const int half = maxTime / 2;
        const int t = half + (rand() % (maxTime - half + 1));

        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(t));
        lock.lock();

        std::cout << "Tutor [" << labID << "]: Students in group[" << g
                  << "] have completed the lab exercise in [" << t
                  << "] units of time. You may leave this room now." << std::endl;
        ctx.exerciseDone[labID] = true;
        ctx.cvExerciseDone.notify_all();

        // wait for every student in the group to leave before reusing the room
        ctx.cvRoomVacated.wait(lock, [&] { return ctx.leftCount[labID] == ctx.groupSize[g]; });
        ctx.labAssignedGroup[labID] = -1;
        // loop back to top: re-announce vacancy and wait for the next group
    }
}
