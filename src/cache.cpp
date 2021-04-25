#include"cache.h"
using namespace std;

int hex2dec(string a){
    int num=0;
    char c[20];
    for(int i=0;i<a.length();i++) c[i]=a[i];
    sscanf(c,"%x",&num);
    return num;
};

void InitCache(cache_item c[]){
    for(int i=0;i<max_cache_line;i++){
        c->cache_line.reset();
    }
};

bool IsHit(bitset<32> flag){
    bool ret=0;
    if(t_assoc==direct){
        bitset<32> tempflag;
        for(int j=0,i=bit_block;i<bit_block+bit_line;i++,j++){
            tempflag[j]=flag[i];
        }
        current_line=tempflag.to_ulong();
        if(cache[current_line].cache_line[31]==0) {
            return false;
        }
        ret=1;
        for(int i=31,j=30;i>31-bit_tag;i--,j--){
            if(flag[i]!=cache[current_line].cache_line[j]){
                ret=false;
                break;
            }
        }
    }
    else if(t_assoc==set){
        bitset<32> tempflag;
        for(int j=0,i=bit_block;i<bit_block+bit_set;j++,i++){
            tempflag[j]=flag[i];
        }
        current_set=tempflag.to_ulong();
        for(int temp=current_set*i_cache_set;temp<(current_set+1)*i_cache_set;temp++){
            if(cache[temp].cache_line[31]==1){
                ret=1;
                for(int i=31,j=30;i>31-bit_tag;i--,j--){
                    if(flag[i]!=cache[temp].cache_line[j]){
                        ret=false;
                        break;
                    }
                }
                if(ret==1){
                    current_line=temp;
                    break;
                }
            }
            else{
                return false;
            }
        }

    }
    else{
        for(int temp=0;temp<max_cache_line;temp++){
            if(cache[temp].cache_line[31]==1){
                ret=1;
                for(int i=31,j=30;i>(31-bit_tag);i--,j--){
                    if(flag[i]!=cache[temp].cache_line[j]){
                        ret =0;
                        break;
                    }
                }
            }
            if(ret){
                current_line=temp;
                break;
            }
        }
    }

    return ret;
};

void replace(bitset<32> flag){
    bool space=0;
    if(t_assoc==direct){
    }
    else if(t_assoc==set){
        for(int m=current_set*i_cache_set;m<(current_set+1)*i_cache_set;m++){
            if(cache[m].cache_line[31]==0){
                if(t_replace==LRU){
                    LRU_space();
                }
                space=1;
                current_line=m;
                break;
            }
        }
        if(t_replace==random&&!space){
            unsigned int temp = rand()%(i_cache_set);
            current_line=current_set*i_cache_set+temp;
            }
        else if(t_replace==LRU&&!space){
            LRU_nospace();
        }   
    }
    else{
        for(int i=0;i<max_cache_line;i++){
            if(cache[i].cache_line[31]==0){
                if(t_replace==LRU){
                    LRU_space();
                }
                space=1;
                current_line=i;
                break;
            }
        }
        if(t_replace==random&&!space){
            current_line=rand()%i_cache_set;
        }
        else if(t_replace==LRU&&!space){
            LRU_nospace();
        }
    }
    cache[current_line].cache_line[31]=1;
    for(int i=31,j=30;i>31-bit_tag;i--,j--){
        cache[current_line].cache_line[j]=flag[i];
    }
    //cout << cache[current_line].cache_line<<endl;
}

void CalcInfo(){
    if(t_assoc==direct){
        bit_set=0;
        bit_line=log2(max_cache_line);
    }
    else if(t_assoc==set){
        bit_line=0;
        i_num_set=max_cache_line/i_cache_set;
        bit_set=log2(i_num_set);
    }
    else{
        bit_line=0;
        bit_set=0;
    }
    bit_tag=32-bit_block-bit_line-bit_set;
    cout << bit_tag<<" "<<bit_line<<" "<<bit_set<<" "<<bit_block<<endl;
}

void LRU_hit(){
    if(t_assoc==set){
        for(int i=current_set*i_cache_set;i<(current_set+1)*i_cache_set;i++){
            if(cache[i].cache_line[31]==1){
                LRU_priority[i]++;
            }
        }
        LRU_priority[current_line]=0;
    }
    else if(t_assoc==full){
        for(int i=0;i<max_cache_line;i++){
            if(cache[i].cache_line[31]==1){
                LRU_priority[i]++;
            }
        }
        LRU_priority[current_line]=0;
    }

}
void LRU_space(){
    if(t_assoc==set){
        for(int i=current_set*i_cache_set;i<(current_set+1)*i_cache_set;i++){
            if(cache[i].cache_line[31]==1){
                LRU_priority[i]++;
            }
        }
        LRU_priority[current_line]=0;
    }
    else if(t_assoc==full){
        for(int i=0;i<max_cache_line;i++){
            if(cache[i].cache_line[31]==1){
                LRU_priority[i]++;
            }
        }
        LRU_priority[current_line]=0;
    }
}

void LRU_nospace(){
    if(t_assoc==set){
        long int temp=LRU_priority[current_set*i_cache_set];
        int j=current_set*i_cache_set;
        for(int i=current_set*i_cache_set;i<(current_set+1)*i_cache_set;i++){
            if(LRU_priority[i]>=temp){
                temp=LRU_priority[i];
                j=i;
            }
        }
        current_line=j;
    }
    else if(t_assoc==full){
        long int temp=LRU_priority[0];
        int j=0;
        for(int i=0;i<max_cache_line;i++){
            if(LRU_priority[i]>=temp){
                temp=LRU_priority[i];
                j=i;
            }
        }
        current_line=j;
    }
}

void Input(string addr){
    bitset<32> tmp;
    if(addr.length()!=0){
        sum_time++;
        tmp=hex2dec(addr.substr(2,addr.length()));
        switch (addr[0]){
                case 'r':
                case 'R':
                    read_time++;
                    if(IsHit(tmp)){
                        if(t_replace==LRU) LRU_hit();
                        read_hit_time++;
                    }
                    else{
                        // replace(tmp);
                    }
                    
                    break;
                case 'w':
                case 'W':
                    write_time++;
                    if(IsHit(tmp)){
                        write_hit_time++;
                        if(t_replace==LRU) LRU_hit();
                    }
                    else{
                        replace(tmp);
                    }
                    break;
                default:
                    break;

        }
    }
}

void HitRate(){
    cout << "access time: "<<sum_time<<endl;
    cout << "read time: "<<read_time<<endl;
    cout << "write time: "<<write_time<<endl;
    cout << "average hit rate: "<<(read_hit_time+write_hit_time)/sum_time*100<<"%"<<endl;
    cout << "read hit rate: "<<read_hit_time/read_time*100<<"%"<<endl;
    cout << "write hit rate: "<<write_hit_time/write_time*100<<"%"<<endl;

}
