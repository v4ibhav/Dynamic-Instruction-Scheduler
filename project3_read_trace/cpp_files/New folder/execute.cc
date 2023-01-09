#include <vector>
#include <deque>
#include <iostream>
#include "sim_ds.h"

#define foru(i,n) for(unsigned int i=0;i<n;i++)
#define itern(it,en) for(auto it = en.begin();it!=en.end();it++)
#define VALID 1
#define INVALID -1
#define NON 0
#define nonblock width*5
int fail = 0;


extern std::vector<EXECUTE> Execution;
extern std::deque<pipeline> Writeback_store;
extern std::deque<pipeline> RR_register;
extern std::deque<pipeline> DI_register;
extern std::vector<IQ> iq;
extern   unsigned int  width;
extern   unsigned int   number_of_cycles;

using namespace std;

void func_execute(){

    if(Execution.size() == 0) return;
    foru(i , Execution.size()) {
        Execution[i].instruction.EX = (Execution[i].instruction.EX == INVALID)?number_of_cycles:Execution[i].instruction.EX;
        Execution[i].age--;
    }
    while(progress_exc()){
        foru( i ,Execution.size()) {
            // If it is finishing execution this cycle...
            if(Execution[i].age == NON) {
                // Wake up iq by matching rob values and setting it to ready.
                foru( j,iq_size) {
                    if(iq[j].instruction.rn_src_1 ==Execution[i].instruction.rn_dest)
                    {
                        iq[j].instruction.rn_src_1 = (iq[j].instruction.rn_src_1 != INVALID)?-1:iq[j].instruction.rn_src_1;
                    }  
                    
                    if(iq[j].instruction.rn_src_2 == Execution[i].instruction.rn_dest)
                    {
                        iq[j].instruction.rn_src_2 = (iq[j].instruction.rn_src_2 != INVALID)?-1:iq[j].instruction.rn_src_2;

                    }
                }
            
                // Wake up DI
                itern(j,DI_register) {
                    if(DI_register.size() == 0) break;
                    j->rn_src_1 = (j->rn_src_1 == Execution[i].instruction.rn_dest)?-1:j->rn_src_1;
                    j->rn_src_2 = (j->rn_src_2 == Execution[i].instruction.rn_dest)?-1:j->rn_src_2;

                }

                // Wake up RR
                itern(j,DI_register) {
                    if(RR_register.size() == 0) break;
                    
                    j->rn_src_1 = (j->rn_src_1 == Execution[i].instruction.rn_dest)?-1:j->rn_src_1;
                    j->rn_src_2 = (j->rn_src_2 == Execution[i].instruction.rn_dest)?-1:j->rn_src_2;

                }

                if(Writeback_store.size() == nonblock) {
                    return;
                }
                Execution[i].instruction.valid = true;
                pipeline instruction = Execution[i].instruction;
                Writeback_store.push_back(instruction);
                Execution.erase(Execution.begin()+i);
            }
        }
    }
    return;
}

bool progress_exc() {
    if(fail)
    {
        Writeback_store.pop_back();
        Execution.erase(Execution.begin());

    }
    bool need = false;
    for(  unsigned int  i=0; i<Execution.size(); i++) {
        need = (!Execution[i].age)?true:need;
    }
    return need;
}

