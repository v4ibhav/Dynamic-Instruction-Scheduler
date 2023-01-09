#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_proc.h"
#include <iostream>
#include <algorithm>
#define forn(i,n) for(int i = 0;i<n;i++)
using namespace std;


/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim 256 32 4 gcc_trace.txt
    argc = 5
    argv[0] = "sim"
    argv[1] = "256"
    argv[2] = "32"
    ... and so on
*/

//Output for the simulator
FILE *FP;               // File handler
OUTPUT Output[10000]= {};

//global Variable 
int tempK = 0;
int k;
// int temp = 0;
int Program_counter = 0;
int number_of_cycles = 0;
int output_counter= 0;
int EOF_Flag = 0;
int width;
int rob_size ;
int iq_size ;
int count_iq;
int op_type, dest, src1, src2;  // Variables are read from trace file
int HEAD, TAIL = 0;
char *trace_file;       // Variable that holds trace file name;

unsigned long int pc; // Variable holds the pc read from input file

// bool is_decoder_empty = true;

int main (int argc, char* argv[])
{
    proc_params params;       // look at sim_bp.h header file for the the definition of struct proc_params
    
    if (argc != 5)
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.rob_size     = strtoul(argv[1], NULL, 10);
    params.iq_size      = strtoul(argv[2], NULL, 10);
    params.width        = strtoul(argv[3], NULL, 10);
    trace_file          = argv[4];
    width = params.width;
    printf("rob_size:%lu "
            "iq_size:%lu "
            "width:%lu "
            "tracefile:%s\n", params.rob_size, params.iq_size, params.width, trace_file);
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        EOF_Flag = 1;
        exit(EXIT_FAILURE);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // The following loop just tests reading the trace and echoing it back to the screen.
    //
    // Replace this loop with the "do { } while (Advance_Cycle());" loop indicated in the Project 3 spec.
    // Note: fscanf() calls -- to obtain a fetch bundle worth of instructions from the trace -- should be
    // inside the Fetch() function.
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // int prog_C = 0;
    // while(fscanf(FP, "%lx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2) != EOF)
    // {
    //     cout<<prog_C<<"\t";
    //     printf("%lx %d %d %d %d\n", pc, op_type, dest, src1, src2);
    //     prog_C++;
    // }
        //Print to check if inputs have been read correctly
    
//Creating and initializing all the structs;


    int Nonblockers = params.width*5;
    DECODE Decode[params.width] = {};
    RENAME Rename[params.width] = {};
    Register_Read Reg_read[params.width] = {};
    DISPATCH dispatch[params.width] = {};
    EXECUTE execute[Nonblockers] = {};
    WRITEBACK Writeback[Nonblockers]= {};

    Issue_queue IQ[params.iq_size]= {};
    ROB rob[params.rob_size]= {};
    forn(i,rob_size)
    {
        rob[i].value = i;
    }
    // cout<<"rob valid is "<<rob[2].valid;
    rename_table RT[67]= {};
    
    //pointer to the head and tail of ROB
    // ROB *head, *tail = &rob[0];
    // rob->head = &rob[0];
    // rob->tail = &rob[0];
    // cout<<&head;    


    do
    {
        // if(fscanf(FP, "%lx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2) == EOF)
        // {
        //     EOF_Flag = 1;

        //     // cout<<"inside the fscanf";
        // }


    //     /* code*/
        func_Retire(RT,rob);
        func_Writeback(Writeback,rob);
        func_Execute(execute,IQ,rob,Writeback,dispatch,Reg_read);
        func_Issue(execute,RT,IQ,rob);
        func_Dispatch(dispatch,IQ,RT,rob);
        func_RegRead(Reg_read,dispatch,Rename,RT,rob);
        func_Rename(Rename,RT, Reg_read, rob);
        func_Decode(Decode,Rename);
        func_Fetch(Decode);


    } while (func_Advance_Cycle(Decode,Rename,Reg_read,dispatch,execute,Writeback,IQ,rob));


    //Output of Simulator
 /*
    forn(i,Program_counter)
    {
        //cout<<sequmber<<"fu{"<<op_type<<"}"<<"src{"<<src1<<","<<src2<<"}"<<"dst{"<<op_type<<"}"<<"FE{"<<src1<<","<<src2<<"}"<<"DE{"<<src1<<","<<src2<<"}"<<"RN{"<<src1<<","<<src2<<"}"<<"RR{"<<src1<<","<<src2<<"}"<<"DI{"<<src1<<","<<src2<<"}"<<"IS{"<<src1<<","<<src2<<"}"<<"EX{"<<src1<<","<<src2<<"}"<<
        // "WB{"<<src1<<","<<src2<<"}"<<"RT{"<<src1<<","<<src2<<"}"<<"\n";
    }
    printf("# === Simulator Command =========\n");
    // printf("%s\n",trace_file);
    printf("# ./sim %s %s %s %s\n",argv[1],argv[2],argv[3],trace_file);

    printf("# === Processor Configuration ===\n"); 

            cout<<"# ROB_SIZE = "<<argv[1]<<endl;
            cout<<"# IQ_SIZE  = "<<argv[2]<<endl;
            cout<<"# WIDTH    = "<<argv[3]<<endl;
    printf("# === Simulator Results =========\n");
            cout<<"# Dynamic Instruction Count    = "<<endl;
            cout<<"# Cycles                       = "<<endl;
            cout<<"# Instructions Per Cycle (IPC) = ";

 */   
    fclose(FP);
    return 0;
}


