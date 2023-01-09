#include <deque>
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

extern std::deque<pipeline> RR_register;
extern std::deque<pipeline> DI_register;
extern std::vector<ROB> rob;
extern   unsigned int  head, tail;
extern std::vector<RMT> rmt;
extern   unsigned int  width;
extern   unsigned int  RR_time;
extern   unsigned int   number_of_cycles;
using namespace std;

void func_regRead(){
    foru( i,width) {

        // set first cycle and duration of this instruction.
        itern(it,RR_register) {
            it->RR = (RR_register.size() && it->RR == -1)? number_of_cycles:it->RR;
        }
        unsigned int tem = DI_register.size();
        if(tem == width) {
            return;
        }
        else if(RR_register.size() != NON) {
            if(RR_register.front().rn_src_1 != -1) {
                RR_register.front().rn_src_1 =(rob[RR_register.front().rn_src_1].ready)? -1 :RR_register.front().rn_src_1;
               
            }
            if(RR_register.front().rn_src_2 != -1) {
                RR_register.front().rn_src_2 =(rob[RR_register.front().rn_src_2].ready)? -1 :RR_register.front().rn_src_2;

            }
            itern(it,RR_register)
            {
                it->RR = (RR_register.size() && it->RR == -1)? number_of_cycles:it->RR;

            }

            // Finally, move this pipeline to the DI register.
            DI_register.push_back(RR_register.front());
            RR_register.pop_front();
        }
    }
    return;
}
