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

double dist( pt a, pt b ){
    return sqrt( pow( a.x - b.x, 2 ) + pow( a.y - b.y, 2 ) );
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
    distr.resize(0);
    sv_solution init = sv_solution(the_problem->source());
    distr = vector<sv_solution> (the_problem->Vehicles_number(),
                                 init);
}

void Solution::show(){
    cout<<"\n";

    for(auto svsol: distr)
        svsol.show();

    cout<<"\n";
}

void Solution::sv_solution::show(){
    cout << "gd: " << goods_delivered << ", cost: " << cost << "\n\t";
    for(auto p: path)
        cout << p.id << ": ( " << p.x << ", " << p.y << " ); ";
    cout << "\n";
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

Solution::option::option(double add_cost, int ins_here){
    insert_here = ins_here;
    additional_cost = add_cost;
}

Solution::option::option(){
    additional_cost = 0;
}

bool Solution::option::operator<(const option &r_opt){
    return this->additional_cost < r_opt.additional_cost;
}

Solution::option Solution::sv_solution::insertion_check( const pt &npt ){

    // finding place for insertion
        // saving iterator for "emplace"-method
        // saving additional cost for such emplacement
        // return "option" with this information

    int it = 0,
        here = it+1;
    double best_len = dist( path[it], npt )
                    + dist( npt, path[it+1] )
                    - dist( path[it], path[it+1] );
    it++;

    int num_of_nodes = path.size();
    for(int i=0; i<num_of_nodes-2; i++){
        double tmp = dist( path[it], npt )
                   + dist( npt, path[it+1] )
                   - dist( path[it], path[it+1] );
        if( tmp < best_len ){
            best_len = tmp;
            here = it+1;
        }
        it++;
    }

    option newopt( best_len, here );
    return newopt;
}

void Solution::insertion( const pt &npt, option &opt ){

    distr[opt.sv_index].cost += opt.additional_cost;
    distr[opt.sv_index].goods_delivered += npt.demand;
    vector<pt>::iterator ins_here = next( distr[opt.sv_index].path.begin(), opt.insert_here );
    distr[opt.sv_index].path.emplace( ins_here, npt );
}

void Solution::point_insertion( const pt &npt ){

    int num_options = 0;
    vector<option> opts(0);
    int capacity = the_problem->Capacity();

    int i=0;
    // for each sv_solution in distr: trying to insert
    for(auto sv_sol: distr){
        if( npt.demand <= capacity - sv_sol.goods_delivered ){
            num_options++;
            option newopt = sv_sol.insertion_check( npt );
            newopt.sv_index = i;
            opts.push_back(newopt);
        }
        i++;
    }

    // Greedy choice:
        // inserting to the place with the lowest additional price

    auto best_opt = *( min_element(opts.begin(),opts.end()) );
    insertion( npt, best_opt );

    /// TODO:
        // random option choice for genetic algorithm
}

void Solution::calculate(){

    // insert each point from points_data into solution:
        // find optimal place among all sv_solutions
            // ( keep all options in memory )
        // insert in there

        cout << "Before:";
        show();



    int n = the_problem->Consumers_number();
    for(int i=0; i<n; i++){
        auto ins = (*the_problem)(i);
        point_insertion( ins );
        cout << "ID: " << ins.id << "\n";
        show();
    }


        cout << "after:";
        show();

    // All points are inserted, the solution is kinda ready...
}

Solution::~Solution(){
    if(the_problem!=nullptr)
        delete the_problem;
}

//
