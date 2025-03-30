#include "time.hpp"


Microseconds::Microseconds(double val) : mValue(val) {}

bool Microseconds::operator==(const Microseconds& other) const
{
    return mValue == other.mValue;
}

bool Microseconds::operator!=(const Microseconds& other) const
{
    return mValue != other.mValue;
}

bool Microseconds::operator>(const Microseconds& other) const
{
    return mValue > other.mValue;
}

bool Microseconds::operator>=(const Microseconds& other) const
{
    return mValue >= other.mValue;
}

bool Microseconds::operator<(const Microseconds& other) const
{
    return mValue < other.mValue;
}

bool Microseconds::operator<=(const Microseconds& other) const
{
    return mValue <= other.mValue;
}

Microseconds::operator Milliseconds() const
{
    return Milliseconds{mValue / 1000.0f};
}

Microseconds::operator Seconds() const
{
    return Seconds{mValue / 1000000.0f};
}

Microseconds Microseconds::operator+(Microseconds other) const
{
    return Microseconds(mValue + other.mValue);
}

Microseconds Microseconds::operator-(Microseconds other) const
{
    return Microseconds(mValue - other.mValue);
}

Microseconds Microseconds::operator*(double scalar) const
{
    return Microseconds(mValue * scalar);
}


Microseconds::operator double() const
{
    return mValue;
}


Milliseconds::Milliseconds(double val) : mValue(val) {}

bool Milliseconds::operator==(const Milliseconds& other) const
{
    return mValue == other.mValue;
}

bool Milliseconds::operator!=(const Milliseconds& other) const
{
    return mValue != other.mValue;
}

bool Milliseconds::operator>(const Milliseconds& other) const
{
    return mValue > other.mValue;
}

bool Milliseconds::operator>=(const Milliseconds& other) const
{
    return mValue >= other.mValue;
}

bool Milliseconds::operator<(const Milliseconds& other) const
{
    return mValue < other.mValue;
}

bool Milliseconds::operator<=(const Milliseconds& other) const
{
    return mValue <= other.mValue;
}

Milliseconds::operator Microseconds() const
{   
    return Microseconds{mValue * 1000.0f};
}

Milliseconds::operator Seconds() const
{
    return Seconds{mValue / 1000.0f};
}

Milliseconds Milliseconds::operator+(Milliseconds other) const
{
    return Milliseconds(mValue + other.mValue);
}

Milliseconds Milliseconds::operator-(Milliseconds other) const
{
    return Milliseconds(mValue - other.mValue);
}

Milliseconds Milliseconds::operator*(double scalar) const
{
    return Milliseconds(mValue * scalar);
}

Milliseconds::operator double() const
{
    return mValue;
}


Seconds::Seconds(double val): mValue(val) {}

bool Seconds::operator==(const Seconds& other) const
{
    return mValue == other.mValue;
}

bool Seconds::operator!=(const Seconds& other) const
{
    return mValue != other.mValue;
}

bool Seconds::operator>(const Seconds& other) const
{
    return mValue > other.mValue;
}

bool Seconds::operator>=(const Seconds& other) const
{
    return mValue >= other.mValue;
}

bool Seconds::operator<(const Seconds& other) const
{
    return mValue < other.mValue;
}

bool Seconds::operator<=(const Seconds& other) const
{
    return mValue <= other.mValue;
}

Seconds::operator Microseconds() const
{
    return Microseconds{mValue * 1000000.0f};
}

Seconds::operator Milliseconds() const
{
    return Milliseconds{mValue * 1000.0f};
}

Seconds Seconds::operator+(Seconds other) const
{
    return Seconds(mValue + other.mValue);
}

Seconds Seconds::operator-(Seconds other) const
{
    return Seconds(mValue - other.mValue);
}

Seconds Seconds::operator*(double scalar) const
{
    return Seconds(mValue * scalar);
}

Seconds::operator double() const
{
    return mValue;
}
