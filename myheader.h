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

    class Solution // global solution
    {
    public:

        struct sv_solution // single-vehicle solution (subsolution)
        {
            int goods_delivered;
            double cost;
            vector<pt> path;
        };

        double cost();
        vector<sv_solution> distr; // consumers distribution among vehicles
    };

};
