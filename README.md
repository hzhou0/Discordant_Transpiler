# Discordant Transpiler
The transpiler for **Discordant**, a language that compiles to C++, written in Python. 
## Installation Instructions
### Perquisites
+  Windows 10
    +   Unix and Linux releases planned
+ g++, part of the gcc toolchain, is required for building executables  
    +  Install with http://mingw.org/ for windows  
    +  Check with `g++ --version`
### Install
   + Download Transpiler.exe and testfile.dis from the release page
   
   + Add Transpiler.exe to path
        
   + `Transpiler run .../discordance.dis`  

   
### How to Use
    Transpiler [run/make/transpiler] [PATH TO FILE] -I[directories to search in for includes]
## Implemented features
+ `If` `else if` `else` `while` `do..while` `for` statements by indentation  
+ Class and function definitions by indentation  
+ Statement conditions are no longer required to be bracketed  
    + example:` if num_a==2: do stuff` vs `if(num_a==2){do stuff}`  
+ Range based `for` loops are assumed to have an auto variable as condition  
+ Dynamic typing, modeled after `std::any`, can implicitly cast to any primitive value  
    + example: `@ my_var; my_var=2; my_var+=3.5; my_var="hello"`;   
+ Initialize vectors/deque with simplified syntax  
    + example: `typename var[100?]` vs `vector<typename> var(100)`;  
    + If no number is given, a deque is created instead. Deques have better performance if memory is not reserved beforehand. 
+ Auto-generated header files with lzz. Just write the source file and a .h file is generated as part of the build pipeline. 
## Features under development
+ Postfix `if`. This will be converted into c++ ternary statements.
    +  example: `my_num=2 if x==3, 3 if x==5, 8 if c==9`
+ Recursive build tree to replace make or cmake by parsing includes
+ STL container slicing
## Features being considered
+  None
## Features discarded
+   Single if line statements
    +   current syntax is sufficient
+   Implicit multithreading
    +   too hardware-dependant to implement
+   List comprehensions
    +   range based `for` loops are sufficient and more readable
+   `when` statement
    +   c++ threads cannot be destroyed, and thus the statement  
    cannot go out of scope properly
+   Chain comparisons
    +   the transpiler is contextless, and cannot parse `<` and `>`  
    due to usage in c++ templates
