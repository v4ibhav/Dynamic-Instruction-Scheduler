#include <vector>
#include "sim_ds.h"
#include <iostream>
//carry on
#define foru(i,n) for(unsigned int i=0;i<n;i++)
#define itern(it,en) for(auto it = en.begin(),ti!=en.end();it++)
#define VALID 1
#define INVALID -1
#define NON 0

extern std::vector<ROB> rob;
extern std::vector<RMT> rmt;
extern   unsigned int   number_of_cycles;
extern   unsigned int   Program_Count;
extern   unsigned int  head, tail;

using namespace std;




void func_retire(){

    if(vac_in_ROB() != NON) return;

   foru(i,rob.size()) {
        if(rob[i].ready  )
        {
            rob[i].instruction.RT1 = ((rob[i].instruction.RT1 == INVALID))?number_of_cycles:rob[i].instruction.RT1;
        }
   }
foru(i,width) {
   if(rob[head].ready)
   {

            if(rob[head].valid) {
       

            rob[head].instruction.RT2 =  number_of_cycles - rob[head].instruction.RT1+1;
            cout << rob[head].instruction.transfer_dy << " "<< "fu{" << rob[head].instruction.op_type << "} ";
            cout << "src{" << rob[head].instruction.src_1 << "," << rob[head].instruction.src_2 << "} ";
            cout<< "dst{" << rob[head].instruction.dest << "} ";
            cout<< "FE{" << rob[head].instruction.FE << ","<<  "} ";
            cout<< "DE{" << rob[head].instruction.DE << ","<<  "} ";
            cout<< "RN{" << rob[head].instruction.RN << ","<<  "} ";
            cout<< "RR{" << rob[head].instruction.RR << ","<<  "} ";
            cout<< "DI{" << rob[head].instruction.DI << ","<<  "} ";
            cout<< "IS{" << rob[head].instruction.IS << ","<<  "} ";
            cout<< "EX{" << rob[head].instruction.EX << ","<<"} ";
            cout<< "WB{" << rob[head].instruction.WB << ","<<"} ";
            cout<< "RT{" << rob[head].instruction.RT1 << ",";
            cout<< rob[head].instruction.RT2 << "}  "<< endl;

            if(rob[head].instruction.dest != INVALID) {
                if(rob[head].instruction.rn_dest == rmt[rob[head].instruction.dest].rob_tag) 
                {
                    rmt[rob[head].instruction.dest].valid = false;
                    rmt[rob[head].instruction.dest].rob_tag = -1;
                }
            }
            rob[head].valid = 0;
            rob[head].instruction.address = 0;
            head++;
            if(head == rob_size) head = 0;
           
        }
   }
}
    return;
}
