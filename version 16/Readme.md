Update
1. add new plot line graph algorithm with Cubis spline
2. Chnage some algorihtm for sorting csv data
3. ```cs
4. #include "vstatic_method.hpp"
#include "vcsv_reader.hpp"
#include "vLPNLP.hpp"
using namespace std;
int main()
{
    CSV temp("sample.csv");
    temp.HTML_CLEAR("test.html");
    temp.SORT("A", NULL);
    temp.SELECT("H","N", BELOW);
    temp.SELECT("A", EMPTY, ALL);
    temp.SELECT("B", EMPTY, ALL);
    temp.SELECT("G", EMPTY, DEDUPLE);
    temp.SELECT("C", EMPTY, DEDUPLE);
    temp.SELECTSHOW();
    temp.HTML_PLOT_GRPAH("test.html", 700, 700,"A", "G", true);
    temp.HTML_LINE_GRAPH("test.html", 500, 500,"A", "G");
    return 0;
}
5. ```