//*******IMPORTANT*********//
/*
1. program_counter = actual instrution count-1

*/

// function definition

void func_Fetch(DECODE decode[])
{
    // cout<<"inside decode"<<endl;
    // cout<<"\nat fetch stage";

    bool is_decoder_empty = true;
    int dic = 0;
    forn(i,width)
    {
        if(decode[i].valid == 1)
        {

            is_decoder_empty = false;
        }
        if(decode[i].valid == 0)    dic++;

    }

    // fscanf(FP, "%lx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2);
    if(feof(FP) || is_decoder_empty == false)
    {
        //DO NOTHING
        // cout<<"doing nothing";
    }
    else if(!feof(FP) && dic ==width)
    {

        // cout<<"we are here"<<endl;
        //fetch width number of instruction and transfer to decode stage
        // cout<<"this is read";
        cout<<"\n"<<width;
        for(int i = 0;i<width;i++)
        {
            // cout<<"read the FP"<< endl; 
            fscanf(FP, "%lx %d %d %d %d", &pc, &decode[i].op_type, &decode[i].dest, &decode[i].src1, &decode[i].src2);
            decode[i].valid = 1;
            // cout<<"this is happening"<<endl;
            Program_counter++;
            decode[i].dur = Program_counter;

            Output[Program_counter].op_type = decode[i].op_type;
            Output[Program_counter].seq_no  = decode[i].dur;
            Output[Program_counter].src1    = decode[i].src1;
            Output[Program_counter].src2    = decode[i].src2;
            Output[Program_counter].dest    = decode[i].dest;
            Output[Program_counter].FE      = output_counter;
            Output[Program_counter].DE      = output_counter++;  
            
            if(feof(FP))
            {
                // cout<<"reached"<<endl;
                break;
            }
            
        }
        
    }
    return;
}

bool func_Advance_Cycle(DECODE decode[],RENAME rename[],Register_Read Register_Read[],DISPATCH dispatch[],EXECUTE execute[],WRITEBACK writeback[],Issue_queue IQ[],ROB rob[])
{
    // cout<<"\nat advance";
    // bool flag;
    //1st advance the simulator cycle
    Program_counter++;
    
    
    //check if the pipeline is empty or not?
    //if not empty return true else return False
    // cout<<"inside func advance";

/* ====================DEBUGGER=======================
    int count_file_size = 0;
    
    if(feof(FP))
    {
        return false;
    }
    else {
        count_file_size++;
    }
    cout<<Program_counter<<endl;
*/



    // cout<<"program is here"<<endl;
    // //no trace left
    if(feof(FP))
    {
        //check in pipeline if any instuction are there
        //check inside all the pipeline elements of size width
        forn(i,width)
        {
            // cout<<" rename valid  "<<rename[i].valid<<"\t";
            // cout<<"  reg valid  "<<Register_Read[i].valid<<"\t";
            
            if(decode[i].valid == 1 || rename[i].valid == 1 || Register_Read[i].valid == 1 || dispatch[i].valid == 1 || writeback[i].valid == 1 )
            {
            // cout<<"program at similar size checkers"<<endl;
                return true;   
            }

        }
        //check execute
        int exec_size = width*5;
        forn(i,exec_size)
        {
            cout<<"program at exec checkers"<<endl;
            if(execute[i].valid == 1)
                return true;
            
        }

        forn(i,iq_size)
        {
            cout<<"program at iq checkers"<<endl;

            if(IQ[i].valid == 1)
                return true;
        }

        forn(i,rob_size)
        {
            cout<<"program at rob checkers"<<endl;

            if(rob[i].valid == 1)
                return true;
        }
        if(!HEAD == TAIL && Program_counter!= 0)
        {
            return true;
        }
    }

    //traces left
    else{
        cout<<"program at traces left"<<endl;
        return true;
    }
    
    return false;


}


