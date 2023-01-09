#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "sim_ds.h"



#define foru(i,n) for(unsigned int i=0;i<n;i++)
#define itern(it,en) for(auto it = en.begin();it!=en.end();it++)
#define VALID 1
#define INVALID -1
#define NON 0
#define nonblock width*5
extern int fail;

using namespace std;

extern ifstream infile;
extern   unsigned int  CACHE_SIZE;
extern   unsigned int   number_of_cycles;
extern deque<pipeline> DE_register;

  int  func_fetch() {
    static   unsigned int  stall = 0;
    string line;                    
    string ADDRESS_OF;             
    unsigned long int address;      
    int   op_type;         
    int   dest,src_1,src_2;
    foru(i,width) {
        if(CACHE_SIZE == 0) {
            if(DE_register.size() == width) {
                stall++;
                return 1;
            }
            getline(infile, line);
            istringstream iss(line);
            if(!(iss >> ADDRESS_OF >> op_type >> dest >> src_1 >> src_2))
                return 0;
        }
        ADDRESS_OF = "0x" + ADDRESS_OF;
        sscanf(ADDRESS_OF.c_str(), "%016lx", &address);
        pipeline instruction;
        instruction.address = address;
        instruction.op_type = op_type;
        instruction.dest    = dest;
        instruction.rn_dest = -1;
        instruction.src_1   = src_1;
        instruction.src_2   = src_2;
        instruction.valid = false;
        instruction.FE =  number_of_cycles;
        instruction.DE=instruction.RN=instruction.RR=instruction.RT1 =instruction.DI=instruction.IS=instruction.EX=instruction.WB=instruction.rn_dest = instruction.rn_src_1 = instruction.rn_src_2 =  -1;
        instruction.transfer_dy =  Program_Count;
        Program_Count+=1;
        DE_register.push_back(instruction);
    }
    stall=0;
    return 1;
}
