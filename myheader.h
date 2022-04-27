#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

class Problem
{
public:

    Problem();

    void Solve();

    struct pt{ // point
        int id;
        double x, y;

        static int ID;
        pt( double X, double Y );

        double operator()( pt &distance_to ); // returns distance between two points
    };

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