//decode stage

void func_Decode(DECODE decode[], RENAME rename[]){
    // cout<<"\nat decode";

    bool is_decoder_empty = true;
    bool is_rn_empty = true;
    forn(i,width)
    {
        if(decode[i].valid == 1)
            is_decoder_empty = false;
    }
    forn(i,width)
    {
        if(rename[i].valid == 1)
            is_rn_empty = false;

    }

    if(!is_decoder_empty && !is_rn_empty)
    {
            // DO Nothing
    }
    else if(!is_decoder_empty && is_rn_empty)
    {
        forn(i,width)
        {
            if(decode[i].valid == 1)
            {
                if(rename[i].valid == 0)
                {

                    decode[i].valid = 0;

                    //transfer
                    rename[i].op_type = decode[i].op_type;
                    rename[i].dest = decode[i].dest;
                    rename[i].src1_value = decode[i].src1;
                    rename[i].src2_value = decode[i].src2;
                    rename[i].valid = 1;
                    cout<<"set rename to valid "<<endl;

                    rename[i].dur = decode[i].dur;
                    Output[rename[i].dur].RN = output_counter++;
                }

            }
        }
    }

    return;
}

void func_Rename(RENAME rename[], rename_table rt[],Register_Read reg_read[],ROB rob[] )
{
    //check RN contains a rename bundle
    
    // cout<<"\nat rename ";

    // bool rn_contains_bundle =false;
    // bool rr_isempty = true;
    // bool rob_have_space = false;
    // int rob_space_count = 0;
    int rn_C = 0,rob_C = 0,read_C = 0;
    forn(i,width)
    {
        if(rename[i].valid == 1)
            {
                // rn_contains_bundle = true;
                rn_C++;
            }

    }

    if(rn_C>0)
    {
        // If RR is empty
        forn(i,width)
        {
            if(rename[i].valid == 0)
                {
                    // rr_isempty = false
                    read_C++;
                }
        }
        // the ROB has
// enough free entries to accept the entire
// rename bundle,
        forn(i,rob_size)
        {
            if(rob[i].valid == 0)
            {
                rob_C++;
            cout<<"we are inside";
                

            }
        }


        // if(rob_space_count >= width)    rob_have_space = true;
        //here conditions are specified and important actions are performed
        // cout<<rob_C<<endl;
        if(read_C == width && rob_C >= width)
        {
                cout<<"set rename to invalid"<<endl;
            // cout<<"the width is "<<width;
            forn(i,width)
            {
                // cout<<rename[i].valid;
        // cout<<"at rename stage"<<endl;
        // cout<<"    the rename valid bit is"<<rename[i].valid;
                if(rename[i].valid == 1)//if the rename reg have value
                {
                    

                    //--------rename source registers---------//
                    //check if the instruction have the src1
                    if(rename[i].src1_value != -1)
                    {   
                        //check inside the rt table
                        if(rt[(rename[i].src1)].valid == 1)
                        {
                            //most recent version
                            rename[i].src1_tag = rt[(rename[i].src1_value)].rob_tag;
                            rename[i].src1_ROB = 1; //value came from rt that is the most recent index and flagged
                        }
                        else
                            rename[i].src1_ROB = 0;
                    }
                    if(rename[i].src2!= -1)
                    {
                        if(rt[(rename[i].src2)].valid == 1)
                        {
                            rename[i].src2_tag = rt[(rename[i].src2_value)].rob_tag;
                            rename[i].src2_ROB = 1;
                        }
                        else
                            rename[i].src2_ROB = 0;
                    }
                    
                    //------rename destination registers------//
                    if(rename[i].dest != -1)
                    {
                        rt[rob[TAIL].dest].rob_tag = rob[TAIL].value;
                        rt[rob[TAIL].dest].valid = 1;
                    }
                    //allocating entry for ROB
                    rob[TAIL].valid = 1;
                    rob[TAIL].dest = rename[i].dest;
                    rob[TAIL].rdy = 0;
                    rob[TAIL].exc = 0;
                    rob[TAIL].mis = 0;
                    rob[TAIL].pc  = 0;

                    rename[i].dest_tag = rob[TAIL].value;

                    //INCREMENTING TAIL 
                    if(TAIL == rob_size-1) TAIL = 0;
                    else TAIL++;

                    //TRANSFER INSTRUCTION FROM RENAME TO REG_READ
                    if(reg_read[i].valid == 0 )
                    {
                        if(rename[i].valid == 1)
                        {
                            rename[i].valid = 0;

                            reg_read[i].op_type = rename[i].op_type;
                            reg_read[i].dest = rename[i].dest;
                            reg_read[i].dest_tag = rename[i].dest_tag;
                            reg_read[i].src1_value = rename[i].src1_value;
                            reg_read[i].src1_tag = rename[i].src1_tag;
                            reg_read[i].src2_value = rename[i].src2_value;
                            reg_read[i].src2_tag = rename[i].src2_tag;
                            // reg_read[i].src
                            reg_read[i].valid = 1;
                            reg_read[i].src1_ROB = rename[i].src1_ROB;
                            reg_read[i].src2_ROB = rename[i].src2_ROB;
                            reg_read[i].src1_rdy_flag = 0;
                            reg_read[i].src2_rdy_flag = 0;

                            Output[reg_read[i].dur].RR = output_counter++;


                        }
                    }

                }
                // cout<<"\nthis";
            }
        }
    }
    return;
}

