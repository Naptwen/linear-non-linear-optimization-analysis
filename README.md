# Market-Data-analysis

this is open source statistical analysis program

15 April, 2022
Update
version 08 

The datail is in the folder


- Probaility algorithm


- Null Hypothesis algorithm


- CVS File reading and Sorting(Merge Sorting)



example:
```cs
          #include "vstatic_method.hpp"

          using namespace std;


          int main(){
              PHANTOM bbb;
              bbb.pm1 = 0;
              bbb.pp1 = 0.70f;
              bbb.p1 = 668.0f/1003.0f;
              cout << "SAMPLE PROPORTION : " << bbb.p1 << endl;
              bbb.n1 = 1003;
              cout << "z : " << bbb.ospzt() <<endl;
              cout << "p : " << bbb.pval(bbb.z, RIGHT) <<endl;
              CSV temp("data_test.csv", true);
              temp.show();
              temp.SORT<float>("Number");
              temp.show();
              return 0;
          }
```
result
          Z FILE OPEN
C FILE OPEN
SAMPLE PROPORTION : 0.666002
z : -2.3496
p : 0.9857
CVS data_test.csv is OPEN
________________________________
[0]    Name        Number      Money       Age         
[1]    e           10          100         20          
[2]    d           12          320         10          
[3]    c           9           1000        40          
[4]    b           NULL        1230        22          
[5]    a           33          1000        41          
[6]    f           99          100000      67          
[7]    g           23          50          12          
[8]    h           29          11          30          
[9]    i           NULL        100         2           
________________________________
[0]    Name        Number      Money       Age         
[1]    c           9           1000        40          
[2]    e           10          100         20          
[3]    d           12          320         10          
[4]    g           23          50          12          
[5]    h           29          11          30          
[6]    a           33          1000        41          
[7]    f           99          100000      67          
[8]    b           NULL        1230        22          
[9]    i           NULL        100         2   


13 April Version 07

- Socket TCP/IP 

- Web Server Socket connection
