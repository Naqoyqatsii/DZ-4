#include <iostream>
#include <type_traits>
#include <vector>
#include <list>
#include <tuple>
#include <string>

template <typename T, typename = void>
struct is_container : std::false_type {};

template <typename T>
struct is_container<T, decltype(std::declval<T>().begin(), void(), std::declval<T>().end())> : std::true_type {};

template <typename T>
constexpr bool is_container_v = is_container<T>::value;
template <typename T, typename Tuple = void>
struct is_homogeneous_tuple : std::false_type {};

template <typename T, typename... Ts>
struct is_homogeneous_tuple<T, std::tuple<Ts...>> : std::conjunction<std::is_same<T, Ts>...> {};

template <typename T, typename Tuple>
constexpr bool is_homogeneous_tuple_v = is_homogeneous_tuple<T, Tuple>::value;
template <typename T>
std::enable_if_t<std::is_integral_v<T>> print_ip(const T &value)
{
    for (int i = sizeof(T) - 1; i >= 0; --i)
    {
        std::cout << (i < sizeof(T) - 1 ? "." : "") << (static_cast<int>(value >> (i * 8)) & 0xFF);
    }
    std::cout << std::endl;
}

template <typename T>
std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string>> print_ip(const T &value)
{
    std::cout << value << std::endl;
}

template <typename T>
std::enable_if_t<is_container_v<std::decay_t<T>>> print_ip(const T &value)
{
    auto it = value.begin();
    while (it != value.end())
    {
        std::cout << *it;
        it++;
        std::cout << (it != value.end() ? "." : "");
    }
    std::cout << std::endl;
}

template <typename T, typename... Ts>
std::enable_if_t<is_homogeneous_tuple_v<T, std::tuple<Ts...>>> print_ip(const std::tuple<Ts...> &value)
{
    std::apply([](const auto &...values)
               { ((std::cout << values << '.'), ...); },
               value);
    std::cout << '\b' << ' ' << std::endl; // Remove trailing dot.
}

int main()
{
    print_ip(int8_t{-1});
    print_ip(int16_t{0});
    print_ip(int32_t{2130706433});
    print_ip(int64_t{8875824491850138409});
    print_ip(std::string{"Hello, World!"});
    print_ip(std::vector<int>{100, 200, 300, 400});
    print_ip(std::list<short>{400, 300, 200, 100});
    print_ip(std::make_tuple(123, 456, 789, 0));
    return 0;
}