void func_RegRead(Register_Read RegRead[],DISPATCH dispatch[], RENAME rename[], rename_table rt[], ROB rob[] )
{
    // cout<<"\nat regread";

    //check traffic in the respective stages
    // bool is_regRead_empty = true;
    // int count,count1;
    int disp_count,read_count ;
    // count = count1 =0; 
    disp_count = read_count = 0;

    // // bool is_dispatch_empty = true;
    // bool is_rob_counter_empty = true;
    bool is_value1_dependent = false;
    bool is_value2_dependent = false;

    //if traffic conditions are satisfied
    forn(i,width)
    {
        if(dispatch[i].valid == 1)
        {
            // is_dispatch_empty = false;
            read_count++;
        }
        if(RegRead[i].valid == 1)
        {
            // is_regRead_empty = false;
            disp_count++;
        }
    }
    //if regread is not empty
    // if(!is_regRead_empty)
    if(read_count!=0) 
    {
        // if(is_dispatch_empty)
        if(disp_count == width)
        {
            forn(i,width)
            {
                if(RegRead[i].valid == 1)
                {

                    if(RegRead[i].src1_value != -1)
                    {
                        //check dependency
                        if(rt[RegRead[i].src1_value].valid == 1)    is_value1_dependent = true;



                        if(is_value1_dependent)
                        {
                            //it is dependent 
                            forn(j,rob_size)
                            {
                            //find in rob
                                if(rob[j].dest == RegRead[i].src1_value&& rob[j].rdy == 1)
                                {
                                    if(rob[RegRead[i].src1_tag].rdy == 1)
                                    {
                                        RegRead[j].src1_rdy_flag = 1;
                                    }
                                }
                                else if(rob[RegRead[i].src1_tag].valid == 0)
                                {
                                    RegRead[i].src1_rdy_flag = 1;
                                }

                            }

                        }
                        else 
                            {
                                RegRead[i].src1_rdy_flag = 1;
                            }
                    }
                    if(RegRead[i].src1_value == -1)
                    {
                        RegRead[i].src1_rdy_flag = 1;
                    }

                    if(RegRead[i].src2_value != -1)
                    {
                        //check dependency 
                        if(rt[RegRead[i].src2_value].valid == 1)    is_value2_dependent = true;

                        if(is_value2_dependent)
                        {
                            //it is dependent 
                            forn(j,rob_size)
                            {
                            //find in rob
                            if(rob[j].dest == RegRead[i].src2_value && rob[j].rdy == 1)
                            {
                                if(rob[RegRead[i].src2_tag].rdy == 1)
                                {
                                    RegRead[i].src2_rdy_flag = 1;
                                }
                            }
                            else if(rob[RegRead[i].src2_tag].valid == 0)
                            {
                                RegRead[i].src2_rdy_flag = 1;
                            }

                            }

                        }
                        else 
                            {
                                RegRead[i].src2_rdy_flag = 1;
                            }
                    }
                    
                    if(RegRead[i].src2_value == -1)
                    {
                        RegRead[i].src2_rdy_flag = 1;
                    }
                }
                //transfer instructions/bundle  from reg_read to Dispatch 
                if(dispatch[i].valid == 0 && RegRead[i].valid == 1)
                {
                    RegRead[i].valid = 0;
                    dispatch[i].dest=RegRead[i].dest;
                    dispatch[i].dest_tag=RegRead[i].dest_tag;
                    dispatch[i].src1_rdy_flag=RegRead[i].src1_rdy_flag;
                    dispatch[i].src1_tag=RegRead[i].src1_tag;
                    dispatch[i].src1_value=RegRead[i].src1_value;
                    dispatch[i].src2_rdy_flag=RegRead[i].src2_rdy_flag;
                    dispatch[i].src2_tag=RegRead[i].src2_tag;
                    dispatch[i].src2_value=RegRead[i].src2_value;
                    dispatch[i].op_type=RegRead[i].op_type;
                    dispatch[i].src1_ROB=RegRead[i].src1_ROB;
                    dispatch[i].src1_ROB=RegRead[i].src2_ROB;
                    dispatch[i].valid=1;
                    Output[dispatch[i].dur].DI = output_counter++;


                }
            }
        }
    }
    return;
    
}

