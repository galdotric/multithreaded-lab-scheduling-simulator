#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>

// Upper bound on the number of students the simulator will accept.
constexpr int kMaxStudents = 180;

struct ValidationResult {
    bool valid;
    std::string errorMessage;  // empty when valid
};

// Validates the four simulation parameters read from the user:
//   studentNumber (N), groupNumber (M), tutorNumber (K), maxTime (T)
//
// Rules:
//   - none of the four values may be negative or zero (all must be >= 1)
//   - studentNumber may not exceed kMaxStudents (180)
//
// Returns the first violation found (valid=false, errorMessage set), or
// {true, ""} once every value is acceptable.
ValidationResult validateInput(int studentNumber, int groupNumber, int tutorNumber, int maxTime);

#endif  // VALIDATION_H
