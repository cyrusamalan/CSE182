// Cyrus Amalan, camalan, pa6

#include <cctype>
#include <cmath>
#include <stdexcept>
#include "BigInteger.h"
#include "List.h"

using namespace std;

const long ROOT = 1000000000;
const int POWER = 9;

void delete_zero(List* L){
    if (L == nullptr || L->length() == 0) return;
    L->moveFront();
    while (L->length() > 0 && !L->peekNext()) {
        L->eraseAfter();
    }
}

List temp(long s, List* b, int* ctr) {
    List result;
    if (b == nullptr || s == 0) {
        result.insertAfter(0); 
        return result;
    }

    long carry = 0;
    b->moveBack(); 
    while (b->position() > 0) {
        long product = b->peekPrev() * s + carry; 
        carry = product / ROOT; // Update carry
        long digit = product % ROOT; 
        result.insertAfter(digit); // Insert the digit into the result list
        b->movePrev(); // Move to the next digit
    }

    if (carry > 0) {
        result.insertAfter(carry); // If there's remaining carry, insert it
    }
    
    result.moveBack(); 
    for (int i = 0; i < *ctr; ++i) {
        result.insertAfter(0); // put zeros at the end 
    }
    
    return result;
}

BigInteger::BigInteger() {
    signum = 0;
    digits = List();
}

BigInteger::BigInteger(long x) {
    if (x == 0) {
        signum = 0;
        digits.insertAfter(0); //  insert 0 
        return; // Return early
    }

    // Determines the sign of the BigInteger 
    signum = (x < 0) ? -1 : 1;
    
    // Convert x to positive 
    if (x < 0) {
        x = -x;
    }

    // Extract and insert digits from x
    while (x != 0) {
        long digit = x % ROOT; // Extract x
        digits.insertBefore(digit); // Insert this digit
        x /= ROOT; // Remove digit from x
    }
}


BigInteger::BigInteger(std::string s) {
    if (s.empty()) {
        throw std::invalid_argument("empty string");
    }

    //strip it from the string if present
    signum = (s.front() == '-') ? -1 : 1;
    if (s.front() == '+' || s.front() == '-') {
        s.erase(0, 1); // Remove the sign character
    }

    // Check if the string is numeric
    if (s.find_first_not_of("0123456789") != std::string::npos) {
        throw std::invalid_argument("non-numeric character found");
    }

    // Remove leading zeros 
    s.erase(0, s.find_first_not_of('0'));
    if (s.empty()) { //case where s was all zeros
        s = "0";
        signum = 0;
    }
    // Processesing the string  
    while (!s.empty()) {
        size_t len;
    if (s.length() > POWER) {
        len = POWER; 
    } else {
        len = s.length(); //set len to the length of the string s
    }   
        std::string chunk = s.substr(s.length() - len);
        long value = std::stol(chunk);
        digits.insertAfter(value);
        if (s.length() > POWER) {
            s.erase(s.length() - len);
        } else {
            s.clear();
        }
    }
    delete_zero(&digits);  
}

BigInteger::BigInteger(const BigInteger& N){
    this->signum = N.signum;
    this->digits = List(N.digits);
}


int BigInteger::sign() const{
    return signum;
}

void BigInteger::makeZero(){
    this->signum = 0;
    this->digits.clear();
}

void BigInteger::negate(){
    this->signum *= -1;
}


int BigInteger::compare(const BigInteger& N) const {
    // Simple case, compare signs
    if (signum > N.signum) {
        return 1;
    }
    else if (signum < N.signum) {
        return -1;
    }
    else if (N.signum == 0 && signum == 0) {
        return 0;
    }
    // Same sign, do checks
    int Length_1 = digits.length();
    int Length_2 = N.digits.length();

    if (Length_1 != Length_2) {
        // If lengths differ, the longer list is larger
        if (Length_1 > Length_2) {
            return signum;
        } else {
            return -N.signum;
        }
    }
    // create 2 copies
    List copy1 = digits;
    List copy2 = N.digits;
    int i;
    // Move to the front lists
    copy1.moveFront();
    copy2.moveFront();
    // Compare digits 
    for (i = 0; i < Length_1; ++i) {
        if (copy1.peekNext() > copy2.peekNext()) {
            //A is larger
            return signum;
        }
        else if (copy1.peekNext() < copy2.peekNext()) {
            return -N.signum;
        }
        // Move to the next element
        copy1.moveNext();
        copy2.moveNext();
    }
    // All digits are equal
    return 0;
}

BigInteger BigInteger::add(const BigInteger& N) const {
    BigInteger result;
    if (this->signum != N.signum) {
        if (this->signum < 0) {
            BigInteger temp = *this;
            temp.negate();
            result = N.sub(temp);
        } else {
            BigInteger temp = N;
            temp.negate();
            result = this->sub(temp);
        }
        return result;
    }

    result.signum = this->signum < 0 ? -1 : 1; 
    
    List A = this->digits;
    List B = N.digits;
    List& C = result.digits;

    A.moveBack();
    B.moveBack();
    long carry = 0;
    while (A.position() > 0 || B.position() > 0 || carry != 0) {
        long sum = carry;
        if (A.position() > 0) {
            sum += A.peekPrev();
            A.movePrev();
        }
        if (B.position() > 0) {
            sum += B.peekPrev();
            B.movePrev();
        }
        carry = sum / ROOT;
        sum %= ROOT;
        C.insertAfter(sum); 
    }

    if (carry > 0) {
        C.insertAfter(carry);
    }
    return result;
}

