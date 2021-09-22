//// This is the runtime library used to log warnings for memory flaw detection
#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <stdbool.h>

FILE *report;

struct pair {
    int8_t* address;
    int64_t size;
};

int         allocationIndex;
struct pair allocationMap[1000];