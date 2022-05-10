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

bool pt::operator<(const pt &right){
    return ( this->id < right.id );
}

pt& pt::operator=(const pt &right){
    this->x = right.x;
    this->y = right.y;
    this->id = right.id;
    this->demand = right.demand;
    return *this;
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
        cout<<p.id<<") "<<p.demand<<" "<<p.x<<" "<<p.y<<"\n";
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

void Problem::permute(){
    next_permutation( points_data.begin(), points_data.end() );
}

/// Solving

Solution::Solution( Problem *P, bool greedy_flag ){
    the_problem = P;
    distr.resize(0);
    sv_solution init = sv_solution(the_problem->source());
    distr = vector<sv_solution> (the_problem->Vehicles_number(),
                                 init);
    Cost = -1;

    variator = vector<int> (the_problem->Consumers_number(),0);
    exists = true;
    greedy = greedy_flag;
}

Solution::Solution( Problem *P, vector<int> &var ){
    the_problem = P;
    distr.resize(0);
    sv_solution init = sv_solution(the_problem->source());
    distr = vector<sv_solution> (the_problem->Vehicles_number(),
                                 init);
    Cost = -1;

    variator = var;
    exists = true;
    greedy = false;
}

vector<int> Solution::Variator(){
    return variator;
}

void Solution::show(){
    cout<<"\n";

    cout << Cost << " 0\n";
    for(auto svsol: distr)
        svsol.show();

    cout<<"\n";
}

void Solution::sv_solution::show(){
    //cout << "gd: " << goods_delivered << ", cost: " << cost << "\n\t";
    for(auto p: path)
        cout << p.id << " ";
        //cout << p.id << ": ( " << p.x << ", " << p.y << " ); ";
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
    return ( this->additional_cost < r_opt.additional_cost );
}

Solution::option& Solution::option::operator=(const option r_opt){
    this->sv_index = r_opt.sv_index;
    this->insert_here = r_opt.insert_here;
    this->additional_cost = r_opt.additional_cost;
    return *this;
}

void Solution::insertion( const pt &npt, option &opt ){
    distr[opt.sv_index].cost += opt.additional_cost;
    distr[opt.sv_index].goods_delivered += npt.demand;
    vector<pt>::iterator ins_here = next( distr[opt.sv_index].path.begin(), opt.insert_here );
    distr[opt.sv_index].path.emplace( ins_here, npt );
}

void Solution::point_insertion( const pt &npt, int k ){

    vector<option> opts(0);
    int capacity = the_problem->Capacity();

    int i=0;
    for(auto s: distr){
        if( npt.demand <= capacity - s.goods_delivered ){
            int n = (s.path).size();
            for(int j=0; j<n-1; j++){
                double add_cost = dist( s.path[j], npt )
                                + dist( npt, s.path[j+1] )
                                - dist( s.path[j], s.path[j+1] );
                option newopt( add_cost, j+1 );
                newopt.sv_index = i;
                opts.push_back(newopt);
            }
        }
        i++;
    }

    sort( opts.begin(), opts.end() );

    int os = opts.size();
    if( os>0 ){
        int index = variator[k]%os;
        auto best_opt = opts[ index ];

        if(greedy){
            // inserting to the place with the lowest additional price
            auto choice = min_element(opts.begin(),opts.end());
            best_opt = *choice;
            variator[k] = distance(opts.begin(),choice);
        }

        insertion( npt, best_opt );
    }
    else{
        exists = false;
    }
}

void Solution::upd_cost(){
    Cost = 0;
    for(auto svsol: distr)
        Cost += svsol.cost;
}

void Solution::calculate(){

    int n = the_problem->Consumers_number();
    for(int i=0; i<n && exists; i++){
        auto ins = (*the_problem)(i);
        point_insertion( ins, i );
    }

    if(exists)
        upd_cost();
    else{
        Cost = -1;
        distr.resize(0);
    }
}

//
