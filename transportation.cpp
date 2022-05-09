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

Solution::Solution( Problem *P ){
    the_problem = P;
    distr.resize(0);
    sv_solution init = sv_solution(the_problem->source());
    distr = vector<sv_solution> (the_problem->Vehicles_number(),
                                 init);
    Cost = -1;

    variator = vector<int> (the_problem->Consumers_number(),0);
}

Solution::Solution( Problem *P, vector<int> &var ){
    the_problem = P;
    distr.resize(0);
    sv_solution init = sv_solution(the_problem->source());
    distr = vector<sv_solution> (the_problem->Vehicles_number(),
                                 init);
    Cost = -1;

    variator = var;
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

Solution::option Solution::sv_solution::insertion_check( const pt &npt ){

    // finding place for insertion
        // saving iterator for "emplace"-method
        // saving additional cost for such emplacement
        // return "option" with this information

    //cout << 5 << "\n";
    int it = 0,
        here = it+1;
    double best_len = dist( path[it], npt )
                    + dist( npt, path[it+1] )
                    - dist( path[it], path[it+1] );
    it++;

    //cout << 6 << "\n";
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
    //cout << 7 << "\n";

    option newopt( best_len, here );
    //cout << 8 << "\n";
    return newopt;
}

void Solution::insertion( const pt &npt, option &opt ){

    //cout << 12 << "\n";
    distr[opt.sv_index].cost += opt.additional_cost;
    distr[opt.sv_index].goods_delivered += npt.demand;
    //cout << 13 << "\n";
    vector<pt>::iterator ins_here = next( distr[opt.sv_index].path.begin(), opt.insert_here );
    //cout << 14 << "\n";
    distr[opt.sv_index].path.emplace( ins_here, npt );
    //cout << 15 << "\n";
}

void Solution::point_insertion( const pt &npt, int k ){

    //cout << 2 << "\n";

    int num_options = 0;
    vector<option> opts(0);
    int capacity = the_problem->Capacity();

    //cout << 3 << "\n";

    int i=0;
    // for each sv_solution in distr: trying to insert
    for(auto sv_sol: distr){
        if( npt.demand <= capacity - sv_sol.goods_delivered ){
            num_options++;
            //cout << 4 << "\n";
            option newopt = sv_sol.insertion_check( npt );
            //cout << 9 << "\n";
            newopt.sv_index = i;
            opts.push_back(newopt);
            //cout << 10 << "\n";
        }
        i++;
    }

    /// ( TODO: )
        // random option choice for genetic algorithm

    /// Greedy choice:
        // inserting to the place with the lowest additional price

    //auto best_opt = *( min_element(opts.begin(),opts.end()) );

    // sort( opts.begin(), opts.end() );
    // auto best_opt = opts[ ( 3 * npt.id ) % opts.size() ];
    //auto best_opt = opts[ abs(rand())%opts.size() ];

    //cout << 11 << "\n";
    // cout << "\t\t" << k << "/" << variator.size() << "\n";
    int index = variator[k],
        os = opts.size();
    // int index = (variator[k])%(opts.size());
    // cout << "\t" << index << " " << index%os << "\n";
    auto best_opt = opts[ index%(opts.size()) ];
    insertion( npt, best_opt );

    //cout << 16 << "\n";

    //cout << "\tNum of options: " << opts.size() << "\n";
}

void Solution::upd_cost(){
    Cost = 0;
    for(auto svsol: distr)
        Cost += svsol.cost;
}

void Solution::calculate(){

    // insert each point from points_data into solution:
        // find optimal place among all sv_solutions
            // ( keep all options in memory )
        // insert in there

    //cout << 1 << "\n";

    int n = the_problem->Consumers_number();
    for(int i=0; i<n; i++){
        auto ins = (*the_problem)(i);
        point_insertion( ins, i );
    }
    //cout << 17 << "\n";

    //cout << "almost done\n";
    upd_cost();
    //cout << 18 << "\n";

    //cout << "done\n";

    // All points are inserted, the solution is kinda ready...
}

/*
Solution::~Solution(){
    if(the_problem!=nullptr)
        delete the_problem;
}
*/

//
