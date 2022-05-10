#include "tester.h"


int main()
{
    set_rand();

    string ifname = "data1.txt";
    string ofname = "0res1.txt";

    RUN(ifname,ofname);

    return 0;
}
