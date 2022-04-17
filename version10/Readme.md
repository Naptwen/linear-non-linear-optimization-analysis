BUG

1. ~~Bug when detect number or string the negative number be detected as string~~
the algorithm https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
is simpliy find the positive interger number
So it can't distinguish . and - sign
so I changeed the condition



2. When I tried to seperate the code and heeader part
    VSCODE compiler linker shows link error arm64 
So too lazy to find and edit the linker just put all in the hpp file

3. ~~Has problem when do massive data sorting by merge algorithm~~
just a mistake for printf 
algorithm has no error


FIX
- Has problem for when reading the quatation mark inside quatation
- As counting the quatation mark with several condition checking fix 
- change the Show function as limited the char printing printf("%.4s")

Update
- HTML GRPHIC 
- TABLE, BOX PLOT, HISTOGRAM (WIDE form and SHORT form(shrink))
- xy Plot, line graph
- Multiple selection 
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
    temp.SORT("Number", "Name", "Money", NULL);
    // temp.SELECT<float>("Number", "10", ABOVE);
    temp.SELECT<float>("Number", "10", OVER);
    temp.SHOW();
    cout << temp.SUM("Number") << endl;
    cout << sqrt(temp.VAR("Number")) << endl;
    cout << temp.AVG("Number") << endl;
    // temp.HTML_TABLE_DRAW("test.html");
    // temp.HTML_BOX_GRAPH("test.html", 500, 500);
    // temp.HTML_HISTO_GRAPH("test.html",1000, 500, 100, WIDE);
    // temp.HTML_PLOT_GRAPH("test.html", 500, 500,"Number", "Age");
    temp.HTML_LINE_GRAPH("test.html", 500, 500,"Number", "Age");
    temp.UNSELECT();
    return 0;
}
```
