#include "myheader.h"

/// Initializing the problem

pt::pt(){
    id = -1;
    x = 0;
    y = 0;
    demand = 0;
}

pt::pt( int ID, double X, double Y, int D ){
    x = X;
    y = Y;
    id = ID;
    demand = D;
}

double pt::dist( pt &distance_to ){
    return sqrt( pow( y-distance_to.y ,2) + pow( x-distance_to.x ,2) );
}

Problem::Problem( int V, int c ){

    Num_points = 0;
    num_vehicles = V;
    capacity = c;

    points_data = vector<pt> (0);
}

void Problem::make_source( pt S ){
    Source = S;
}

void Problem::add_point( pt newpt ){
    Num_points++;
    points_data.push_back(newpt);
}

void Problem::show_cond(){

    cout<<"\nThe problem initialized:\n\n";
    cout<<Num_points<<" "
    <<num_vehicles<<" "
    <<capacity<<"\n";

    cout<<Source.demand<<" "
        <<Source.x<<" "<<Source.y<<"\n\n";
    for(auto p: points_data)
        cout<<p.demand<<" "<<p.x<<" "<<p.y<<"\n";
}

int Problem::Consumers_number(){
    return Num_points;
}

int Problem::Vehicles_number(){
    return num_vehicles;
}

int Problem::Capacity(){
    return capacity;
}

const pt& Problem::source(){
    return Source;
}

const pt& Problem::operator()(int index){
    if(index<Num_points)
        return points_data[index];
    else
        return Source;
}

/// Solving

Solution::Solution( Problem *P ){
    the_problem = P;
    distr = vector<sv_solution>(the_problem->Consumers_number(),
                                the_problem->source());
}

Solution::sv_solution::sv_solution(){
    goods_delivered = 0;
    cost = 0;
    path = {};
}

Solution::sv_solution::sv_solution( const pt &source ){
    goods_delivered = 0;
    cost = 0;
    path = {source,source};
}

bool Solution::option::operator<(const option &r_opt){
    return this->additional_cost < r_opt.additional_cost;
}

/*
option& Solution::sv_solution::insertion_check( sv_solution &sv_sol ){

    /// TODO

    // finding place for insertion
        // saving iterator for "emplace"-method
        // saving additional cost for such emplacement
        // return "option" with this information
}

void Solution::point_insertion( const pt &npt ){

    int num_options = 0;
    vector<option> opts(0);

    for(int i=0; i<num_vehicles; i++){
        if( npt.demand <= capacity - Solution[i].cost ){
            num_options++;
            option newopt = Solution[i].insertion_check( npt );
            newopt.sv_index = i;
            opts.push_back(newopt);
        }
    }

    /// TODO
    // Greedy choice:
        // inserting to the place with the lowest additional price

            /// LATER (for genetic algorithm):
            // *a different way to choose the place for insertion
                // the idea is just in my head
}

void Solution::calculate(){

    // insert each point from points_data into solution:
        // find optimal place among all sv_solutions
            // ( keep all options in memory )
        // insert in there

    int n = the_problem->Consumers_number();
    for(int i=0; i<n; i++)
        point_insertion( the_problem->(i) );

    // All points are inserted, the solution is kinda ready...
}
*/
