#include <iostream>

#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

using namespace std;

struct pt{ // point
    int id;
    double x, y;
    int demand;

    pt();
    pt( int ID, double X, double Y, int D );
    double dist( pt &distance_to ); // returns distance between two points
};

class Problem{
public:

    Problem( int V, int c );
    void make_source( pt S );
    void add_point( pt newpt );

    void Solve();

    void show_cond();

private:

    pt Source;

    int Num_points,
        num_vehicles,
        capacity;

    vector<pt> points_data;

    struct option{ // auxiliary structure
        int sv_index;
        double additional_cost;
        vector<pt>::iterator insert_here;

        bool operator<(const option &r_opt);
    };

    struct sv_solution // single-vehicle solution ("sub-solution")
    {
        sv_solution();
        sv_solution( const pt &source );

        int goods_delivered;
        double cost;
        vector<pt> path;

        option& insertion_check( const pt &npt );
    };

    friend sv_solution;
    friend option;

    double solution_cost();

    vector<sv_solution> Solution; // consumers distribution among vehicles

    void point_insertion( const pt &npt );

};
