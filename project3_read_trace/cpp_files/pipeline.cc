#include <deque>
#include <vector>
#include <iostream>
#include "sim_proc.h"
#include <cstdlib>
#include <fstream>
#include <sstream>



#define foru(i,n) for(unsigned int i=0;i<n;i++)
#define itern(it,en) for(auto it = en.begin();it!=en.end();it++)
#define VALID 1
#define INVALID -1
#define NON 0
#define nonblock width*5

using namespace std;

exTend vector<IQ> iq;

int fail = 0;

exTend int ody,oFU,oFE,oSRC1,oSRC2,oDSR,oFE,oDE,oRN,oRR,oDI,oIS,oEX,oWB,oRT1,oRT2;


exTend deque<pipeline> Writeback_store, DE_register, DI_register, RN_register, RR_register;

exTend   unsigned int  head, tail;
exTend vector<RMT> rmt;
exTend vector<ROB> rob;

exTend   unsigned int  width, RN_time;


exTend vector<EXECUTE> Execution;
exTend   unsigned int   number_of_cycles, Program_Count;

using namespace std;

void counter()
{
    foru(i,10);
}

void printing(unsigned int head)
{
    if(rob[head].valid != NON)
    {
        cout << ody << " "<< "fu{" << oFU<< "} ";
        cout << "src{" << oSRC1 << "," <<oSRC2 << "} ";
        cout<< "dst{" <<oDSR << "} ";
        cout<< "FE{" << oFE << ","<< oDE-oFE <<"} ";
        cout<< "DE{" << oDE << ","<< oRN-oDE <<"} ";
        cout<< "RN{" << oRN << ","<< oRR-oRN <<"} ";
        cout<< "RR{" << oRR << ","<< oDI-oRR    <<"} ";
        cout<< "DI{" << oDI << ","<< oIS-oDI<<"} ";
        cout<< "IS{" << oIS << ","<< oEX-oIS<<"} ";
        cout<< "EX{" << oEX << ","<< oWB-oEX<<"} ";
        cout<< "WB{" << oWB << ","<< oRT1-oWB<<"} ";
        cout<< "RT{" << oRT1 << ",";
        cout<< oRT2 << "}  "<< endl;
    }
}
bool checker(int t,unsigned int ind)
{
    unsigned int  empty_slots=rob_size;
    unsigned int  empty_iq=iq_size;

    bool red = true;
    unsigned int index;


    switch (t)
    {
    case 0:
        /* code */
        {
                foru(i,rob.size()) {
                if(rob[i].valid) empty_slots--;
            }
            if(empty_slots<width)
            {
                return true;
            }
            else{
                return false;
            }

        }

        break;
    case 1:
        {
                bool red = true;
        if(fail)
        {
            
            Execution.erase(Execution.begin());

        }
        if(iq[ind].instruction.rn_src_1 != -1 || iq[ind].instruction.rn_src_2 != -1) red = false;
        return red;
        }

    case 2:
    {
        foru(i,iq_size) 
        {
            if(iq[i].valid != NON) empty_iq--;
        }
        if(empty_iq < width)
        {
            return true;
        }
        else return false;
    }
        break;
    
    default:
        break;
    }
}

void func_rename(){
    itern(j,RN_register)
    {
            j->RN = (RN_register.size() && (j->RN == -1))? number_of_cycles:j->RN ;

    }
    if((RR_register.size() == width) || checker(NON,0) != NON) {
                return;
            }

    foru(i,width) {
        if(RN_register.size() != NON) {

            rob[tail].valid = true;
            int addre_front = RN_register.front().address;
            rob[tail].address = addre_front;
            rob[tail].transfer_dy = RN_register.front().transfer_dy;
            rob[tail].ready = false;
            rob[tail].instruction = RN_register.front();



            if(RN_register.front().src_1 != INVALID) {
                RN_register.front().rn_src_1 =(rmt[RN_register.front().src_1].valid)? rmt[RN_register.front().src_1].rob_tag:RN_register.front().rn_src_1;
            }

            if(RN_register.front().src_2 != INVALID) {
                RN_register.front().rn_src_2 =(rmt[RN_register.front().src_2].valid)? rmt[RN_register.front().src_2].rob_tag:RN_register.front().rn_src_2;
                
                
                
            }

            rob[tail].dest = RN_register.front().dest;
            if(RN_register.front().dest != INVALID) {
                rmt[RN_register.front().dest].valid = true;
                rmt[RN_register.front().dest].rob_tag = tail;
            }
            RN_register.front().rn_dest = tail;

            tail+=1;
            tail = (tail == rob_size)?0:tail;

            RR_register.push_back(RN_register.front());
            RN_register.pop_front();
        }
    }
    return;
}

