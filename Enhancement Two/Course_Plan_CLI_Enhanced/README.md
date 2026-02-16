# Modular Advising Tool (BST vs AVL)
**A Modular C++ Academic Advising System with Data Structure Benchmarking**

## Overview

The Modular Advising Tool is a command-line C++ application that loads academic course data from a CSV file, stores it in tree-based data structures, and allows users to view a sorted course schedule, look up individual course details, and benchmark search performance.

This project began as a **CS300 Data Structures and Algorithms final assignment**, where a Binary Search Tree (BST) was used to store and search course records. The original implementation demonstrated functional correctness but tightly coupled data loading, tree logic, and user interaction.

This enhanced version refactors the system into a **modular, extensible architecture** and introduces a **self-balancing AVL tree implementation** alongside the original BST. A built-in benchmark mode now compares empirical search performance between the two structures, transforming the project from a simple implementation exercise into a performance-driven, systems-oriented artifact.

---

## Learning Objectives Demonstrated

- Implementation of tree-based data structures (BST and AVL)
- Understanding of algorithmic time complexity and tradeoffs
- Modular C++ design and separation of concerns
- Empirical performance benchmarking
- Input normalization and resilient CSV parsing
- Basic test construction without external frameworks

---

## Key Enhancements Over Original Project

- Introduced a self-balancing **AVL Tree** implementation  
- Added a **common tree interface (`ICourseTree`)** for interchangeability  
- Implemented **search benchmarking** (BST vs AVL) using identical workloads  
- Hardened CSV parsing (trimming, normalization, safe skipping of malformed rows)  
- Refactored into modular source and header files  
- Added smoke and targeted tree tests  
- Added professional inline documentation and Doxygen-style comments  

---

## Features

- Load course records from a CSV file  
- Print ordered course schedule (in-order traversal)  
- Print details for a single course (title + prerequisites)  
- Benchmark repeated search performance (BST vs AVL)  
- Modular design for maintainability and testability  

---

## Architecture Overview
```
CLI (main.cpp)
   |
   +-- csv_loader  ---> parses CSV into index map
   |
   +-- ICourseTree interface
         |
         +-- BstTree (unbalanced)
         +-- AvlTree (self-balancing)
   |
   +-- benchmark ---> measures search performance
   |
   +-- utils ---> trimming, case normalization, CSV tokenization
```
Both trees are populated from the same parsed dataset to ensure **apples-to-apples benchmarking**.

---

## Project Structure
```
Modular-Advising-Tool/
  include/
    avl.h
    bst.h
    benchmark.h
    course.h
    csv_loader.h
    tree_iface.h
    utils.h
  src/
    avl.cpp
    bst.cpp
    benchmark.cpp
    csv_loader.cpp
    utils.cpp
    main.cpp
  tests/
    smoke_test.cpp
    test_trees.cpp
```
---

## Clone Repository
```bash
git clone https://github.com/eric-foster/Modular-Advising-Tool.git  
cd Modular-Advising-Tool
```
---

## Build
```bash
g++ -std=c++17 -O2 -Iinclude src/*.cpp -o course_planner
```
---

## Run
```bash
./course_planner
```
or
```bash
./course_planner "CS 300 ABCU_Advising_Program_Input.csv"
```
---

## Menu Options

1. Load Courses  
2. Print Schedule (AVL)  
3. Print Course (AVL)  
4. Benchmark Search (BST vs AVL)  
9. Exit  

---

## Benchmarking (BST vs AVL)

Menu option **4** executes repeated searches across all course IDs for both trees.

Reported metrics:
- Total searches  
- Total time (microseconds)  
- Average time per search (microseconds)

---

## CSV Format

CourseId,Title,Prereq1,Prereq2,...

Example:
CS200,Data Structures,CS100

---

## Unit Tests

Smoke Test:
```bash
g++ -std=c++17 -I../include ../src/avl.cpp ../src/benchmark.cpp ../src/bst.cpp ../src/csv_loader.cpp ../src/utils.cpp tests/smoke_test.cpp -o smoke_test  
./smoke_test
```
Tree Tests:
```bash
g++ -std=c++17 -I../include ../src/avl.cpp ../src/benchmark.cpp ../src/bst.cpp ../src/csv_loader.cpp ../src/utils.cpp tests/test_trees.cpp -o test_trees  
./test_trees
```

---

## Future Improvements

- Add delete operation for trees  
- Add clear/reset methods for reload support  
- Introduce unit test framework (Catch2 or GoogleTest)  
- Add persistence (save/load binary tree snapshot)  
- Extend benchmark to include insert timing  

---

## License
For educational and portfolio use.