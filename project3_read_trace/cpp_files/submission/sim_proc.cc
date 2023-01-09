
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <deque>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include "sim_proc.h"

using namespace std;
#define foru(i,n) for(unsigned int i=0;i<n;i++)

// These variables must be consistent throughout the simulation.
unsigned int  rob_size, iq_size, width, CACHE_SIZE, P,  Program_Count=0,  number_of_cycles;
char * trace_file;
ifstream infile;    // Input file stream

// rob, as well head and tail
  unsigned int  head=2, tail=2;
vector<ROB> rob = {};
vector<RMT> rmt(67);
int ody,oSRC1,oSRC2,oDSR;
int oFE,oDE,oRN,oRR,oDI,oIS,oEX,oWB,oRT1,oRT2,oFU;

// Issue Queue
vector<IQ> iq = {};

// Execution list
vector<EXECUTE> Execution = {};

// FE_continue indicates where we have reached the end of the sequence.
  unsigned int  FE_continue=0;

// Global pipeline registers themselves.
deque<pipeline> DE_register;
deque<pipeline> RN_register;
deque<pipeline> RR_register;
deque<pipeline> DI_register;
deque<pipeline> Writeback_store;

// Main routine
int main(int argc, char ** argv) {
    if(argc != 5) {
        cout << "Invalid amount of arguments for 'sim_ds'. Please include 6 " <<
                "parameters of the format <ROB_SIZE> <iq_size> <width> " <<
                "<CACHE_SIZE> <P> <trace_file>." << endl;
        return 1;
    }

    rob_size    = atoi(argv[1]),
    iq_size    = atoi(argv[2]), 
    width      = atoi(argv[3]), 
    trace_file =      argv[4] ;
    Program_Count   = 0,   
    number_of_cycles            = 0;   
    double ipc        = 0;  

    infile.open(trace_file);

    rob.resize(rob_size);
    iq.resize(iq_size);
 ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // The following loop just tests reading the trace and echoing it back to the screen.
    //
    // Replace this loop with the "do { } while (Advance_Cycle());" loop indicated in the Project 3 spec.
    // Note: fscanf() calls -- to obtain a fetch pipeline worth of instructions from the trace -- should be
    // inside the Fetch() function.
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    


    // Pipeline routine
    do {
        func_retire();
        func_writeback();
        func_execute();
        func_issue();
        func_dispatch();
        func_regRead();
        func_rename();
        func_decode();
        FE_continue = func_fetch();
    }
    while (Advance_Cycle());

    ipc = (double)  Program_Count / (double)  number_of_cycles;
    // foru(i,Program_Count)
    // {
        
        
    // }

    printf("# === Simulator Command =========\n");
    // printf("%s\n",trace_file);
    printf("# ./sim %s %s %s %s\n",argv[1],argv[2],argv[3],trace_file);

    printf("# === Processor Configuration ===\n"); 

            cout<<"# ROB_SIZE = "<<argv[1]<<endl;
            cout<<"# IQ_SIZE  = "<<argv[2]<<endl;
            cout<<"# WIDTH    = "<<argv[3]<<endl;
    printf("# === Simulation Results ========\n");
            cout<<"# Dynamic Instruction Count    = "<<Program_Count<<endl;
            cout<<"# Cycles                       = "<<number_of_cycles<<endl;
            printf("# Instructions Per Cycle (IPC) = %.2f",ipc);

    return 0;
}

bool Advance_Cycle() {
     number_of_cycles++;
    if(FE_continue || DE_register.size() || RN_register.size() || RR_register.size()
            || DI_register.size() || !vac_in_IQ() || Execution.size()
            || Writeback_store.size() || !vac_in_ROB()) return true;
    else return false;
}

bool vac_in_IQ() {
    bool empty = true;
    for(  unsigned int  i=0; i<iq_size; i++)
        if(iq[i].valid) empty = false;
    return empty;
}
bool vac_in_ROB() {
    bool empty = true;
    for(  unsigned int  i=0; i<rob_size; i++)
        if(rob[i].valid) empty = false;
    return empty;
}