void func_Dispatch(DISPATCH dispatch[],Issue_queue iq[],rename_table rt[],ROB rob[])
{
    // bool is_dispatch_empty = true;
    // bool is_iq_empty = true;

    int N_inst_IQ = 0;
    int N_inst_disp = 0;


    forn(i,width)
    {
        if(dispatch[i].valid == 1)
        {
            N_inst_disp++;
            // is_dispatch_empty = false;
        }
    }   
    forn(i,iq_size)
    {
        if(iq[i].valid == 1)
        {
            N_inst_IQ++;
            // is_iq_empty = false;
        }
    }

    forn(i,width)
    {
        if(rt[dispatch[i].src1_value].valid == 1 && (dispatch[i].valid == 1) && (dispatch[i].src1_ROB))
        {
            if((dispatch[i].valid == 1) && (dispatch[i].src1_ROB) && (rob[dispatch[i].src1_tag].rdy == 1))
            {
                dispatch[i].src1_rdy_flag = 1;
            }
 
        }
        else if(dispatch[i].valid ==1)
        {
            dispatch[i].src1_rdy_flag = 1;

        }

        if(rt[dispatch[i].src2_value].valid == 1 && (dispatch[i].valid == 1) && (dispatch[i].src2_ROB))
        {
            if((dispatch[i].valid == 1) && (dispatch[i].src2_ROB) && (rob[dispatch[i].src2_tag].rdy == 1))
            {
                dispatch[i].src2_rdy_flag = 1;
            }
        }
        else if(dispatch[i].valid)
        {
            dispatch[i].src2_rdy_flag = 1;

        }

    }

    if(N_inst_disp != 0)
    {
        if(N_inst_disp <= N_inst_IQ)
        {   
            forn(i,width)
            {
                if(dispatch[i].valid == 1)
                {
                    forn(j,iq_size)
                    {
                        if(iq[j].valid==0)   //move only at a vacant place
                        {
                            dispatch[i].valid=0;    //remove from DI bundle

                            //move from DI to IQ
                            iq[j].dest=dispatch[i].dest;
                            iq[j].dest_tag=dispatch[i].dest_tag;
                            iq[j].src1_rdy_flag=dispatch[i].src1_rdy_flag;
                            iq[j].src1_tag=dispatch[i].src1_tag;
                            iq[j].src1_value=dispatch[i].src1_value;
                            iq[j].src2_rdy_flag=dispatch[i].src2_rdy_flag;
                            iq[j].src2_tag=dispatch[i].src2_tag;
                            iq[j].src2_value=dispatch[i].src2_value;
                            iq[j].op_type=dispatch[i].op_type;
                            iq[j].src1_ROB=dispatch[i].src1_ROB;
                            iq[j].src2_ROB=dispatch[i].src2_ROB;
                            iq[j].valid=1;

                            Output[iq[j].dur].IS = output_counter++;

                            break;
                        }

                    }

                }
            }
        }
    }
    return;

}

