#include "tester.h"


int main()
{
    srand(time(0));
    string ifname = "data_part.txt";
    string ofname = "0res_part.txt";

    RUN(ifname,ofname);

    return 0;
}
