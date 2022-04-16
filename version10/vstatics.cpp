
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
    temp.SELECT<float>("Money", "1000", BELOW);
    temp.SHOW();
    cout << temp.SUM("Number") << endl;
    cout << sqrt(temp.VAR("Number")) << endl;
    cout << temp.AVG("Number") << endl;
    temp.HTML_TABLE_DRAW("test.html");
    temp.HTML_HISTO_GRAPH("test.html",1000, 500, 100, WIDE);
    temp.UNSELECT();
    return 0;
}
