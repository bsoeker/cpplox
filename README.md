# cppLox

A high-performance, memory-safe tree-walk interpreter for the Lox programming language, written in modern C++20/C++23. 

This project transitions the Java-based architecture from Bob Nystrom's *Crafting Interpreters* into an idiomatic, modern C++ codebase by prioritizing value semantics, compile-time type safety, and standard library optimizations over classic object-oriented boilerplate.

---

## Key Architectural Highlights

* **Zero Java-style Boilerplate:** Replaced the classic Object-Oriented Visitor Pattern with modern type-safe sums via `std::variant` and `std::visit`. AST node traversal is managed entirely through high-speed, compile-time optimized lambda matching using the `overloaded` struct pattern.
* **100% Memory Safe:** Adheres strictly to RAII (Resource Acquisition Is Initialization). The Abstract Syntax Tree (AST) is built using explicit ownership tracking via `std::unique_ptr` and `std::move` semantics. No raw pointers, no manual `delete` calls, and zero memory leaks.
* **Modular Namespaces:** Encapsulates core diagnostic features, global states, and interpreter components under an open-ended `Lox::` namespace, completely avoiding global scope pollution and naming collisions.
* **Panic-Mode Synchronization:** Features an advanced syntax error recovery loop that allows the parser to shrug off local grammar errors, realign at statement boundaries, and continue identifying further errors down the script.

---

## AST Printer Preview

The parser generates a highly nested Abstract Syntax Tree which can be displayed as fully parenthesized infix notation. 

```text
lox> (10 + 20) * 5 - 3
(((group (10 + 20)) * 5) - 3)
