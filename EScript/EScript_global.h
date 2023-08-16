/**
 * @file escript_global.h
 */
#ifndef ESCRIPT_GLOBAL_H
#define ESCRIPT_GLOBAL_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(ESCRIPT_LIBRARY)
#  define ESCRIPT_EXPORT Q_DECL_EXPORT
#else
#  define ESCRIPT_EXPORT Q_DECL_IMPORT
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#if !defined(ENVIRONMENT64)
#error 64bit platform is required
#endif

#include <cstring>
#include <codecvt>
#include <locale>

namespace escript {

#if __cpp_lib_bit_cast
#else
// https://en.cppreference.com/w/cpp/numeric/bit_cast

template<class To, class From>
std::enable_if_t<
    sizeof(To) == sizeof(From) &&
    std::is_trivially_copyable_v<From> &&
    std::is_trivially_copyable_v<To>,
    To>
// constexpr support needs compiler magic
bit_cast(const From& src) noexcept
{
    static_assert(std::is_trivially_constructible_v<To>,
        "This implementation additionally requires "
        "destination type to be trivially constructible");

    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}
#endif

/**
 * Тип данных, равный размеру указателя
 */
using PtrIntType = uint64_t;

template<typename T = std::u32string>
std::string to_utf8(const T &str)
{
    std::wstring_convert<std::codecvt_utf8<typename T::value_type>, typename T::value_type> convert;
    return convert.to_bytes(str);
}

template<typename T = int64_t>
std::string to_utf8(int64_t i)
{
    return std::to_string(i);
}


} // namespace escript

#endif // ESCRIPT_GLOBAL_H
