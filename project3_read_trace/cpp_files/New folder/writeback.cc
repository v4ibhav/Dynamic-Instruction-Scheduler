#include <deque>
#include <vector>
#include "sim_ds.h"
#include <iostream>

#define foru(i,n) for(unsigned int i=0;i<n;i++)
#define itern(it,en) for(auto it = en.begin();it!=en.end();it++)
#define VALID 1
#define INVALID -1
#define NON 0


extern std::deque<pipeline> Writeback_store;
extern std::vector<ROB> rob;
extern   unsigned int  rob_size;

using namespace std;

void func_writeback(){
    while(Writeback_store.empty() == NON) {
        itern(it,Writeback_store) {
            if(Writeback_store.size() != NON)
            {
                Writeback_store.front().WB = (Writeback_store.front().WB == INVALID)?number_of_cycles:Writeback_store.front().WB;
            } 
        }

        // Mark the instruction as ready within the rob.
        foru( j , rob_size) {
            if(rob[j].valid) {
                if (rob[j].instruction.transfer_dy == Writeback_store.front().transfer_dy) {
                    rob[j].ready = true;
                    itern(it,Writeback_store) {
                        if(Writeback_store.size() != NON)
                        {
                            Writeback_store.front().WB = (Writeback_store.front().WB == INVALID)?number_of_cycles:Writeback_store.front().WB;
                        } 
                        
                    }
                    rob[j].instruction = Writeback_store.front();
                    Writeback_store.pop_front();
                    if(Writeback_store.size() == NON) break;
                }
            }
        }
    }
    return;
}
