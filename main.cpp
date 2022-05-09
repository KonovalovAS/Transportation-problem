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

    //P.show_cond();

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
    //P.show_cond();

    return P;
}

void add( vector<int>& v, const vector<int> sum, int n ){
    for(int i=0; i<min(v.size(),sum.size()); i++){
        v[i] = ( v[i] + sum[i] )%n;
    }
}

void increase(vector<int>& v, int n){
    int k = 0;
    while(v[k]==n-1){
        v[k] = 0;
        k++;
    }
    if(k<v.size())
        v[k]++;
}

Solution simulated_annealing( Problem& P ){

    int n = P.Consumers_number() + P.Vehicles_number();
    double best = 1.79E+308;
    vector<int> cur_var (n,0);
    vector<int> best_var (n,0);

    Solution G(&P,true);
    G.calculate();

    Solution Init(&P,cur_var);
    Init.calculate();
    if(Init.exists)
        best = Init.Cost;

    if(G.exists && (G.Cost<Init.Cost) ){
        cur_var = G.Variator();
        best = G.Cost;
        best_var = G.Variator();
    }

    cout << best << "\n";

    int T = n,
        counter = 0,
        limit = 100;

    while( T > 1 ){

        vector<int> new_var = cur_var,
                    sum(n);

        for(int i=0; i<n; i++){
            sum[i] = rand()%T;
        }
        add(new_var,sum,n);

        /*
        for(auto in: new_var)
            cout << in << " ";
        cout<<"\n";
        */

        Solution A( &P, new_var );
        A.calculate();

        if( A.exists && A.Cost > 0 ){
            if( best > A.Cost ){
                best_var = new_var;
                best = A.Cost;
                cout << "\t\t" << best << "\n";
            }

            cur_var = new_var;


            int flag = abs(rand())%150;
            if( flag > 100 )
                cur_var = new_var;
            else if(flag > 50)
                cur_var = best_var;

            counter++;
            if(counter==limit){
                cout << "\t" << (n-T+1) << "/" << n << "\n";
                counter = 0;
                T -= 1;
            }
        }
    }

    Solution fin( &P, best_var );
    fin.calculate();

    return fin;
}

Solution all_options( Problem& P ){

    int n = P.Consumers_number() + P.Vehicles_number();
    double best = 1.79E+308;

    vector<int> best_var (n,0);
    vector<int> var (n,0);

    int N = pow(n,n);
    cout << N << "\n";
    for(int i=0; i<N; i++){
        increase(var,n);

        /*
        for(auto it: var)
            cout << it << " ";
        cout << "\n";
        */

        Solution S(&P,var);
        S.calculate();
        if( S.exists && (best < S.Cost) ){
            best = S.Cost;
            best_var = var;
            cout << "\t" << best << "\n";
        }
    }

    Solution fin( &P, best_var );
    fin.calculate();

    return fin;
}

void all_tests(string name){
    ifstream in (name);
    ofstream out ("0_res.txt");

    string fname;
    double MV;

    int failed = 0;
    int tests = 0;

    in >> fname >> MV;
    while( fname != "end" ){
        tests++;
        Problem P = p_from_file(fname);
        Solution S = simulated_annealing(P);
        double me = S.Cost;

        double percent = 100*(me-MV)/MV;

        out << fname << "\tme: " << me
            << "\tteacher: " << MV
            << "\t" << percent << "%\n";

        cout << fname << "\tme: " << me
             << "\tteacher: " << MV
             << "\t" << percent << "%\n";

        if(percent>20)
            failed ++;

        in >> fname >> MV;
    }

    out << " Failed " << 100*((double)failed/tests) << "%";
    cout << " Failed " << 100*((double)failed/tests) << "%";

    in.close();
    out.close();
}

int main()
{
    srand(time(0));
    string name = "data.txt";
    all_tests(name);
    /*
    cin >> name;
    //Problem P = p_from_input();
    Problem P = p_from_file(name);

    Solution fin = simulated_annealing( P );
    //Solution fin = all_options( P );
    fin.show();
    */
    /*
    Solution g(&P,true);
    g.calculate();
    vector<int> var = g.Variator();

    Solution u(&P,var);
    u.calculate();
    cout << g.Cost << " " << u.Cost;
    */

    return 0;
}
