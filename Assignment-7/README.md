# Assignment 07: University Timetable Scheduling

## Aim
To design a university examination timetable using Graph Colouring with Backtracking so that examinations with common students do not overlap and the number of examination slots is minimized.

## Problem Statement
A university needs to schedule examinations for students enrolled in overlapping courses. Two examinations cannot take place at the same time if any student is enrolled in both. Graph Colouring is used to assign conflict-free examination slots, and Backtracking is used to find the minimum number of slots required.

## Objectives
- Understand Graph Theory and Graph Colouring.
- Implement the m-Colouring problem using Backtracking for examination scheduling.
- Consider classroom capacity and availability constraints.

## Theory
Graph Colouring assigns colours to graph vertices such that no two adjacent vertices have the same colour. In examination scheduling, each subject is represented by a vertex, each conflict between subjects is represented by an edge, and each colour represents an examination time slot.

### Backtracking
Backtracking builds the solution one subject at a time. For each subject, it tries every colour from `0` to `m-1`. A colour is assigned only if it is **safe**, i.e., no conflicting subject already has that colour. If a subject cannot be given any safe colour, the algorithm **backtracks** to the previous subject, removes its colour and tries the next one.

To minimize the number of slots, the m-Colouring check is run for `m = 1, 2, 3, ...`. The first `m` for which backtracking succeeds is the **chromatic number** of the conflict graph, which is the minimum number of exam slots.

Unlike Greedy colouring, which can use more colours than necessary depending on vertex order, Backtracking always finds the optimal (minimum) number of slots.

## Algorithm
1. Create a vertex for every subject.
2. Add edges between subjects that have common students.
3. For `m = 1` to `n`:
   1. Mark all subjects as uncoloured.
   2. Call `colourGraph(0, m)`.
   3. If it succeeds, `m` is the minimum number of slots. Stop.
4. `colourGraph(u, m)`:
   1. If `u == n`, all subjects are coloured. Return true.
   2. For each colour `c` from `0` to `m-1`:
      - If `isSafe(u, c)`, assign `slot[u] = c`.
      - If `colourGraph(u + 1, m)` returns true, return true.
      - Otherwise reset `slot[u] = -1` (backtrack).
   3. Return false.
5. For each slot, allocate the smallest free classroom that can hold each subject's students (best fit).
6. Display the examination timetable and the total number of slots.

## Pseudocode
```text
isSafe(u, c)
    FOR each subject v
        IF graph[u][v] AND slot[v] == c
            RETURN false
    RETURN true

colourGraph(u, m)
    IF u == n
        RETURN true
    FOR c = 0 to m-1
        IF isSafe(u, c)
            slot[u] = c
            IF colourGraph(u + 1, m)
                RETURN true
            slot[u] = -1        // backtrack
    RETURN false

FOR m = 1 to n
    IF colourGraph(0, m)
        minimumSlots = m
        BREAK
```

## Complexity Analysis
- Time Complexity: O(m^V × V) per m-colouring attempt in the worst case, where all colour combinations are explored.
- Space Complexity: O(V^2 + R) for the adjacency matrix, slot array, recursion stack and room data.

Where:
- V = Number of subjects
- m = Number of colours (slots) being tried
- R = Number of classrooms

## Technologies Used
- Programming Language: C++
- Algorithm: Backtracking (m-Colouring)
- Compiler: g++ (C++11 or later)

## How to Run

```bash
g++ timetable_backtracking.cpp -o timetable
./timetable
```

Sample input and output are in `output.txt`.

## Conclusion
Implemented Graph Colouring using Backtracking to generate a conflict-free examination timetable with the minimum number of slots, and allocated suitable classrooms according to capacity constraints.
