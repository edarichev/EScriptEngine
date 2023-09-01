#ifndef COMPARE_H
#define COMPARE_H

namespace escript {

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
        double d2 = bit_cast<double>(doubleAsUInt64);
        uint64_t ud1 = bit_cast<uint64_t>(d1);
        // в эти случаи включается NaN, Infinity и прочее:
        if (ud1 == doubleAsUInt64)
            return true;
        // остальное - с погрешностью
        return equals_double(d1, d2);
    }

    static bool equals_bool(bool b, uint64_t boolAsUInt64)
    {
        return b == boolAsUInt64 ? true : false;
    }

    static bool equals_string(const std::u32string &s, uint64_t data)
    {
        return s == ((StringObject*)data)->toString();
    }
};

} // namespace escript

#endif // COMPARE_H
