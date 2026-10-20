# CercaPercorso

A C command-line application for managing highway service stations and planning journeys between them. Developed for the **Algorithms and Data Structures project, academic year 2022/2023**.

Each station is identified by its distance from the beginning of the highway and holds a fleet of cars with different driving ranges. A journey consists of successive legs between stations: each leg must be within the range of a car available at its departure station.

The route planner aims to minimize intermediate stops and, for equally short routes, favor stops closer to the beginning of the highway. The supplied implementation contains separate algorithms for travel in increasing and decreasing distance order; route planning is marked as work in progress in the source.

## Features

- Add and remove highway stations.
- Add cars to a station or scrap one car with a specified range.
- Plan routes in either direction along the highway.
- Read commands from standard input and print results to standard output.

## Requirements

- A C compiler supporting C99 or later, such as GCC or Clang.
- The standard C library; no third-party dependencies are required.

## Build and run

The supplied source was provided as a text attachment. Save its contents as `main.c`, then compile:

```sh
gcc -std=c11 -Wall -Wextra -O2 main.c -o cercapercorso
```

Run interactively:

```sh
./cercapercorso
```

Or process a command file:

```sh
./cercapercorso < input.txt > output.txt
```

Enter one command per line. The program has no active menu or prompt and exits at end-of-file. In a Unix-like terminal, press Ctrl+D to finish entering commands. End command files with a newline: the current input loop may skip a final line without one.

## Commands

Command names and response messages are in Italian. Use non-negative integers for station distances and car ranges.

| Command | Description | Response |
| --- | --- | --- |
| `aggiungi-stazione D N R1 ... RN` | Create a station at distance `D` with `N` cars and their ranges. A station distance must be unique. | `aggiunta` or `non aggiunta` |
| `demolisci-stazione D` | Remove the station at distance `D` and its fleet. | `demolita` or `non demolita` |
| `aggiungi-auto D R` | Add one car with range `R` to the station at distance `D`. | `aggiunta` or `non aggiunta` |
| `rottama-auto D R` | Remove one car with range `R` from the station at distance `D`. | `rottamata` or `non rottamata` |
| `pianifica-percorso A B` | Plan a journey from station `A` to station `B`. | Station distances separated by spaces, including both endpoints, or `nessun percorso` |

`N` must match the number of ranges supplied. Multiple cars may have the same range; scrapping removes only one matching car. Adding a car is rejected when the station already has 512 cars. Initial fleet sizes are not checked against that limit, so keep them at or below 512.

Route planning reads the maximum range available at each station and does not remove cars or otherwise change fleet contents.

## Example

Save the following as `input.txt`:

```text
aggiungi-stazione 0 1 25
aggiungi-stazione 20 1 25
aggiungi-stazione 40 1 1
pianifica-percorso 0 40
aggiungi-auto 20 30
rottama-auto 20 30
demolisci-stazione 20
pianifica-percorso 0 40
```

Output:

```text
aggiunta
aggiunta
aggiunta
0 20 40
aggiunta
rottamata
demolita
nessun percorso
```

The car at station `0` can reach station `20`, whose cars can then reach station `40`. Once station `20` is removed, the remaining stations cannot form a route from `0` to `40`.

## Implementation

The supplied program is a single C source file.

| Component | Representation | Purpose |
| --- | --- | --- |
| Stations | Unbalanced binary search tree (`struct node`) | Store stations ordered by distance; support lookup, insertion, deletion, predecessor, and successor operations. |
| Car fleets | Dynamically allocated arrays sorted in descending order | Store car ranges, with the maximum range at index `0`. |
| Route candidates | Singly linked lists (`struct valida`) | Build and adjust sequences of station distances. |

Although some source comments refer to a max-heap, fleets are implemented as sorted arrays using bubble sort.

Key functions:

- `string_parsing`: extracts numeric command arguments.
- `aggiungi_stazione` / `demolisci_stazione`: manage the station tree.
- `aggiungi_auto` / `rottama_auto`: update station fleets.
- `pianifica_percorso_a`: plans travel toward increasing distances.
- `pianifica_percorso_r` / `aggiusta_lista`: plan and adjust travel toward decreasing distances.
- `freeTree`: releases the station tree and fleet arrays at shutdown.

For a tree of height `h`, station lookup, insertion, and deletion take O(h). Because the tree is not balanced, `h` can grow to the number of stations. Sorting a fleet of `m` cars takes O(m²) in the worst case. Route planning performs repeated tree navigation and linked-list updates; its correctness and performance across all edge cases have not been verified.

## Current limitations

The source should be treated as an academic implementation with unfinished edge-case handling:

- Input lines use a 10,000-byte buffer, and argument parsing allocates space for 1,000 integers without bounds checking. Malformed, oversized, or incomplete commands are not validated.
- Adding or scrapping a car when the station tree is empty can dereference a null pointer.
- Route planning assumes valid station pointers and accesses fleet element `0` without checking for an empty fleet. Missing endpoints, identical endpoints, and traversal beyond the first or last station can cause invalid memory access. Reverse planning also contains unchecked pointer accesses.
- Route cleanup can leave allocated list nodes unreleased. Allocation failures are not checked.
- `fflush(stdin)` is used even though the C standard does not define it as a portable way to clear input.

The program compiled with GCC using the command above, with an unused-parameter warning for `n_tappe` in `aggiusta_lista`. The example in this README was executed successfully against the supplied source; it is not a comprehensive test suite.
