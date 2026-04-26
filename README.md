# Polen

**Polen** is a competitive programming problem I created that was officially used in the 9th grade problem set at the Romanian National Olympiad in Informatics, one of the highest-level programming contests for pre-university students in Romania.

This repository contains the full working package used around the problem: statements, editorial, official solution, alternative solutions, validators, generator, and test-generation scripts.

The problem statement, editorial, and official explanations are stored in `docs/`. This README focuses only on the repository layout and on how the files are intended to be used.

## Repository Structure

- `docs/` - official documentation files:
  - `statement ro.pdf` - Romanian statement.
  - `statement hu.pdf` - Hungarian statement.
  - `Editorial.pdf` - official editorial.
- `main.cpp` - official accepted solution.
- `solutions/` - additional solution files, including brute-force, partial, and full-score approaches.
- `generator.cpp` - source code for the test generator.
- `generate_tests.sh` - WSL/Linux script used to generate the test set.
- `validators/` - validators for the individual subtasks.
- `tests/` - output directory for generated tests.
- `wsl_build/` - intermediate files produced or used by the generation workflow.
- `zip_split_wsl.sh` - helper script for creating and splitting test archives.
- `polen.in`, `polen.out` - local sample/input-output files used while testing.

## Documentation

The official materials are kept separate from the source code:

```text
docs/
+-- statement ro.pdf
+-- statement hu.pdf
`-- Editorial.pdf
```

Use these files for the statement, constraints, scoring, and explanation of the intended solution.

## Solutions

The `solutions/` directory contains multiple implementations used during development and verification:

- brute-force solutions for small tests;
- partial solutions for specific requirements or subtasks;
- the full-score solution, also available as `main.cpp`.

These files are useful for cross-checking generated tests and comparing outputs between slower and faster approaches.

## Validators

The `validators/` directory contains one validator per subtask:

```text
validators/
+-- sub1.cpp
+-- sub2.cpp
+-- ...
`-- sub10.cpp
```

Each validator reads from `polen.in` and checks whether the input satisfies the restrictions of its corresponding subtask.

## Building

Compile the official solution:

```bash
g++ -std=c++17 -O2 -pipe -static -s main.cpp -o main
```

Compile the generator:

```bash
g++ -std=c++17 -O2 generator.cpp -o generator
```

## Generating Tests

Generate tests in the default location:

```bash
./generate_tests.sh
```

Generate tests in a custom directory:

```bash
./generate_tests.sh path/to/tests
```

Create and split a test archive:

```bash
./zip_split_wsl.sh tests polen_tests 25
```

## Notes

The scripts are intended to be run from a Linux or WSL environment. Some generated binaries and intermediate files may appear in `wsl_build/` during the test generation process.
