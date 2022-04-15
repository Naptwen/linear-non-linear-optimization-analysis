code
```cs
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
    temp.SORT("Money", "Name", "Age", NULL);
    temp.show();
    temp.SELECT<float>("Number", "30", OVER);
    temp.show();
    return 0;
}
```
data
```
Name,Number,Money,Age,
"e",10,100,20,
"d",12,320,10,
"c",9,1000,40,
"b",,1230,22,
"a",33,1000,41,
"f",99,100000,67,
"g",23,50,12,
"h",29,11,30,
"i",,100,2,
"j",20,100,10,
"k",22,100,10,
"l",24,100,10,
"m",25,100,10,
"n",12,100,10,
```

result
```
Z FILE OPEN
C FILE OPEN
SAMPLE PROPORTION : 0.666002
z : -2.3496
p : 0.9857
CVS data_test.csv is OPEN
________________________________
[    0]    Name        Number      Money       Age         
[    1]    e           10          100         20          
[    2]    d           12          320         10          
[    3]    c           9           1000        40          
[    4]    b           NULL        1230        22          
[    5]    a           33          1000        41          
[    6]    f           99          100000      67          
[    7]    g           23          50          12          
[    8]    h           29          11          30          
[    9]    i           NULL        100         2           
[   10]    j           20          100         10          
[   11]    k           22          100         10          
[   12]    l           24          100         10          
[   13]    m           25          100         10          
[   14]    n           12          100         10          
________________________________
[    0]    Name        Number      Money       Age         
[    1]    h           29          11          30          
[    2]    g           23          50          12          
[    3]    e           10          100         20          
[    4]    i           NULL        100         2           
[    5]    j           20          100         10          
[    6]    k           22          100         10          
[    7]    l           24          100         10          
[    8]    m           25          100         10          
[    9]    n           12          100         10          
[   10]    d           12          320         10          
[   11]    a           33          1000        41          
[   12]    c           9           1000        40          
[   13]    b           NULL        1230        22          
[   14]    f           99          100000      67          
________________________________
[    0]    Name        Number      Money       Age         
[    1]    h           29          11          30          
[    2]    g           23          50          12          
[    3]    e           10          100         20          
[    4]    i           NULL        100         2           
[    5]    j           20          100         10          
[    6]    k           22          100         10          
[    7]    l           24          100         10          
[    8]    m           25          100         10          
[    9]    n           12          100         10          
[   10]    d           12          320         10          
[   11]    a           33          1000        41      <- it should be green  
[   12]    c           9           1000        40          
[   13]    b           NULL        1230        22          
[   14]    f           99          100000      67      <- it should be green
```
