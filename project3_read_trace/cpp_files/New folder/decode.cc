#include <iostream>
#include <deque>
#include "sim_ds.h"

#define foru(i,n) for(unsigned int i=0;i<n;i++)
#define itern(it,en) for(auto it = en.begin();it!=en.end();it++)
#define VALID 1
#define INVALID -1
#define NON 0
#define nonblock width*5
extern int fail;

extern std::deque<pipeline> DE_register;
extern std::deque<pipeline> RN_register;
extern   unsigned int  width;
extern   unsigned int  DE_time;
extern   unsigned int   number_of_cycles;
using namespace std;
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
            RN_register.push_back(DE_register.front());
            DE_register.pop_front();
        }
    }
    return;
}
