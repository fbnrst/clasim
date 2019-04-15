// sample.h
#ifndef ANIMAL_H
#define ANIMAL_H
#include "types.h"
#include <vector>
#include <array>
#include <random>
#include "cell.h"


template < typename cell_type>
class sample
{
    URNG& RandGenerator;
    t_InitParameter& rInitParameter;
    REAL KillTime;
    REAL AnimalTc;
    std::vector<cell_type> cells;
    std::array<int,2> LabelIndex{{0,0}};
    void create_cells(int mode);
    REAL create_inittime(REAL Tc);
    int to_many_cells;
public:
    sample(t_InitParameter& rInit,unsigned nKTime,REAL ATc);
    sample(t_InitParameter& rInit,unsigned nKTime,REAL ATc,int mode);
    ~sample(void);
    REAL get_killtime();
    std::string get_cell(void);
    std::string get_result_str(void);
    REAL get_result(void);
    void run();
};


template <>
REAL sample<cell_sym>::create_inittime(REAL Tc);
template <>
REAL sample<cell_sym>::get_result(void);


#include "sample.cpp"
#endif
