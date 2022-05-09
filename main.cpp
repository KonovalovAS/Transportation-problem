#include "myheader.h"

int main()
{
    srand(time(0));

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
    //P.show_cond();

    int N = pow(n,3);
    double best = -1;
    for(int i=0; i<N; i++){
        Solution A( &P );
        A.calculate();
        //if(best>A.Cost) cout << "\t";
        best = (best<0) ? A.Cost : min( best, A.Cost );
        //cout << A.Cost << "\n";
    }
    cout << best;

    return 0;
}
