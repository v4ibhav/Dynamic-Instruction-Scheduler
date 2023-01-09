#ifndef __SUPERSCALAR_PIPELINE__
#define __SUPERSCALAR_PIPELINE__



extern   unsigned int  rob_size, iq_size, width, CACHE_SIZE, P,  Program_Count,  number_of_cycles;
extern char* trace_file;
void func_retire();
void func_writeback();
void func_execute();
void func_issue();
void func_dispatch();
void func_regRead();
void func_rename();
void func_decode();
int  func_fetch();
bool Advance_Cycle();
bool check_space_rob();
bool check_space_iq();
bool vac_in_ROB();
bool vac_in_IQ();
bool is_Ready(  unsigned int  index);
bool progress_exc();

struct pipeline {
    unsigned long int address,transfer_dy;
    int op_type;
    int dest;
    int rn_dest,src_1,rn_src_1,src_2,rn_src_2,oDSR,oFU;
    int FE,DE,RN,RR,DI,IS,EX,WB,RT1,RT2;
    bool valid;
} ;

struct ROB {
    bool valid,ready;
    unsigned int  transfer_dy,address;
    int dest;
    pipeline instruction;
} ;

struct RMT {
    bool valid;
    int rob_tag;
} ;

struct IQ {
    bool valid;
    pipeline instruction;
} ;

struct EXECUTE {
    pipeline instruction;
    int age;
} ;

#endif

