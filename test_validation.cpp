// Standalone test suite for validateInput().
//
// made deliberately dependency-free so it builds anywhere the
// rest of the project builds: it's its own main(), compiled and linked
// against validation.cpp only. See README.md for how to run it via
// `make test`.

#include "validation.h"

#include <iostream>
#include <string>

namespace {

int testsRun = 0;
int testsFailed = 0;

void check(const std::string& name, bool condition) {
    testsRun++;
    if (condition) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        testsFailed++;
        std::cout << "[FAIL] " << name << std::endl;
    }
}

}  // namespace

int main() {
    // --- Valid input should be accepted ---
    check("typical valid input is accepted", validateInput(30, 5, 2, 10).valid);
    check("minimum boundary values (1,1,1,1) are accepted", validateInput(1, 1, 1, 1).valid);
    check("student number at the upper boundary (180) is accepted", validateInput(180, 10, 3, 5).valid);

    // --- Negative numbers ---
    check("negative student number is rejected", !validateInput(-1, 5, 2, 10).valid);
    check("negative group number is rejected", !validateInput(30, -5, 2, 10).valid);
    check("negative tutor number is rejected", !validateInput(30, 5, -2, 10).valid);
    check("negative max time is rejected", !validateInput(30, 5, 2, -10).valid);

    // --- Zero values ---
    check("zero groups is rejected", !validateInput(30, 0, 2, 10).valid);
    check("zero tutors is rejected", !validateInput(30, 5, 0, 10).valid);
    check("zero students is rejected", !validateInput(0, 5, 2, 10).valid);
    check("zero max time is rejected", !validateInput(30, 5, 2, 0).valid);

    // --- Student number cap (> 180) ---
    check("student number just above the cap (181) is rejected", !validateInput(181, 5, 2, 10).valid);
    check("student number far above the cap is rejected", !validateInput(1000, 5, 2, 10).valid);

    // --- Multiple invalid fields at once ---
    check("multiple invalid fields are still reported invalid", !validateInput(-5, 0, -1, 0).valid);

    // --- Error messages are non-empty exactly when invalid ---
    check("invalid result carries a non-empty error message", !validateInput(0, 5, 2, 10).errorMessage.empty());
    check("valid result carries no error message", validateInput(30, 5, 2, 10).errorMessage.empty());

    std::cout << std::endl << (testsRun - testsFailed) << "/" << testsRun << " tests passed." << std::endl;

    return testsFailed == 0 ? 0 : 1;
}
