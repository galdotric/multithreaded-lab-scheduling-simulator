#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>

#include "student.h"
#include "threads.h"
#include "validation.h"

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int studentNumber;  // N
    int groupNumber;    // M
    int tutorNumber;    // K
    int maxTime;        // T

    std::cout << "Please specify N(Student Number): ";
    std::cin >> studentNumber;

    std::cout << "Please specify M(Group Number): ";
    std::cin >> groupNumber;

    std::cout << "Please specify K(Tutor Number): ";
    std::cin >> tutorNumber;

    std::cout << "Please specify T(Maximum time): ";
    std::cin >> maxTime;

    if (std::cin.fail()) {
        std::cerr << "Invalid input: please enter numeric values only." << std::endl;
        return 1;
    }

    const ValidationResult result = validateInput(studentNumber, groupNumber, tutorNumber, maxTime);
    if (!result.valid) {
        std::cerr << "Invalid input: " << result.errorMessage << std::endl;
        return 1;
    }

    ClassroomContext ctx;

    std::thread teacherThread = makeTeacher(ctx, groupNumber, studentNumber, tutorNumber, maxTime);
    teacherThread.join();

    std::cout << "Main thread: All students have completed their lab exercises. This is the end of simulation."
              << std::endl;
    return 0;
}
