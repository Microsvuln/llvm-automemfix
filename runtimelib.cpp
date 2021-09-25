//// This is the runtime library used to log warnings for memory flaw detection
//// Describe why we need to add a runtime library to our LLVM implementation for our LLVM Pass to work well
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

// Log stack allocations to the global map
void logAlloca(int32_t* address) {
    int8_t* castAddress = (int8_t *)address;
    struct pair newAllocation = {castAddress, 0};
    allocationMap[allocationIndex] = newAllocation;
    allocationIndex++;
}

void logMalloc(int8_t* address, int64_t size, int64_t line, int64_t col){
    struct pair newAllocation = {address, size};
    allocationMap[allocationIndex] = newAllocation;
    allocationIndex++;
    fprintf(report, "log %ld %ld Malloc %p\n", line, col, address);
}

void logFree(int8_t* address, int64_t line, int64_t col){
    struct pair dummyAllocation = {0 ,0};
    for (int i = 0; i < 1000; ++i){
        if(!comparePairs(allocationMap[i], dummyAllocation) && allocationMap[i].address == address){
            allocationMap[i] = dummyAllocation;
            fprintf(report, "log %ld %ld Free %p\n", line, col, address);
            return ;
        }
    }
    if(line > -1){
        printf("Line %ld.%ld: Warning! Attempted double free!\n", line, col);
        fprintf(report, "Line %ld.%ld: Warning! Attempted Double free!\n", line, col);
    }else{
        printf("Line ?: Warning! Attempted Double Free!\n");
        fprintf(report, "Line ?: Warning! Attempted Double Free!\n");
    }
   
}

void LogQuery(int32_t* address, int64_t size, int64_t line, int64_t col){
    struct pair dummyAllocation = {0, 0};
    int8_t* castAddress = (int8_t *) address;
    int8_t* startAddress = castAddress;
    int8_t* endAddress = castAddress + size;
    int flag = 0;

    for (int i = 0; i < 1000; ++i)
    {
        if(!comparePairs(allocationMap[i], dummyAllocation)){
            int8_t* allocStartAddress = allocationMap[i].address;
            int8_t* allocEndAddress = allocationMap[i].address + allocationMap[i].size;
        }
        if(allocationMap[i].size == 0 && allocStartAddress == startAddress){
            flag = 2;
            break;
        }
        if(allocStartAddress <= startAddress && allocEndAddress >= startAddress){
            flag+= 1;
            break;
        }
    }
    for (int i = 0; i < 1000; ++i)
    {
        if(flag == 2){
            break;
        }
    }

    if(!comparePairs(allocationMap[i], dummyAllocation)){
        int8_t allocStartAddress = allocationMap[i].address;
        int8_t allocEndAddress   = allocationMap[i].address + allocationMap[i].size;
    }
    
    
}