void func_retire(){

    if(checker2(1) != NON) return;

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
            ody        =  rob[head].instruction.transfer_dy;
            oFU    =    rob[head].instruction.op_type;
            oFE    =rob[head].instruction.FE;
            oSRC1    =rob[head].instruction.src_1;
            oSRC2    =rob[head].instruction.src_2;
            oFE    =rob[head].instruction.FE;
            oDE    =rob[head].instruction.DE;
            oRN    =rob[head].instruction.RN;
            oRR    =rob[head].instruction.RR;
            oDI    =rob[head].instruction.DI;
            oIS    =rob[head].instruction.IS;
            oEX    =rob[head].instruction.EX;
            oWB    =rob[head].instruction.WB;
            oRT1    =rob[head].instruction.RT1;
            oRT2 = rob[head].instruction.RT2;
            oDSR = rob[head].instruction.dest;
            printing(head);
            
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

void func_writeback(){
    while(Writeback_store.empty() == NON) {
        itern(it,Writeback_store) {
            if(Writeback_store.size() != NON)
            {
                Writeback_store.front().WB = (Writeback_store.front().WB == INVALID)?number_of_cycles:Writeback_store.front().WB;
            } 
        }

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

void func_execute(){

    if(Execution.size() == 0) return;
    foru(i , Execution.size()) {
        Execution[i].instruction.EX = (Execution[i].instruction.EX == INVALID)?number_of_cycles:Execution[i].instruction.EX;
        Execution[i].age--;
    }
    while(progress_exc()){
        foru( i ,Execution.size()) {
            if(Execution[i].age == NON) {
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
            
                itern(j,DI_register) {
                    if(DI_register.size() == 0) break;
                    j->rn_src_1 = (j->rn_src_1 == Execution[i].instruction.rn_dest)?-1:j->rn_src_1;
                    j->rn_src_2 = (j->rn_src_2 == Execution[i].instruction.rn_dest)?-1:j->rn_src_2;

                }

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
                if(iq[j].instruction.FE < min && checker(VALID,j)){
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

        if(min_index == INVALID) return;

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
        iq[min_index].valid=0;
    }
    return;
}

void func_dispatch(){


        itern(it,DI_register) {
            it->DI = (DI_register.size() && it->DI == -1)? number_of_cycles:it->DI;
        }

    if(checker(2,0) != NON) {
        return;
    }
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
            counter();
            counter();
        }
    }

    return;
}

void func_regRead(){
    foru( i,width) {

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
            counter();
            DI_register.push_back(RR_register.front());
            counter();
            RR_register.pop_front();
        }
    }
    return;
}

void func_decode(){
    itern(j,DE_register)
    {
        j->DE = (DE_register.size() && (j->DE == -1))? number_of_cycles:j->DE;

    }
    foru(i,width)
    {
        if(RN_register.size() == width) {
            return;
        }
        else if(DE_register.size() != NON) {
            counter();
            RN_register.push_back(DE_register.front());
            counter();
            DE_register.pop_front();
            counter();

        }
    }
    return;
}


exTend ifstream infile;


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
        pipeline BUND;
        BUND.address = address;
        BUND.op_type = op_type;
        BUND.dest    = dest;
        BUND.rn_dest = -1;
        BUND.src_1   = src_1;
        BUND.src_2   = src_2;
        BUND.valid = false;
        BUND.FE =  number_of_cycles;
        BUND.DE=BUND.RN=BUND.RR=BUND.RT1 =BUND.DI=BUND.IS=BUND.EX=BUND.WB=BUND.rn_dest = BUND.rn_src_1 = BUND.rn_src_2 =  -1;
        BUND.transfer_dy =  Program_Count;
        Program_Count+=1;
        DE_register.push_back(BUND);
    }
    stall=0;
    return 1;
}