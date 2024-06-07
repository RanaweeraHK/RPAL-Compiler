
# RPAL Compiler Design in C++

Welcome to the RPAL Compiler Design project! This repository contains the implementation of a compiler for the RPAL programming language, developed in C++. RPAL, short for Right-reference Pedagogical Algorithmic Language, is a functional programming language designed primarily for educational purposes, to illustrate the principles of programming languages and compiler construction.

## Overview

This project is structured to include the following major components:

1. **Lexical Analyzer (Lexer)**: The lexical analyzer is responsible for reading the input source code and converting it into a stream of tokens. Tokens are the atomic units of syntax that the parser will use to generate the syntax tree.

2. **Parser**: The parser takes the stream of tokens produced by the lexer and constructs a parse tree (or abstract syntax tree, AST). This tree represents the syntactic structure of the source code according to the grammatical rules of RPAL.

3. **Common Subexpression Elimination (CSE) Machine**: The CSE machine is an optimization step that identifies and eliminates redundant subexpressions in the parse tree. This is an important step in optimizing the performance of the generated code.

## Components

### Lexical Analyzer
The lexical analyzer scans the source code and breaks it down into meaningful symbols or tokens. This phase includes:
- Token identification (keywords, identifiers, literals, operators, etc.)
- Ignoring whitespace and comments
- Error handling for unrecognized symbols

### Parser
The parser processes the token stream and generates the abstract syntax tree (AST). This phase involves:
- Syntax analysis based on RPAL grammar
- Construction of the parse tree
- Syntax error detection and reporting

### CSE Machine
The Common Subexpression Elimination (CSE) machine optimizes the parse tree by:
- Detecting repeated subexpressions
- Eliminating redundancies
- Enhancing the efficiency of the code execution

## Installation and Usage

To build and run the RPAL compiler, follow these steps:

1. **Clone the repository**:
   ```sh
   git clone https://github.com/RanaweeraHK/Compiler.git
   cd Compiler
   ```

2. **Build the project**:
   Ensure you have a C++ compiler installed (like g++ or clang). Then, run:
   ```sh
   make
     ```

4. **Run the compiler**:
   Provide an RPAL source file to the compiler

   ## References
   ![image](https://github.com/RanaweeraHK/Compiler/assets/129282753/e3880cea-4373-4bc7-8201-af6cf5c84a70)

   ![image](https://github.com/RanaweeraHK/Compiler/assets/129282753/569ddc64-61cb-4b39-8d22-d25255dcd22c)
   
   ![image](https://github.com/RanaweeraHK/Compiler/assets/129282753/13fc2e59-416b-44a9-a4d2-4e0ed78f3137)

   
   


   
