#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include "SandPile.h"
struct Args{
    char* input;
    char* output;
    uint32_t max_iter;
    uint32_t freq;
    Args()=default;
    ~Args()=default;
};

struct tsv_args{
    uint16_t args_amount;
    int16_t* x_coord;
    int16_t* y_coord;
    uint64_t* sand_val;
    int16_t max_y=INT16_MIN;
    int16_t max_x=INT16_MIN;
    int16_t min_y=INT16_MAX;
    int16_t min_x=INT16_MAX;
    tsv_args(uint16_t argc){
        args_amount=argc/3;
        x_coord=new int16_t[args_amount];
        y_coord=new int16_t[args_amount];
        sand_val=new uint64_t [args_amount];
    }
    ~tsv_args()=default;
};

Args Parse(int, char**);

tsv_args Tsv_Parse(const char*);

Field tsv_to_field(tsv_args&);


