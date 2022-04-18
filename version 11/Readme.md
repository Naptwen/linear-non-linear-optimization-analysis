April 19. 2022
- Matrix for linear algebra
- Echelon elimination (include Gaussian and upper triangular reduce)
- Ax=b Solution algorithm (using Echleon, if inf many solution set the free variables are 1)

```cs

int main(){
    MATRIX<float> A(3,4);
    MATRIX<float> B(4,3);
    MATRIX<float> C(3,3);
    float t[] = {
        1,0,2,0,
        0,1,2,2,
        0,0,0,2};
    float u[] = {
        -1,2,6,
        2,3,5,
        0,3,4,
        4,6,10};
    float k[] = {
        1,1,1,
        0,1,1,
        0,0,0};
    A = t;
    B = u;
    C = k;
    vector<float> ya = {1,2,3};
    vector<float> yb = {1,2,3,4};
    vector<float> yc = {1,2, 3};
    vector<float> xa = A.SOLVE(ya);
    vector<float> xb = B.SOLVE(yb);
    vector<float> xc = C.SOLVE(yc);
    cout << "sol----A\n";
    for(auto vec : xa)
        cout << vec << endl;
    cout << "sol----B\n";
    for(auto vec : xb)
        cout << vec << endl;
    cout << "sol----C should be no solution\n";
    for(auto vec : xc)
        cout << vec << endl;
    return  0;
}   
```
