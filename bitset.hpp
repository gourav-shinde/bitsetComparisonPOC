#include<iostream>

template<size_t N>
class BITSET{
    public:
    bool array[N]{0};
    BITSET(){
        
    }
    void set(std::u_int32_t pos, bool val){
        array[pos]=val;
    }
    void get(std::u_int32_t pos){
        return array[pos];
    }

};

template<std::size_t N1,std::size_t N2>
BITSET<N1+N2> operator +( 
                            const BITSET<N1>& a,
                            const BITSET<N2>& b
                            )
{
    BITSET<N1+N2> res;
    for(size_t i=0;i<N1;++i)
        res.set(i, a[i]);

    for(size_t i=N1;i<N2;++i)
        res.set(i, b[i]);

    return res;
}
