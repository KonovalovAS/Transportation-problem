#include "myheader.h"
#include <fstream>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <queue>

struct test{
    string fname;
    double MV;
    bool calculated;
    double me, percent;

    bool operator<(const test& r);
};

Problem p_from_input();
void increase(vector<int>& v, int n);
Solution all_options( Problem& P );
void add( vector<int>& v, const vector<int> sum, int n );
Problem problem_reader( string &name );

Solution simulated_annealing( Problem& P );

void RUN( string &ifname, string &ofname );

/*
class Tester{
public:
    Tester(string tests_list_fname);
    void Run();
    void Report(string fname);

    struct test{
        string fname;
        double MV;
        bool calculated;
        double me, percent;
    };

    vector<test> Data;
    int num_tests,
        better,
        failed;
    mutex output;

    int cur;

    Problem problem_reader( string &name );

    void run1( vector<test> D );
    void run_st( test& t );
};

*/
