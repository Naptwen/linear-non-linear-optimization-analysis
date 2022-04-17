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

4. ~~HTML table is not sorted~~
just a mistake in print value algorithm fix the error


FIX
- Has problem for when reading the quatation mark inside quatation
- As counting the quatation mark with several condition checking fix 
- change the Show function as limited the char printing printf("%.4s")

Update
- HTML GRPHIC 
- TABLE, BOX PLOT, HISTOGRAM (WIDE form and SHORT form(shrink))
- xy Plot, line graph
- Multiple selection 
- String Histogram
- Historgram Graph text graphic change
- appending graph and table on html
```cs
#include "vstatic_method.hpp"
#include "vcsv_reader.hpp"
using namespace std;


int main(){
    CSV temp("sample.csv");
    temp.SORT("C", "A", NULL);
    temp.HTML_TABLE_DRAW("test.html");
    temp.HTML_BOX_GRAPH("test.html", 500, 500);
    temp.SELECT("C","B",BELOW);
    temp.HTML_CLEAR("test.html");
    temp.HTML_TABLE_DRAW("test.html");
    temp.HTML_PLOT_GRAPH("test.html", 500, 250,"A", "F");
    temp.HTML_LINE_GRAPH("test.html", 500, 250,"A", "F");
    temp.SELECT("G","1",BELOW);
    temp.SELECTSHOW();
    temp.HTML_HISTO_GRAPH("test.html", "J", 500, 500, 0.1, WIDE);
    temp.HTML_HISTO_GRAPH("test.html", "D", 500, 500, 2000, SHORT);
    temp.HTML_HISTO_GRAPH("test.html", "C", 500, 500, 2000, SHORT);
    temp.UNSELECT();
    return 0;
}
```
