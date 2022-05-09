#include "myheader.h"
#include <fstream>
#include <stdio.h>

Problem p_from_file( string &name ){

    ifstream in(name);

    int n,v,c;
    in >> n >> v >> c;
    Problem P(v,c);
    int d;
    double x,y;
    in>>d>>x>>y;

    P.make_source(pt(0,x,y,0));

    for(int i=1; i<n; i++){
        in>>d>>x>>y;
        P.add_point(pt(i,x,y,d));
    }
    in.close();

    P.show_cond();

    return P;
}

Problem p_from_input(){

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

    return P;
}

Solution simulated_annealing( Problem& P ){

    int n = P.Consumers_number();
    vector<int> best_var (n,0);
    double best = -1;

    Solution A( &P, best_var );
    A.calculate();
    best = A.Cost;
    cout << best << "\n\n";

    vector<int> cur_var (n,0);

    int T = 800;
    while( T > 0 ){

        vector<int> new_var (n);

        for(int i=0; i<n; i++){
            int sgn = (rand()%10>5) ? 1 : -1;
            new_var[i] = abs( cur_var[i] + (sgn*rand())%T );
        }
/*
        for(auto i: new_var)
            cout<<i<<" ";
        cout<<"\n";
*/
        Solution A( &P, new_var );
        A.calculate();

        if( A.exists ){
            if( best > A.Cost )
                best_var = new_var;
            best = min(A.Cost,best);

            int flag = abs(rand())%150;
            if( flag > 100 )
                cur_var = new_var;
            else if(flag > 50)
                cur_var = best_var;

            T--;
        }
    }

    Solution fin( &P, best_var );
    fin.calculate();

    return fin;
}


int main()
{
    srand(time(0));
    string name;
    cin >> name;
    //Problem P = p_from_input();
    Problem P = p_from_file(name);
    Solution fin = simulated_annealing( P );
    fin.show();

    return 0;
}
