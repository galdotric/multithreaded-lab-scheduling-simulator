#include "validation.h"

ValidationResult validateInput(int studentNumber, int groupNumber, int tutorNumber, int maxTime) {
    if (studentNumber <= 0) {
        return {false, "Student number (N) must be a positive integer."};
    }
    if (studentNumber > kMaxStudents) {
        return {false, "Student number (N) cannot exceed " + std::to_string(kMaxStudents) + "."};
    }
    if (groupNumber <= 0) {
        return {false, "Group number (M) must be a positive integer."};
    }
    if (tutorNumber <= 0) {
        return {false, "Tutor number (K) must be a positive integer."};
    }
    if (maxTime <= 0) {
        return {false, "Maximum time (T) must be a positive integer."};
    }
    return {true, ""};
}
