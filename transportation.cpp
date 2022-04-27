#include "myheader.h"

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

    cout<<Num_points<<" "
    <<num_vehicles<<" "
    <<capacity<<"\n";

    cout<<Source.demand<<" "
        <<Source.x<<" "<<Source.y<<"\n\n";
    for(auto p: points_data)
        cout<<p.demand<<" "<<p.x<<" "<<p.y<<"\n";
}
