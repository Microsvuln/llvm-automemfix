//// This is the runtime library used to log warnings for memory flaw detection
#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <stdbool.h>

FILE *report;

struct pair {
    int8_t  *address;
    int64_t size;
};

int allocationIndex;
struct pair allocationMap[1000];

bool comparePairs(struct pair pair_one, struct pair pair_two){
    if(pair_one.address == pair_two.address && pair_one.size == pair_two.size) {
        return true;
    }
    return false;
}

void initialize() {
    allocationIndex = 0;
    struct pair dummyAllocation = {0, 0};
    for (int i = 0; i < 1000; i++){
        allocationMap[i] = dummyAllocation;
    }
    report = fopen("report.txt", "w");    
    
}