#include <deque>
#include <vector>
#include "sim_ds.h"

#define foru(i,n) for(unsigned int i=0;i<n;i++)
#define itern(it,en) for(auto it = en.begin();it!=en.end();it++)
#define VALID 1
#define INVALID -1
#define NON 0
#define nonblock width*5
extern int fail;

extern std::deque<pipeline> DI_register;
extern std::vector<ROB> rob;
extern   unsigned int  iq_size;
extern std::vector<IQ> iq;
extern   unsigned int   number_of_cycles;

extern   unsigned int   width;
extern   unsigned int  DI_time;

using namespace std;
void func_dispatch(){


    // set first cycle and duration of this instruction.
        itern(it,DI_register) {
            it->DI = (DI_register.size() && it->DI == -1)? number_of_cycles:it->DI;
        }

    // If there isn't enough room in the iq for the entire pipeline set, do nothing.
    if(checker(2,0) != NON) {
        return;
    }
    // Otherwise move everything to the iq.
    else if(DI_register.size() != NON) {
        while(DI_register.size()) {
            foru(j,iq_size) {
                if (iq[j].valid != NON) continue;
                if(DI_register.front().rn_src_1 != INVALID) {
                    DI_register.front().rn_src_1 = (rob[DI_register.front().rn_src_1].ready)?-1:DI_register.front().rn_src_1;
                    }
                if(DI_register.front().rn_src_2 != INVALID) {
                    DI_register.front().rn_src_2 = (rob[DI_register.front().rn_src_2].ready)?-1:DI_register.front().rn_src_2;
                    }
                

                itern(it ,DI_register) {
                    it->DI =(DI_register.size() && it->DI == -1)?  number_of_cycles: it->DI;
                }

                iq[j].valid = true;
                iq[j].instruction = DI_register.front();
                break;
            }
            if(!DI_register.size()) break;
            else DI_register.pop_front();
        }
    }

    return;
}

bool check_space_iq() {
      unsigned int  empty_slots=iq_size;
    foru(i,iq_size) {
        if(iq[i].valid != NON) empty_slots--;

    }
    if(empty_slots < width)
    {
        return true;
    }
    else return false;
}
