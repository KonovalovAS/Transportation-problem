#include "myheader.h"

int main()
{
    int n,v,c;
    cin >> n >> v >> c;
    Problem P(v,c);
    int d;
    double x,y;
    cin>>d>>x>>y;

    P.make_source(pt(0,x,y,0));

    for(int i=1; i<n; i++){
        cin>>d>>x>>y;
        P.add_point(pt(i,x,y,d));
    }
    P.show_cond();

    Solution A( &P );
    A.calculate();

    return 0;
}
