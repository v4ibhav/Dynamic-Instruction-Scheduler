#include <vector>
#include <iostream>
#include "sim_ds.h"

#define foru(i,n) for(unsigned int i=0;i<n;i++)
#define itern(it,en) for(auto it = en.begin();it!=en.end();it++)
#define VALID 1
#define INVALID -1
#define NON 0
#define nonblock width*5
extern int fail;

extern   unsigned int  iq_size;
extern std::vector<IQ> iq;
extern std::vector<EXECUTE> Execution;
extern   unsigned int  width;
extern   unsigned int  IS_time;
extern   unsigned int   number_of_cycles;

using namespace std;
void func_issue(){

    foru(i,width) {
        int min=INT32_MAX;
        int min_index=-1;
        if(Execution.size() == nonblock) return;

        foru(j,iq_size) {
            if(iq[j].valid == NON) continue;
            if(iq[j].instruction.rn_src_1 != INVALID) continue;
            if(iq[j].instruction.rn_src_2 != INVALID) continue;
            if(iq[j].valid) {
                if(iq[j].instruction.FE < min && is_Ready(j)){
                    min = iq[j].instruction.FE;
                    min_index = j;
                }
            }
        }

        foru( j,iq_size) {
            if(iq[j].valid == NON) continue;
            if(iq[j].valid) {
                iq[j].instruction.IS = (iq[j].instruction.IS == -1)? number_of_cycles:iq[j].instruction.IS;

            }
        }

        // If there wasn't an oldest valid instruction, leave.
        if(min_index == INVALID) return;

        // add the instruction to the execute list.
        int TTL;
        switch (iq[min_index].instruction.op_type)
        {
        case 0:
            TTL = 1;
            break;
        case 1:
            TTL = 2;
            break;
        case 2:
            TTL = 5;
            break;
        }
        EXECUTE ex_instruction = {iq[min_index].instruction, TTL};
        Execution.push_back(ex_instruction);
        // invalidate that instruction in the iq.
        iq[min_index].valid=0;
    }
    return;
}

bool is_Ready(  unsigned int  index) {
    bool red = true;
    if(fail)
    {
        // Writeback_store.pop_back();
        Execution.erase(Execution.begin());

    }
    if(iq[index].instruction.rn_src_1 != -1 || iq[index].instruction.rn_src_2 != -1) red = false;
    return red;
}
