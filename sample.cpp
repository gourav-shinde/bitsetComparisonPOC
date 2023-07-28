#include <iostream>
#include <bitset>
#include <type_traits>
#include <limits>
#include <string>

template< std::size_t N1, std::size_t N2 >
std::bitset<N1+N2> operator+(const std::bitset<N1>& a, const std::bitset<N2>& b ) {
    return std::bitset<N1+N2>(a.to_string() + b.to_string());
}

template<std::size_t N>
bool operator<(const std::bitset<N>& x, const std::bitset<N>& y) {
    for (int i = N-1; i >= 0; i--) {
        if (x[i] ^ y[i]) return y[i];
    }
    return false;
}

int main() {
    std::bitset<32> bitSet1(6);
    std::bitset<32> bitSet2(-7);
    std::bitset<32> bitSet3(7);
    std::bitset<32> bitSet4(-7);
    auto bitSet5 = bitSet1 + bitSet2 + bitSet3;
    auto bitSet6 = bitSet3 + bitSet4;
    bool status = bitSet5 < bitSet6;
    std::cout << bitSet1 << std::endl;
    std::cout << bitSet2 << std::endl;
    std::cout << bitSet3 << std::endl;
    std::cout << bitSet4 << std::endl;
    std::cout << bitSet5 << std::endl;
    std::cout << bitSet6 << std::endl;
    std::cout << "bitSet < bitSet6 = " << status << std::endl;
}
