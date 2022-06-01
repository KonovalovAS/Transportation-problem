#include "tester.h"


int main()
{
    set_rand();

    string ifname = "data.txt";
    string ofname = "_partial_result_017.txt";

    RUN(ifname,ofname);

    return 0;
}
