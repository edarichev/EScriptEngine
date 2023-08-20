#ifndef COMPARE_H
#define COMPARE_H


class Compare
{
public:
    static bool equals_int64(int64_t d1, uint64_t raw)
    {
        return d1 == escript::bit_cast<int64_t>(raw);
    }

    static bool equals_double(double d1, double d2)
    {
        return std::abs(d1 - d2) <= std::numeric_limits<double>::epsilon()*100;
    }

    static bool equals_double(double d1, uint64_t doubleAsUInt64)
    {
        double d2 = escript::bit_cast<double>(doubleAsUInt64);
        return std::abs(d1 - d2) <= std::numeric_limits<double>::epsilon()*100;
    }

    static bool equals_bool(bool b, uint64_t boolAsUInt64)
    {
        return b == boolAsUInt64 ? true : false;
    }
};

#endif // COMPARE_H
