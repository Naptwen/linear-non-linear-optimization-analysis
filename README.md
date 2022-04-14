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

13 April Version 07

- Socket TCP/IP 

- Web Server Socket connection
