# Map of language implementation

## Front end
Front end is concerned with the source language that the program is written in.
Each stage makes the code simpler. Slowly leading to the back end.

### Scanning
Scanning or lexing means to read a stream of characters and chunk it into a series words (tokens).<br>
**Scanner** usually ignores things like whitespaces or comments because they don't mean anything.<br>
Some examples of token: . (separator), "Hi!" (string literal), myFunc (identifier), 123 (number) etc.

### Parsing
The process of giving our syntax a grammar.<br>
**Grammar** is the ability to compose expressions and statements out of small parts (chunks/tokens).<br>
**Parser** takes tokens and builds a tree structure (parse tree/syntax tree) that shows the nested nature of our grammar.
- Parser also tells about syntax errors.

### Static analysis
So far we know about syntax of the language stored in our tree, but suppose an expression a + b.
- We don't know the scope of a and b. Local? Global?

#### Binding/Resolution (First analysis)
For each identifier:
- Find where its defined (the scope).
- Wire them together.

If the language is statically typed, check their types. If not same (we are adding a and b here) return a **Type Error**.<br>
Else, the language is dynamically typed, so check it at runtime.

#### Storing insight from analysis
- As attributes on our tree. Extra nodes that are created earlier, but not filled in earlier.
- Store in a lookup table called symbol table. Use variable identifier as the key.
- Transform tree into another data structure to store the semantics of the code.

## Middle end

### Intermediate representations
Interface between front and back end.
Examples:
- Control flow graph.
- Static single-assignment.
- Continuation-passing style.
- Three-address code.

#### Big use (Shared IR)
Suppose 3 source languages and you want to compile all of them for 3 different backends.
- This means 9 different combinations. Yikes!
- Having a shared IR would mean that it can be used for all 9 combinations, except it has to be written only once.
- Just write the 3 front ends and 3 back ends :3.

### Optimization
Compile-time optimization.
Example: constant folding. Evaluate expressions to the exact values at compile time.

## Back end

### Code generation
Converting the user program into machine code.

#### Bytecode
Decision. Code for an actual CPU (lightning fast) or a virtual machine?
- Used to be a big problem of portability (Lack of virtual machine).
- Fix: Bytecode! Portable. Each instruction is a byte long.

### Virtual machine
Convert bytecode for a chip that can understand it.
2 Options:
1. Little mini compiler. One for every architecture. Good thing is that the pipeline can be reused. You still need multiple of these though.
2. Language Virtual Machine. Emulates a hypothetical chip to run the bytecode. Slow, but you get portability.

### Runtime
Some utilities for the program while it is running.

## Alternative routes
### Tree-walk interpreters
Start compiling by reading through the syntax tree. Our first compilation is like this.

### Transpiler/Source-to-source compiler
Convert a source code into another high-level source code and use that language's compiler.

### Just-in-time compilation
JIT compilation! Automatic optimization of code by recompiling "Hotspots" (performance critical section) with advanced optimizations.

## Compiler vs Interpreter
Compiling: implementation technique. Translating source code into some other source code or machine code.
- Only translation. Not running.<br>
Interpreting: Takes source code and immediately starts running it from source.

## Challenges
### Question 2.1: Pick a language and analyse its parser and lexer
**Ans.** I chose RUST. Found its scanner and parser files inside the compiler folder. They seem to be written in RUST itself (if that's what .rs format is for). They are not using Lex or Yacc for it.

### Question 2.2: Why doesn't every dynamically typed language use JIT?
**Ans.** Ok, so I researched about this for 2 minutes. The thing with JIT is that it adds an additional step during compilation. First run is slow. It is good for long running processes, but bad for short processes.

### Question 2.3: Why does Lisp use both compiler and interpreter?
**Ans.** 
- The interpreter in Lisp is used for testing. Compilation may take a long time for each test run, while an interpreter can do it on the fly.<br>
- The compiler on the other hand produces the final optimized code for stable builds.
