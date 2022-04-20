20, Aprill 2022

For the study again linear algebra and practical experience applying mathematics in computer programming, 
I created this Linear Program for optimization.
The principle algorithm is referenced from the class note by Professor, Philip E. Gill UCSD 
Thanks to taking the class by Professor, Philip E. Gill, I could study a lot more deeply in Linear Algebra and how applying the theorem related to LP.

- LP Phase 1 
- LP Standard Phase 1
- LP Simplex
- LP Standard Simplex

This code is not optimized fully (I just used a lot of deep copy vectors instead reference)
I could shrink the code and make the optimizing algorithm but now I just let it.
The aim of this programming is to really create programming that I learn in mathematics lectures.

#SV FILE SHOULD BE THIS PATTERN
No! space required

EX)

```
    min -6x1-9x2-5x3
    
    2x1+3x2+x3<=5
                 
    x1+2x2+x3>=3
    
    x1,x2,x3>=0
```
TEST.CSV FILE SHOULD BE

```
min,-6,x1,-9,x2,-5,x3
2,x1,3,x2,1,x3,"<=",5
1,x1,2,x2,1,x3,">=",3
1,x1,">=",0
1,x2,">=",0
1,x3,">=",0
```

```cs
#include "vmatrix.hpp"
#include "vcsv_reader.hpp"


int main(){
    LP test;
    test.READING("TEST.csv");
    test.LP_BEST();
    return 0;
}
```



Copyright (c) <2022> <Useop Gim>
Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
