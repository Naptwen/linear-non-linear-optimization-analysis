20, Aprill 2022

For the study again linear algebra and practical experience applying mathematics in computer programming, \
I created this Linear Program for optimization.\
The principle algorithm is referenced from the class note by Professor, Philip E. Gill UCSD \
Thanks to taking the class by Professor, Philip E. Gill, I could study a lot and more deeply in Linear Algebra and how applying the theorem related to LP.

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
#include "vNLP.hpp"

int main(){
    LP test;
    test.READING("TEST.csv");
    test.LP_BEST();
    if(!test.answer_x.empty()){
        cout<<test.answer_x<<endl;
        cout<<test.answer<<endl;
    }
    return 0;
}
```



Copyright (c) <2022> Useop Gim\
    GNU General Public License
