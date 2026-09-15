# Multithreaded Lab Scheduling Simulator

A C++ simulation of a classroom where students are split into groups and cycled through a limited number of lab rooms, each staffed by a tutor. It's built with `std::thread`, `std::mutex`, and `std::condition_variable` to coordinate students, tutors, and a teacher thread that assigns groups and hands out lab rooms as they free up.

## How it works

- **N students** arrive and check in with the teacher.
- The teacher randomly splits them into **M groups** (as evenly as possible).
- **K tutors** each run a lab room; when a room is free, the tutor announces it.
- The teacher assigns waiting groups to free rooms in order. Once all students in a group have entered, the tutor runs the exercise for a random duration (bounded by **T**), then the group leaves and the room becomes available again.
- Once every group has been through a lab, idle tutors are sent home, and the simulation ends after all student and tutor threads finish.

## Build

Requires a C++17 compiler with pthread support.

```
make
```

This produces a `main` executable. Run `make clean` to remove build artifacts.

## Usage

```
./main
```

You'll be prompted for four values:

| Input | Meaning |
|-------|---------|
| N | Number of students |
| M | Number of groups |
| K | Number of tutors / lab rooms |
| T | Maximum time an exercise can take |

The program then prints a running log of arrivals, group assignments, lab entries/exits, and exercise completions as the simulation plays out.

## Input validation

All four values are validated before the simulation starts:

| Input | Rule |
|-------|------|
| N | Positive integer, no more than 180 |
| M | Positive integer |
| K | Positive integer |
| T | Positive integer |

Negative numbers, zero, non-numeric input, and a student count over 180 are all rejected. If any value is invalid, the program prints an error describing the problem and exits immediately (status code `1`) without starting the simulation.

## Tests

The validation logic lives in `validation.h` / `validation.cpp` and has its own test suite (`test_validation.cpp`), covering negative numbers, zero groups/tutors/students, the 180-student cap, boundary values, and combinations of invalid fields.

Run it with:

```
make test
```

This builds the `test_validation` executable and runs it, printing a `[PASS]`/`[FAIL]` line per case and a final `x/y tests passed` summary. It exits with status `0` if everything passed, `1` otherwise.
