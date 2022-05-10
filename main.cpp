#include "tester.h"


int main()
{
    srand(time(0));
    string ifname = "data.txt";
    string ofname = "0RES.txt";

    RUN(ifname,ofname);

    return 0;
}
