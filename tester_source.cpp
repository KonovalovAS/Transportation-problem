#include "tester.h"


bool test::operator<(const test& r){
    return ( this->percent>0 && this->percent<r.percent);
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

void increase(vector<int>& v, int n){
    int k = 0;
    while(v[k]==n-1){
        v[k] = 0;
        k++;
    }
    if(k<v.size())
        v[k]++;
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


void add( vector<int>& v, const vector<int> sum, int n ){
    for(int i=0; i<min(v.size(),sum.size()); i++){
        v[i] = ( v[i] + sum[i] )%n;
    }
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

    int T0 = 3*n,
        T = T0,
        counter = 0,
        limit = 5;

    while( T > 1 ){

        vector<int> new_var = cur_var,
                    sum(n);

        for(int i=0; i<n; i++)
            sum[i] = rand()%T;
        add(new_var,sum,n);

        Solution A( &P, new_var );
        A.calculate();

        if( A.exists && A.Cost > 0 ){
            if( best > A.Cost ){
                best_var = new_var;
                best = A.Cost;
            }

            cur_var = new_var;


            int flag = abs(rand())%150;
            if( flag > 100 )
                cur_var = new_var;
            else if(flag > 50)
                cur_var = best_var;

            counter++;
            if(counter==T*limit){
                counter = 0;
                T -= 3;
            }
        }
    }

    Solution fin( &P, best_var );
    fin.calculate();

    return fin;
}

Problem problem_reader( string &name ){

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

void run( queue<test> &Data, mutex &common, queue<test> &RES ){

    while(!Data.empty()){

        common.lock();
        test t = Data.front();
        Data.pop();
        cout << this_thread::get_id() << " is working on " << t.fname << "\n";
        common.unlock();

        Problem P = problem_reader(t.fname);
        Solution S = simulated_annealing(P);

        t.calculated = true;
        t.me = S.Cost,
        t.percent = 100*(t.me-t.MV)/t.MV;

        int ats0 = 3,
            ats = ats0;
        while(ats>0 && t.percent>20){
            common.lock();
            cout << this_thread::get_id()
                 << ": attempt " << ats0+1-ats << "/" << ats0 << "\n";
            common.unlock();
            S = simulated_annealing(P);
            if(t.me>S.Cost){
                t.calculated = true;
                t.me = S.Cost,
                t.percent = 100*(t.me-t.MV)/t.MV;
            }
            ats--;
        }

        common.lock();
        cout << "Thread " << this_thread::get_id()
        << ": " << t.fname << " is done with result " << t.percent << "%\n";
        RES.push( t );
        common.unlock();

    }
}

void RUN( string &ifname, string &ofname ){

    ifstream in (ifname);

    string name;
    double MV;

    queue<test> Data;
    in >> name >> MV;
    while( name != "end" ){
        test new_test = { name, MV, false, -1, -1 };
        Data.push( new_test );
        in >> name >> MV;
    }
    in.close();

    mutex common;
    queue<test> RES;

    int num = Data.size();
    int req_num_treads = min(num,10);
	int max_threads = thread::hardware_concurrency();
	int num_threads = min(max_threads,req_num_treads);
	vector<thread> threads(num_threads - 1);

	try
    {
		auto start = std::chrono::steady_clock::now();

        for(int i = 0; i < num_threads-1;i++)
			  threads[i] = thread(run,ref(Data),ref(common),ref(RES));

		for(int i = 0; i < num_threads-1;i++)
            threads[i].join();

		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		cout<< "Done by "<< num_threads <<" thread(s) in "<<elapsed.count()<<" milisecs."<<endl;
	}
	catch(...)
	{
		for(int i = 0; i < num_threads-1;i++)
            threads[i].join();
        throw;
	}

    ofstream out (ofname);
    int better(0),
        failed(0);

    while(!RES.empty()){

        test t = RES.front();
        RES.pop();

        out << t.fname << "\tme: " << t.me
            << "\tteacher: " << t.MV
            << "\t" << t.percent << "%\n";

        cout << t.fname << "\tme: " << t.me
             << "\tteacher: " << t.MV
             << "\t" << t.percent << "%\n";

        if(t.percent>20) failed++;
        else if(t.percent<0) better++;

    }
    out << "\nBetter: " << better << "/" << num
        << ":\t" << (double)(100*better)/num
        << "%\nIn 20%: " << num-better-failed << "/" << num
        << ":\t" << (double)(100*(num-better-failed))/num
        << "%;\nFailed: " << failed << "/" << num
        << ":\t" << (double)(100*failed)/num << "%.";

    cout << "\nBetter: " << better << "/" << num
         << ":\t" << (double)(100*better)/num
         << "%\nIn 20%: " << num-better-failed << "/" << num
         << ":\t" << (double)(100*(num-better-failed))/num
         << "%;\nFailed: " << failed << "/" << num
         << ":\t" << (double)(100*failed)/num << "%.";



    out.close();
}

//
