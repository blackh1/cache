#include "cache.h"
#include<iomanip>
#define cache_size (1<<13)

using namespace std;

ASSOC t_assoc=set;
REPLACE t_replace=LRU;

unsigned long int current_line;
unsigned long int current_set;

double read_hit_time=0;
double write_hit_time=0;
double sum_time=0;
double read_time=0;
double write_time=0;

unsigned int i_num_set;
unsigned int i_cache_set=2;//x-set
unsigned int i_cache_size=cache_size/max_cache_line;//bytes

unsigned int bit_block=log2(i_cache_size);
unsigned int bit_line;
unsigned int bit_set;
unsigned int bit_tag;

cache_item cache[max_cache_line];
unsigned long int LRU_priority[max_cache_line];

int main(){
    cout << fixed;
    cout << setprecision(6);
    int i=0;
    srand(time(0)); 
    ifstream f; 
    cin >> i;
    if(i==1) f.open("MA2.txt");
    else f.open("MA2kij.txt");
    for(int m=0;m<3;m++){
        CalcInfo();
        InitCache(cache);
        // string filename;
        // cout << "Input filename:(xx.txt)\t";
        // getline(cin,filename);
       
        bitset<32> temp;
        // f.open(filename.c_str());
        
        while(!f.eof()){
            i++;
            if(i%1000000==0) cout <<"w-";
            string x;
            getline(f,x);
            Input(x);
        }
        HitRate();
        i_cache_set*=2;
    }
    system("pause");
    return 0;
}