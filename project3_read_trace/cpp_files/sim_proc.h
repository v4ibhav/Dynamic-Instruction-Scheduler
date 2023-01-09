#ifndef SIM_PROC_H
#define SIM_PROC_H

typedef struct proc_params{
    unsigned long int rob_size;
    unsigned long int iq_size;
    unsigned long int width;
}proc_params;


// Put additional data structures here as per your requirement

//overall pipeline containing all the required output values;
struct OUTPUT{
    int seq_no;
    int op_type;
    int src1, src2;
    int dest;
    int FE,DE,RN,RR,DI,IS,EX,WB,RT1,RT2;
};

//pipeline stage structs
struct DECODE{
    int op_type;
    int dest;
    int src1_tag,src1_value, src2_tag, src2_value;
    int src1_ROB, src2_ROB;
    int src1,src2;
    
    int src1_rdy_flag;
    int src2_rdy_flag;

    int dur;

    int busy;
    int valid;


};
struct RENAME{
    int op_type;
    int dest;
    
    int src1,src2;
    int src1_tag,src1_value, src2_tag, src2_value;
    int src1_ROB, src2_ROB;

    int src1_rdy_flag;
    int src2_rdy_flag;
    
    int dest_tag;
    int dur;
    int busy;
    int valid;
};
struct Register_Read{

    int op_type;
    int dest;
    
    int src1_tag,src1_value, src2_tag, src2_value;
    int src1_ROB, src2_ROB;
    
    int src1_rdy_flag;
    int src2_rdy_flag;

    int dest_tag;
    int dur;
    int busy;
    int valid;

};
struct DISPATCH{
    int op_type;
    int dest;
    
    int src1_tag,src1_value, src2_tag, src2_value;
    int src1_ROB, src2_ROB;
    
    int src1_rdy_flag;
    int src2_rdy_flag;

    int dest_tag;
    int dur;
    int busy;
    int valid;



};
// struct Issue{

// };
struct EXECUTE{
    int op_type;
    int dest;
    
    int src1_tag,src1_value, src2_tag, src2_value;
    int src1_ROB, src2_ROB;
    
    int src1_rdy_flag;
    int src2_rdy_flag;
    int src1,src2;

    int dest_tag;
    int dur;
    int busy;
    int valid;

    int latency;



};
struct WRITEBACK{
    int op_type;
    int dest;
    
    int src1_tag,src1_value, src2_tag, src2_value;
    int src1_ROB, src2_ROB;
    
    int src1_rdy_flag;
    int src2_rdy_flag;
    int src1,src2;

    int dest_tag;
    int dur;
    int busy;
    int valid;



};

//additional data struct for superscalar pipeline
//1. Issue queue
struct Issue_queue{
    int op_type;
    int dest;
    
    int src1_tag,src1_value, src2_tag, src2_value;
    int src1_ROB, src2_ROB;
    
    int src1_rdy_flag;
    int src2_rdy_flag;

    int dest_tag;
    int dur;
    int busy;
    int valid;
};
//2. ROB
struct ROB{
    int value;
    int dest;
    int rdy;
    int exc;
    int mis;
    int pc;
    int dur; // time measurement
    int valid;
    int busy;
    ROB* head;
    ROB* tail;
    // int valid;


};


//3. Rename Table
struct rename_table{
    int valid;
    int rob_tag;
};

//function declarations
void func_Retire(rename_table[],ROB[]);
void func_Writeback(WRITEBACK[],ROB[]);
void func_Execute(EXECUTE[] ,Issue_queue[], ROB[] , WRITEBACK[] , DISPATCH[] , Register_Read[] );
void func_Issue(EXECUTE[],rename_table[],Issue_queue[],ROB[]);
void func_Dispatch(DISPATCH[],Issue_queue[],rename_table[],ROB[]);
void func_RegRead(Register_Read[],DISPATCH[],RENAME[],rename_table[],ROB[]);
void func_Rename(RENAME[], rename_table[], Register_Read[],ROB[]);
void func_Decode(DECODE[],RENAME[]);
void func_Fetch(DECODE[]);
bool func_Advance_Cycle(DECODE[],RENAME[],Register_Read[],DISPATCH[], EXECUTE[], WRITEBACK[], Issue_queue[],ROB[]);

#endif

//fetch stage
// Do nothing if either (1) there are no
// more instructions in the trace file or
// (2) DE is not empty (cannot accept a new
// decode bundle).
//
// If there are more instructions in the
// trace file and if DE is empty (can accept
// a new decode bundle), then fetch up to
// WIDTH instructions from the trace file
// into DE. Fewer than WIDTH instructions
// will be fetched only if the trace file
// has fewer than WIDTH instructions left.



// while (Advance_Cycle());
// // Advance_Cycle performs several functions.
// First, it advances the simulator cycle.
// Second, when it becomes known that the
// pipeline is empty AND the trace is depleted,
// the function returns “false” to terminate
// the loop.


// Decode(); 
// If DE contains a decode bundle:
// If RN is not empty (cannot accept a new
// rename bundle), then do nothing.
// If RN is empty (can accept a new rename
// bundle), then advance the decode bundle
// from DE to RN.

// Rename();
 // If RN contains a rename bundle:
// If either RR is not empty (cannot accept
// a new register-read bundle) or the ROB
// does not have enough free entries to
// accept the entire rename bundle, then do
// nothing.
// If RR is empty (can accept a new
// register-read bundle) and the ROB has
// enough free entries to accept the entire
// rename bundle, then process (see below)
// the rename bundle and advance it from
// RN to RR.
//
// Apply your learning from the class
// lectures/notes on the steps for renaming:
// (1) allocate an entry in the ROB for the
// instruction, (2) rename its source
// registers, and (3) rename its destination
// register (if it has one). Note that the
// rename bundle must be renamed in program
// order (fortunately the instructions in
// the rename bundle are in program order).

// RegRead();
 // If RR contains a register-read bundle:
// If DI is not empty (cannot accept a
// new dispatch bundle), then do nothing.
// If DI is empty (can accept a new dispatch
// bundle), then process (see below) the
// register-read bundle and advance it from
// RR to DI.
//
// Since values are not explicitly modeled,
// the sole purpose of the Register Read
// stage is to ascertain the readiness of
// the renamed source operands. Apply your
// learning from the class lectures/notes on
// this topic.
//
// Also take care that producers in their
// last cycle of execution wakeup dependent
// operands not just in the IQ, but also in
// two other stages including RegRead()
// (this is required to avoid deadlock). See
// Execute() description above.

//how do i check dependency
/*
    *   if the most recent version of src1&2 is inside the rt table then
    *   
    * 
*/

//Dispatch();
 // If DI contains a dispatch bundle:
// If the number of free IQ entries is less
// than the size of the dispatch bundle in
// DI, then do nothing. If the number of
// free IQ entries is greater than or equal
// to the size of the dispatch bundle in DI,
// then dispatch all instructions from DI to
// the IQ.'


//// Issue up to WIDTH oldest instructions
// from the IQ. (One approach to implement
// oldest-first issuing, is to make multiple
// passes through the IQ, each time finding
// the next oldest ready instruction and
// then issuing it. One way to annotate the
// age of an instruction is to assign an
// incrementing sequence number to each
// instruction as it is fetched from the
// trace file.)