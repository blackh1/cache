#include<iostream>
#include<bitset>
#include<cmath>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<fstream>
#define max_cache_line (1<<10)
using namespace std;

struct cache_item{
    std::bitset<32> cache_line;
};

extern cache_item cache[max_cache_line];
extern unsigned long int LRU_priority[max_cache_line];
typedef enum associativity_way{direct=1,set,full} ASSOC;
typedef enum Replace_way{random=1,LRU} REPLACE;
extern ASSOC t_assoc;
extern REPLACE t_replace;
extern unsigned int i_cache_size;
extern unsigned int i_cache_set;
extern unsigned int i_num_set;

extern unsigned int bit_size;
extern unsigned int bit_block;
extern unsigned int bit_line;
extern unsigned int bit_tag;
extern unsigned int bit_set;

extern double sum_time;
extern double read_hit_time;
extern double read_time;
extern double write_time;
extern double write_hit_time;
extern unsigned long int current_line;
extern unsigned long int current_set;
void CalcInfo();
int hex2dec(string a);
void InitCache(cache_item c[]);
bool IsHit(bitset<32> flag);
void replace(bitset<32> flag);
void LRU_hit();
void LRU_space();
void LRU_nospace();
void Input(string addr);
void HitRate();