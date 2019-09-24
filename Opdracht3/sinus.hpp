//Author is Erik de Zeeuw
#ifndef SINUS_HPP
#define SINUS_HPP

#include <array>

constexpr double degrees_to_radian(double value){
    return 2 * 3.14 * (value / 360.0);
}

//returns g^n
constexpr double pow(double g, int n){
    double value = 1;
    while(n > 0){
        value *= g;
        --n;
    }
    return value;
}

//returns g!
constexpr double fac(int n){
    double value = 1;
    while( n > 0){
        value *= n;
        --n;
    }
    return value;
}
//return sinus * 1000'000 for precision without floats
constexpr double sinus(double value){
    value = degrees_to_radian(value);
    return 1000000 * (value - pow(value, 3) / fac(3) + pow( value, 5) / fac(5) - pow(value, 7) / fac(7) + pow(value, 9) / fac(9)
    - pow(value, 11) / fac(11) + pow(value, 13) / fac(13));
}

//general lookup table with any types, sizes and functions
template<unsigned int N, typename T>
class table{
private:
    T values[N];
public:
    template<typename F>
    constexpr table(F function):
        values()
    {
        for(unsigned int i = 0; i < N; ++i){
            values[i] = function(i);
        }
    }
    
    constexpr T get(int n)const{
        return values[n];
    }
    
    
};

#endif