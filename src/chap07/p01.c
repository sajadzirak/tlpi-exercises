#include "tlpi_hdr.h"
#define MAX_ALLOCS 1000000

/*

Initial program break:      0x5620eca8f000
Allocating 20*2048 bytes
Program break after allocating block number 0: 0x5620eca8f000
Program break after allocating block number 1: 0x5620eca8f000
Program break after allocating block number 2: 0x5620eca8f000
Program break after allocating block number 3: 0x5620eca8f000
Program break after allocating block number 4: 0x5620eca8f000
Program break after allocating block number 5: 0x5620eca8f000
Program break after allocating block number 6: 0x5620eca8f000
Program break after allocating block number 7: 0x5620eca8f000
Program break after allocating block number 8: 0x5620eca8f000
Program break after allocating block number 9: 0x5620eca8f000
Program break after allocating block number 10: 0x5620eca8f000
Program break after allocating block number 11: 0x5620eca8f000
Program break after allocating block number 12: 0x5620eca8f000
Program break after allocating block number 13: 0x5620eca8f000
Program break after allocating block number 14: 0x5620eca8f000
Program break after allocating block number 15: 0x5620eca8f000
Program break after allocating block number 16: 0x5620eca8f000
Program break after allocating block number 17: 0x5620eca8f000
Program break after allocating block number 18: 0x5620eca8f000
Program break after allocating block number 19: 0x5620eca8f000
Program break is now:       0x5620eca8f000
Freeing blocks from 1 to 20 in steps of 2
After free(), program break is: 0x5620eca8f000

*/

int main(int argc, char *argv[])
{
    char *ptr[MAX_ALLOCS];
    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;
    
    printf("\n");

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-allocs block-size [step [min [max]]]\n", argv[0]);

    numAllocs = getInt(argv[1], GN_GT_0, "num-allocs");
    if (numAllocs > MAX_ALLOCS)
        cmdLineErr("num-allocs > %d\n", MAX_ALLOCS);
    
    blockSize = getInt(argv[2], GN_GT_0 | GN_ANY_BASE, "block-size");

    freeStep = (argc > 3) ? getInt(argv[3], GN_GT_0, "step") : 1;
    freeMin = (argc > 4) ? getInt(argv[4], GN_GT_0, "min") : 1;
    freeMax = (argc > 5) ? getInt(argv[5], GN_GT_0, "max") : numAllocs;
    
    if (freeMax > numAllocs)
        cmdLineErr("free-max > num-allocs\n");
    
    printf("Initial program break:      %10p\n", sbrk(0));

    printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
    
    for (j = 0; j < numAllocs; j++) {
        ptr[j] = malloc(blockSize);
        printf("Program break after allocating block number %d: %10p\n", j, sbrk(0));
        if (ptr[j] == NULL)
            errExit("malloc");
    }

    printf("Program break is now:       %10p\n", sbrk(0));

    printf("Freeing blocks from %d to %d in steps of %d\n",
                freeMin, freeMax, freeStep);
    for (j = freeMin - 1; j < freeMax; j += freeStep)
        free(ptr[j]);

    printf("After free(), program break is: %10p\n", sbrk(0));
    
    exit(EXIT_SUCCESS);
}