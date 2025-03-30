#pragma once

#include <cstdint> 

// Forward declarations
class Microseconds;
class Milliseconds;
class Seconds;

class Microseconds
{
public:    
    double mValue = 0.0f;

    Microseconds() = default;
    explicit Microseconds(double val);

    bool operator==(const Microseconds& other) const;
    bool operator!=(const Microseconds& other) const;
    bool operator>(const Microseconds& other) const;
    bool operator>=(const Microseconds& other) const;
    bool operator<(const Microseconds& other) const;
    bool operator<=(const Microseconds& other) const;
    explicit operator Milliseconds() const;
    explicit operator Seconds() const;
    Microseconds operator+(Microseconds other) const;
    Microseconds operator-(Microseconds other) const;
    Microseconds operator*(double scalar) const;
    operator double() const;
};

class Milliseconds
{
public:    
    double mValue = 0.0f;

    Milliseconds() = default;

    explicit Milliseconds(double val);

    bool operator==(const Milliseconds& other) const;
    bool operator!=(const Milliseconds& other) const;
    bool operator>(const Milliseconds& other) const;
    bool operator>=(const Milliseconds& other) const;
    bool operator<(const Milliseconds& other) const;
    bool operator<=(const Milliseconds& other) const;
    explicit operator Microseconds() const;
    explicit operator Seconds() const;
    Milliseconds operator+(Milliseconds other) const;
    Milliseconds operator-(Milliseconds other) const;
    Milliseconds operator*(double scalar) const;
    operator double() const;
};

class Seconds
{
public:    
    double mValue = 0.0f;

    Seconds() = default;

    explicit Seconds(double val);

    bool operator==(const Seconds& other) const;
    bool operator!=(const Seconds& other) const;
    bool operator>(const Seconds& other) const;
    bool operator>=(const Seconds& other) const;
    bool operator<(const Seconds& other) const;
    bool operator<=(const Seconds& other) const;
    explicit operator Microseconds() const;
    explicit operator Milliseconds() const;
    Seconds operator+(Seconds other) const;
    Seconds operator-(Seconds other) const;
    Seconds operator*(double scalar) const;
    operator double() const;
};

/// User defined literals
// Floating point literals
// Allows the user to simply write time like: 10.5_us
inline Microseconds operator"" _us(long double val)
{
    return Microseconds{static_cast<double>(val)};
}

// Allows the user to simply write time like: 10.5_ms
inline Milliseconds operator"" _ms(long double val)
{
    return Milliseconds{static_cast<double>(val)};
}

// Allows the user to simply write time like: 10.5_s
inline Seconds operator"" _s(long double val)
{
    return Seconds{static_cast<double>(val)};
}

// Integer literals
// Allows the user to simply write time like: 10_us
inline Microseconds operator"" _us(unsigned long long val)
{
    return Microseconds{static_cast<double>(val)};
}

// Allows the user to simply write time like: 10_ms
inline Milliseconds operator"" _ms(unsigned long long val)
{
    return Milliseconds{static_cast<double>(val)};
}

// Allows the user to simply write time like: 10_s
inline Seconds operator"" _s(unsigned long long val)
{
    return Seconds{static_cast<double>(val)};
}