BigInteger BigInteger::sub(const BigInteger& N) const {
    BigInteger A = *this;
    BigInteger B = N;
    BigInteger C;
    List A_A = B.digits;
    List B_B = A.digits;
    List C_C = C.digits;

    // Case check
    if (B == A) {
        return C;  // Return zero 
    }
    if (B < A) {
        C = B.sub(A);
        C.negate();
        return C;  // Return negation of (A - B) 
    }
    if (B.sign() < 0 && A.sign() > 0) {
        A.negate();
        C = B.add(A);
        C.negate();
        return C;  // Return (A + (-B)) 
    }
    if (B.sign() && !A.sign()) {
        B.negate();
        return B;  // Return negation of A 
    }
    if (B.sign() < 0 && A.sign() < 0) {
        B.negate();
        A.negate();
        C = A.sub(B);
        C.negate();
        return C;  // Return (B - A) 
    }
    if (!B.sign() && A.sign()) {
        return A;  // Return B if A is negative 
    }
    
    if (B.sign() > 0 && A.sign() < 0) {
        B.negate();
        C = B.add(A);
        return C;  // Return (A + (-B)) 
    }


    A_A.moveBack();
    B_B.moveBack();
    long distance = 0, temp = 0;
    int i;
    for (i = B_B.position(); i > 0; i--) {
        long prev = A_A.peekPrev(), bPrev = B_B.peekPrev();
        if (prev - distance < bPrev) {
            temp = prev + ROOT - bPrev - distance;
            distance = 1;
        } else {
            temp = prev - distance - bPrev;
            distance = (prev > 0) ? 0 : distance;
        }
        C_C.insertAfter(temp);
        A_A.movePrev();
        B_B.movePrev();
    }
    for (; A_A.position() > 0; A_A.movePrev()) {
        long prev = A_A.peekPrev();
        if (prev - distance < 0) {
        temp = prev + ROOT - distance;
        distance = 1;
        } else {
        temp = prev - distance;
        distance = (prev > 0) ? 0 : distance;
        }
        C_C.insertAfter(temp);
    }
C.digits = C_C;
delete_zero(&(C.digits)); // Remove leading zeros 
C.signum = -1; // Set to negative
return C;
}



BigInteger BigInteger::mult(const BigInteger& N) const {
    BigInteger product;
    const BigInteger& multiplicand = *this;
    const BigInteger& multiplier = N;

    List multiplicandDigits = multiplicand.digits;
    List multiplierDigits = multiplier.digits;

    int shiftCounter = 0;
    
    multiplicandDigits.moveBack();
    multiplierDigits.moveBack();
    
    int position = multiplicandDigits.position();
    int i;
    for (i = position; i > 0; i--) {
        List tempResult = temp(multiplicandDigits.peekPrev(), &multiplierDigits, &shiftCounter);
        
        BigInteger tempBigInteger;
        tempBigInteger.signum = 1;
        tempBigInteger.digits = tempResult;
        
        product = product.add(tempBigInteger);
        
        multiplicandDigits.movePrev();
        
        shiftCounter++;
    }
    
    product.signum = multiplicand.signum * multiplier.signum;
    
    return product;
}

std::string BigInteger::to_string() {
    std::string output;

    if (signum == 0) {
        return "0"; // Return 0
    } else if (signum < 0) {
        output += "-"; 
    }

    // iterate through the digits from most to least 
    bool isFirstDigit = true; // Flag to track the first digit (to avoid padding it)
    for (digits.moveFront(); digits.position() < digits.length(); digits.moveNext()) {
        int digit = digits.peekNext();
        std::string digitStr = std::to_string(digit);

        if (isFirstDigit) {
            output += digitStr; // append the first digit
            isFirstDigit = false;
        } else {
            std::string paddedDigitStr = std::string(POWER - digitStr.length(), '0') + digitStr;
            output += paddedDigitStr;
        }
    }

    return output;
}

std::ostream& operator<<( std::ostream& stream, BigInteger N ){
    return stream << N.to_string();
}

bool operator==( const BigInteger& A, const BigInteger& B ) {
    int i = A.compare(B);
    return i == 0;
}

bool operator<( const BigInteger& A, const BigInteger& B ) {
    int i = A.compare(B);
    return (i == -1);
}

bool operator<=( const BigInteger& A, const BigInteger& B ) {
    int i = A.compare(B);
    return ((i == 0) || (i == -1));
}

bool operator>( const BigInteger& A, const BigInteger& B ) {
    int i = A.compare(B);
    return (i == 1);
}

bool operator>=( const BigInteger& A, const BigInteger& B ) {
    int i = A.compare(B);
    return ((i == 0) || (i == 1));
}
 
BigInteger operator+( const BigInteger& A, const BigInteger& B ) {
    return A.add(B);
}

BigInteger operator+=( BigInteger& A, const BigInteger& B ){
    A = A.add(B);
    return A;
}

BigInteger operator-( const BigInteger& A, const BigInteger& B ) {
    return A.sub(B);
}

BigInteger operator-=( BigInteger& A, const BigInteger& B ){
    A = A.sub(B);
    return A;
}
 
BigInteger operator*( const BigInteger& A, const BigInteger& B ) {
    return A.mult(B);
}

BigInteger operator*=( BigInteger& A, const BigInteger& B ){
    A = A.mult(B);
    return A.mult(B);
}


