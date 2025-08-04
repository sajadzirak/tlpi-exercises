/*
    Assume a system where the value returned by the call sysconf(_SC_CLK_TCK) is
    100. Assuming that the clock_t value returned by times() is an unsigned 32-bit
    integer, how long will it take before this value cycles so that it restarts at 0?

    since sysconf(_SC_CLK_TCK) = 100, we have 100 clock ticks in each second.
    so each clock tick will take 10ms long. since clock_t is a 32 bit value it can be 
    at most 4294967296. so 4294967296 * 10 = 42949672960 ms
    It will take 42949672 secs for it to restarts at 0.
*/

/*
    Perform the same calculation for the CLOCKS_PER_SEC value returned by clock().
*/

#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
    printf("CLOCKS_PER_SEC: %ld\n", CLOCKS_PER_SEC);
    printf("%ld\n", __UINT32_MAX__ / CLOCKS_PER_SEC);   // The answer is 4294

    return 0;
}