void func_Issue(EXECUTE exc[],rename_table rt[],Issue_queue iq[], ROB rob[])
{
    count_iq = 0;
    tempK = 0;
    k = 0;
    int temp;
    // int m;
    forn(i,iq_size)
    {
        if(iq[i].valid == 1)
        {
            if(rt[iq[i].src1_value].valid == 1 && iq[i].src1_ROB == 1 )
            {
                if(rob[iq[i].src1_tag].rdy == 1)
                {
                    iq[i].src1_rdy_flag = 1;
                }
                
            }

            if(rt[iq[i].src2_value].valid == 1 && iq[i].src2_ROB == 1)
            {
                if(rob[iq[i].src2_tag].rdy == 1)
                {
                    iq[i].src2_rdy_flag = 1;
                }
                
            }
        }
        else if(iq[i].valid == 1)
        {
            iq[i].src1_rdy_flag = 1;
            iq[i].src2_rdy_flag = 1;
        }
        
    }


    forn(i,iq_size)
    {
        if(iq[i].valid == 1)
        {
            if(iq[i].src1_rdy_flag == 1 && iq[i].src2_rdy_flag == 1) 
                {
                    count_iq++;
                }
        }

    }

    int executable[count_iq];
    forn(i,iq_size)
    {
        if(iq[i].valid == 1)
        {
            if(iq[i].src1_rdy_flag == 1 && iq[i].src2_rdy_flag == 1)
            {
                executable[k] = iq[i].dur;
                k++;
            }
        }
    }

    // int len = sizeof(executable)/sizeof(executable[0]);
    // sort(executable,executable+len);


    forn(i,count_iq)
    {
        forn(k,count_iq)
        {
            if(executable[k]>executable[k+1])
            {
                temp = executable[k];
                executable[k] = executable[k+1];
                executable[k+1] = temp;
            }
        }
    }


    forn(i,width)
    {
        forn(k,iq_size)
        {
            if(executable[i] == iq[k].dur && iq[k].valid == 1) 
            {
                
                break;
            }
        }
        iq[k].valid = 0;
        forn(x,width*5)
        {
            if(exc[x].valid == 0)
            {

                exc[x].src1_value=iq[k].src1_value;
                exc[x].dest_tag = iq[k].dest_tag;
                exc[x].src2_ROB=iq[k].src1_ROB;
                exc[x].op_type=iq[k].op_type;
                exc[x].dur=iq[k].dur;
                exc[x].dest=iq[k].dest;
                exc[x].src1_ROB=iq[k].src1_ROB;
                exc[x].src2_value=iq[k].src2_value;
                exc[x].valid=1;

                switch (iq[k].op_type)
                {
                case 0: 
                        exc[x].latency = 1;
                    break;
                case 1: exc[x].latency = 2;
                case 2: exc[x].latency = 5;
                }

                Output[exc[x].dur].EX = output_counter++;

                break;
            }             
        }
    }  
    return; 
}

