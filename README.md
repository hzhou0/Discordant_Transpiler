## Introduction
**Discordance is a language that compiles into C++.** Despite it's insistence on importing code via Copy Pasting, C++ is an wickedly fast and sometimes
even readable language. Discordance attempts to highlight the good parts of C++ and paper over the bad parts.

Discordance is just C++. It compiles into C++ source with no performance penalties, and the header is generated for you. 
C++ constructs can be used in discordance, and vice versa.   
## Overview  
**main.dis**
```c++
#include <iostream>
using namespace std;
//dynamic function
@ return_any(@ any):
  return any;
//Functions can be declared with python syntax
int main():
    //as can classes
    class apple:
        string color="green";
        //variables declared as type @ can take on any primitive value, including std::string
        @ weight="2kg";
        apple():
            weight=2;
            weight=2.0+weight;
            //use if just like a normal variable
            cout<<weight;
    //easy declaration of vectors
    int numbers[10?][?]={     //[num?] will declare a vector, [?] a deque
    {1,2,3,4,5,6,7,8,9,10},
    {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10}
    };
    //python-style for statements
    for row in numbers:
        for column in row:
            cout<<column<<endl;
    //arrays of unlike things
    @ bucket[?]={2.0, "chocolate milk", "15789"};
    for item in bucket:
        cout<<item<<endl;
    //array slicing
    auto small_bucket=bucket[1:2];
    for item in small_bucket:
        cout<<item<<endl;
    //dynamic functions
    cout<<return_any(2.0)<<endl;
    cout<<return_any(5)<<endl;
    cout<<return_any("hello")<<endl;
    return 0;
```
**main.h**
```c++
// discordance.h
//
#ifndef LZZ_discordance_h
#define LZZ_discordance_h
#include "__Discordance.h"
using namespace discordance;
using discordance::deque; using discordance::vector; using discordance::var;
#include <iostream>
#define LZZ_INLINE inline
discordance::var return_any (discordance::var any);
int main ();
#undef LZZ_INLINE
#endif
```
**main.cpp**
```c++
// discordance.cpp
//

#include "discordance.h"
#include "__Discordance.h"
using namespace discordance;
using discordance::deque; using discordance::vector; using discordance::var;
#define LZZ_INLINE inline
using namespace std;
discordance::var return_any (discordance::var any)
                                                   {
  return any;
}
int main ()
          {
    class apple{
        string color="green";
        discordance::var  weight="2kg";
        apple(){
            weight=2;
            weight=2.0+weight;
            cout<<weight;
        }
    };
    discordance::deque<discordance::vector<int >> numbers={
    {1,2,3,4,5,6,7,8,9,10},
    {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10}
    };
    for ( auto row : numbers ){
        for ( auto column : row ){
            cout<<column<<endl;
        }
    }
    discordance::deque<discordance::var  > bucket={2.0, "chocolate milk", "15789"};
    for ( auto item : bucket ){
        cout<<item<<endl;
    }
    auto small_bucket=bucket.slice(1,2);
    for ( auto item : small_bucket ){
        cout<<item<<endl;
    }
    cout<<return_any(2.0)<<endl;
    cout<<return_any(5)<<endl;
    cout<<return_any("hello")<<endl;
    return 0;
}
#undef LZZ_INLINE
```
## Full Documentation
View the install instructions and full docs on github pages
[https://neverlucky123.github.io/Discordant_Transpiler/]()

