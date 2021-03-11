# quesler
An experimental language and compiler that compiles into Phantasy Star Online quest file format. The purpose of this project is to explore the possibilities of a high-level language for PSO quest development. The project also includes an interpeter and a disassembler for compiler development purposes.

## Features
Quesler as a language is imperative and dynamically-typed, and has curly-bracket syntax. Quesler aims to keep the runtime library minimal. The syntax resembles a subset of JavaScript.

Following is an incomprehensive list of language features that may or may not yet be implemented and are subject to change.

### Variables and data types
```
let foo = 123; // Declaring and defining a variable.
let bar; // Undefined variables are implicitly initialized to 0.

foo = 3.14; // Variables are dynamically typed and mutable.
bar = true; // Boolean literals are `true` and `false`.
```
Variables can be declared with the `let` keyword. Variables are dynamically typed.  Currently the available data types are signed 32-bit integers and 32-bit floating-point numbers. For the purposes of boolean checks the value 0 is considered falsy and any other values are considered truthy. The boolean literals `true` and `false` are translated to the integers 1 and 0 respectively. Variable names are block-scoped. Variable names must be alphanumeric and must start with a letter.

### Expressions
In order of precedence from highest to lowest:
* **( )**: Grouping operator and function call.
* **-**, **!**: Unary negation and logical NOT operators.
* **/**, **\***, **%**, **+**, **-**: Arithmetic operators.
* **&&**, **||**: Logical AND and OR. These operators are short-circuiting.
* **<**, **>**, **<=**, **>=**, **==**, **!=**: Comparison operators.
* **=**: Assignment is also an expression.

### Conditional statements
```
if (true) {
    // Yup
} else if (false) {
    // Nope
} else {
    // Unreachable!
}
```
The conditional statements are `if`, `else if` and `else`.

### Loop statements
```
for (let i = 0; i < 10; i++) {
    frob(i);
}

while (true) {
    if (twiddle()) {
        break;
    }
}
```
Traditional `for` and `while` statements are supported, as are the `break` and `continue` statements.

### Functions
```
function fib(n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

fib(7);
```
Functions can be defined with the the `function` keyword. Functions can take multiple arguments and return one value. Recursion is supported.

### Entry function
```
function main() {
    // Execution starts here.
}
```
Each quesler program must have a function named "main", which is where the execution of the program starts. For those familiar with qedit scripting, this is the "0" label.

### PSO quest opcodes
```
set_episode(0);
BB_Map_Designate(0, 0, 0, 0);

let diff = get_difflv2();
if (diff == 3) {
    // ...
} else {
    // ...
}
```
Quest opcodes can be called just like any other function using the standard qedit names. If the opcode has an output argument then that argument must be omitted and the output is instead given as the return value of the function call. Opcode names are protected and may not be shadowed or overwritten.

### Unsafe quest opcodes
Using the following opcodes may conflict with the quesler runtime library and is considered unsafe:
* call, ret
* All of the jump opcodes
* stack_push, stack_pop, stack_pushm, stack_popm

### Print statement
```
let foo = 123;
print foo; // Prints 123 into a popup window (or stdout in the interpreter).
```
The `print` statement translates into a call to `window_msg` and `winend`.

### Plans for the future
* Strings
* Arrays
* Structs
* Switch-statements