void func_Execute(EXECUTE exc[],Issue_queue iq[], ROB rob[], WRITEBACK wb[], DISPATCH di[], Register_Read Regread[])
{
    forn(i, width*5)
    {
        if(exc[i].valid)
        {
            exc[i].latency--;
            if(exc[i].latency == 0)
            {
                forn(j,width*5)
                {
                    
                    if(wb[j].valid == 0)
                    {
                        exc[i].valid = 0;

                        wb[j].valid =1;
                        wb[j].op_type = exc[i].op_type;
                        wb[j].dest = exc[i].dest;
                        wb[j].src1 = exc[i].src1;
                        wb[j].src2 = exc[i].src2;
                        wb[j].dest_tag = exc[i].dest_tag;
                        wb[j].dur = exc[j].dur;

                        Output[wb[j].dur].WB = output_counter++;
                        break;

                    }
                }

                forn(k,iq_size)
                {
                    if(iq[k].valid ==1)
                    {
                        if(iq[k].src1_tag == exc[i].dest_tag && iq[k].src1_rdy_flag == 0)
                        {
                            iq[k].src1_rdy_flag = 1;
                        }
                        if(iq[k].src2_tag == exc[i].dest_tag && iq[k].src2_rdy_flag == 0)
                        {
                            iq[k].src2_rdy_flag = 1;
                        }
                        
                    }
                }

                forn(k,width)
                {
                    if(di[k].valid ==1)
                    {
                        if(di[k].src1_tag == exc[i].dest_tag && di[k].src1_rdy_flag == 0)
                        {
                            di[k].src1_rdy_flag = 1;
                        }
                        if(di[k].src2_tag == exc[i].dest_tag && di[k].src2_rdy_flag == 0)           
                        {
                            di[k].src2_rdy_flag = 1;
                        }
                        if(Regread[k].src1_tag == exc[i].dest_tag && Regread[k].src1_rdy_flag == 0)
                        {
                            Regread[k].src1_rdy_flag = 1;
                        }
                        if(Regread[k].src2_tag == exc[i].dest_tag && Regread[k].src2_rdy_flag == 0)             
                        {
                            Regread[k].src2_rdy_flag = 1;
                        }
                    }
                }
            }
        }

    }
    return;
}

void func_Writeback(WRITEBACK wb[],ROB rob[])
{
    forn(i,width*5)
    {
        if(wb[i].valid == 0)
        {
            //do nothing
        }
        if(wb[i].valid == 1)
        {
            forn(k,rob_size)
            {
                if(rob[k].valid == 1)
                {
                    if(rob[k].rdy == 0 && wb[i].dest_tag == rob[k].value)
                    {
                        rob[k].rdy = 1;
                        wb[i].valid = 0;
                        // rob[k].dur
                        rob[k].dur = wb[i].dur;
                        Output[rob[k].dur].RT1 = output_counter++;

                        break;
                    }
                }
            }
        }
    }
    return; 
}
void func_Retire(rename_table rt[],ROB rob[])
{
    forn(i,width)
    {
        if(rob[HEAD].rdy == 1)
        {
            if(rob[HEAD].valid == 1 && rob[HEAD].dest!=-1)
            {
                if((rt[rob[HEAD].dest].rob_tag == rob[HEAD].value) && rt[rob[HEAD].dest].valid == 1)
                {
                    rt[rob[HEAD].dest].valid = 0;
                    rt[rob[HEAD].dest].rob_tag = 0;
                }

                rob[HEAD].exc = 0;
                rob[HEAD].mis = 0;
                rob[HEAD].rdy = 0;
                rob[HEAD].valid = 0;
                Output[rob[HEAD].dur].RT2 = output_counter++;

                if(rob[HEAD].value != rob_size-1)   HEAD++;
                else if(rob[HEAD].value == rob_size-1)   HEAD = 0;
            }
            else if(rob[HEAD].dest == -1)
            {
                if(rob[HEAD].rdy == 1 && rob[HEAD].valid == 1)
                {
                    rob[HEAD].exc = 0;
                    rob[HEAD].mis = 0;
                    rob[HEAD].rdy = 0;
                    rob[HEAD].valid = 0;
                    Output[rob[HEAD].dur].RT2 = output_counter++;

                    if(rob[HEAD].value != rob_size-1)   HEAD++;
                    else if(rob[HEAD].value == rob_size-1)    HEAD = 0;
                }
            }
            
        }
        else    break;
    }
    return;
}