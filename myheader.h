#include <iostream>

#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

#include <ctime>
#include <random>
#include <limits>

using namespace std;

struct pt{ // point
    int id;
    double x, y;
    int demand;

    pt();
    pt( int ID, double X, double Y, int D );
    double dist( pt &distance_to ); // returns distance between two points

    bool operator<(const pt &right);
    pt& operator=(const pt &right);
};

double dist( pt a, pt b );

class Problem{
public:

    Problem( int V, int c );
    void make_source( pt S );
    void add_point( pt newpt );
    void show_cond();

    int Consumers_number();
    int Vehicles_number();
    int Capacity();

    const pt& source();
    const pt& operator()(int index);

    void permute();

private:

    pt Source;

    int Num_points,
        num_vehicles,
        capacity;

    vector<pt> points_data;
};

class Solution{
public:
    Solution( Problem *P );
    Solution( Problem *P, vector<int> &var );
    void calculate();
    double Cost;

    void show();

    bool exists;

    //~Solution();

private:

    Problem * the_problem;

    struct option{ // auxiliary structure
        int sv_index;
        double additional_cost;
        int insert_here;

        option();
        option(double add_cost, int ins_here);

        bool operator<(const option &r_opt);
        option& operator=(const option r_opt);
    };

    struct sv_solution{ // single-vehicle solution ("sub-solution");
        sv_solution();
        sv_solution( const pt &source );

        int goods_delivered;
        double cost;
        vector<pt> path;

        option insertion_check( const pt &npt );

        void show();
    };

    vector<sv_solution> distr; // consumers distribution among vehicles
    double solution_cost();
    void point_insertion( const pt &npt, int k );
    void insertion( const pt &npt, option &opt );

    vector<int> variator;
    void upd_cost();
};

//
