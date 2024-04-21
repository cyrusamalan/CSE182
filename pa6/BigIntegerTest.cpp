// Cyrus Amalan, camalan, pa6

#include <iostream>
#include <cassert>
#include "BigInteger.h"


bool operator!=(const BigInteger& A, const BigInteger& B) {
    return !(A == B);
}


int main() {
    try {
        // Test constructors and to_string()
        BigInteger num1(1234);
        BigInteger num2("-5678");
        std::cout << "num1: " << num1 << std::endl;
        std::cout << "num2: " << num2 << std::endl;
        assert(num1.to_string() == "1234");
        assert(num2.to_string() == "-5678");

        // Test sum
        BigInteger sum = num1 + num2;
        std::cout << "Sum: " << sum << std::endl;
        assert(sum.to_string() == "-4444");

        // Test Difference
        BigInteger diff = num1 - num2;
        std::cout << "Difference: " << diff << std::endl;
        assert(diff.to_string() == "6912");

        // Test product
        BigInteger product = num1 * num2;
        std::cout << "Product: " << product << std::endl;
        assert(product.to_string() == "-7006652");

        // Test comparison 
        std::cout << "num1 == num2: " << (num1 == num2 ? "true" : "false") << std::endl;
        std::cout << "num1 < num2: " << (num1 < num2 ? "true" : "false") << std::endl;
        std::cout << "num1 > num2: " << (num1 > num2 ? "true" : "false") << std::endl;
        assert(num1 == BigInteger("1234"));
        assert(num1 != num2);
        assert(num2 < num1);

        // Negate num1 and check
        num1.negate();
        std::cout << "Negated num1: " << num1 << std::endl;
        assert(num1.to_string() == "-1234");
        num1.negate();
        std::cout << "Negated back: " << num1 << std::endl;
        assert(num1.to_string() == "1234");

        // Create zero
        BigInteger zero;
        zero.makeZero();
        std::cout << "Zero: " << zero << std::endl;
        assert(zero.to_string() == "0");

        // Create BigInteger from string and test
        BigInteger fromString("9677809");
        std::cout << "fromString: " << fromString << std::endl;
        assert(fromString.to_string() == "9677809");


    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

     // All tests passed
    std::cout << "All tests passed." << std::endl;

    return 0;
}
