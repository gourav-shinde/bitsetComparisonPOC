#include<iostream>
#include<bitset>
#include<chrono>


// template<std::size_t N1, std::size_t N2>
// std::bitset<N1+N2> operator +(std::bitset<N1> lhs, std::bitset<N2> rhs){
//     std::bitset<N1+N2> temp;
//     for(int i=0;i<N2;++i)
//         temp[i]=rhs.test(i);
//     for(int i=0;i<N1;++i)
//         temp[i+N2]=lhs.test(i);
//     return temp;
// }

template< std::size_t N1, std::size_t N2 >
std::bitset<N1+N2> operator+(const std::bitset<N1>& a, const std::bitset<N2>& b ) {
    return std::bitset<N1+N2>(a.to_string() + b.to_string());
}

// template<std::size_t N>
// bool operator<(const std::bitset<N>& x, const std::bitset<N>& y) {
//     size_t a{0},b{0};
//     while(a!=N && b!=N){
//         if((a=x._Find_next(a))<(b=y._Find_next(b))){
//             return false;
//         }
//     }
//     return true;
// }

template<std::size_t N>
bool operator<(const std::bitset<N>& x, const std::bitset<N>& y) {
    for (int i = N-1; i >= 0; i--) {
        if (x[i] ^ y[i]) return y[i];
    }
    return false;
}

int main(){
    std::bitset<64> a(5);
    std::bitset<64> b(7);
    std::bitset<64> c(11);
    std::bitset<64> d(23);
    std::bitset<64> e(1);

    int i =1000;
    std::bitset<320> res;
    auto start = std::chrono::steady_clock::now();
    while(--i){
        res=a+b+c+d+e;
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in nanoseconds: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " ns" << std::endl;
    std::cout<<res<<std::endl;
}