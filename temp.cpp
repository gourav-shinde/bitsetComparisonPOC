#include <iostream>

// Function to pack three numbers into an unsigned int
unsigned int packNumbers(unsigned int num1, unsigned int num2, unsigned int num3) {
    // Make sure each number is within its bit range
    num1 &= 0x7FF; // 11 bits
    num2 &= 0x7FF; // 11 bits
    num3 &= 0x3FF; // 10 bits

    // Pack the numbers into a single unsigned int
    unsigned int packedNum = (num1 << 21) | (num2 << 10) | num3;

    return packedNum;
}

// Function to retrieve the first number from the packed value
unsigned int getNum1(unsigned int packedNum) {
    return (packedNum >> 21) & 0x7FF;
}

// Function to retrieve the second number from the packed value
unsigned int getNum2(unsigned int packedNum) {
    return (packedNum >> 10) & 0x7FF;
}

// Function to retrieve the third number from the packed value
unsigned int getNum3(unsigned int packedNum) {
    return packedNum & 0x3FF;
}

int main() {
    // Example values
    unsigned int num1 = 500; // 11 bits
    unsigned int num2 = 700; // 11 bits
    unsigned int num3 = 300; // 10 bits

    // Pack the numbers
    unsigned int packedValue = packNumbers(num1, num2, num3);

    // Retrieve the numbers
    unsigned int retrievedNum1 = getNum1(packedValue);
    unsigned int retrievedNum2 = getNum2(packedValue);
    unsigned int retrievedNum3 = getNum3(packedValue);

    // Display the results
    std::cout << "Original numbers: " << num1 << ", " << num2 << ", " << num3 << std::endl;
    std::cout << "Packed value: " << packedValue << std::endl;
    std::cout << "Retrieved numbers: " << retrievedNum1 << ", " << retrievedNum2 << ", " << retrievedNum3 << std::endl;

    return 0;
}
