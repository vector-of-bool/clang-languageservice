#ifndef MIRRORPPP_MIRROR_TRAITS_BASE_HPP_INCLUDED
#define MIRRORPPP_MIRROR_TRAITS_BASE_HPP_INCLUDED

#include <type_traits>

namespace mirror
{

template <typename T> struct tail_member
{
};

template <typename... Info> struct member_list
{
};

namespace detail
{

template <typename T> struct basic_type_info;

template <typename T, std::size_t N> struct nth_member_info;

template <typename> struct just_void
{
    using type = void;
};


template <std::size_t... Is> struct index_sequence
{
    using type = index_sequence;
};

template <typename, typename> struct make_member_list_t;

template <typename T, std::size_t... Is>
struct make_member_list_t<T, index_sequence<Is...>>
{
    using type = member_list<nth_member_info<T, Is>...>;
};

template <typename A, typename B> struct join_t;

template <std::size_t... As, std::size_t... Bs>
struct join_t<index_sequence<As...>, index_sequence<Bs...>>
    : public index_sequence<As..., (Bs + sizeof...(As))...>
{
};

template <typename A, typename B> using join = typename join_t<A, B>::type;

template <std::size_t N>
struct make_index_sequence_t : public join<typename make_index_sequence_t<N / 2>::type,
                                           typename make_index_sequence_t<N - N / 2>::type>
{
};

template <>
struct make_index_sequence_t<0> : index_sequence<>
{
};
template <> struct make_index_sequence_t<1> : index_sequence<0>
{
};

template <std::size_t N>
using make_index_sequence = typename make_index_sequence_t<N>::type;

template <typename R>
using make_member_list =
    typename make_member_list_t<typename R::type, make_index_sequence<R::size>>::type;

template <typename = void> struct delayed_false
{
    enum
    {
        value = false
    };
};

template <typename T> using void_t = typename just_void<T>::type;

template <typename T, bool B> struct reflect
{
};

template <typename T> struct reflect<T, true>
{
private:
    using _bti = basic_type_info<T>;

public:
    using type = typename _bti::type;
    enum
    {
        size = _bti::num_members
    };

    template <std::size_t N>
    using nth_member =
        typename std::conditional<N == size,
                                  tail_member<T>,
                                  nth_member_info<type, N>>::type;
    using first = nth_member<0>;

    using member_list = typename detail::make_member_list<reflect<T, true>>;
};

template <typename, typename = void>
struct is_reflected_helper : public std::false_type
{
};

template <typename T>
struct is_reflected_helper<T, void_t<typename basic_type_info<T>::type>>
    : public std::true_type
{
};

} /* detail */

template <typename T>
struct is_reflected : public detail::is_reflected_helper<T>
{
};

template <typename T>
struct reflect : public detail::reflect<T, is_reflected<T>::value>
{
    static_assert(is_reflected<T>::value,
                  "mirror::reflect<T> was applied to a type which hasn't yet "
                  "been reflected using MIRROR_REFLECT.");
};

template <typename T, std::size_t N>
using nth_member_of = typename reflect<T>::template nth_member<N>;
template <typename T, std::size_t N>
using nth_type = typename reflect<T>::template nth_member<N>::type;

#ifdef MIRRORPP_HAVE_VARIABLE_TEMPLATES
template <typename T> constexpr std::size_t size_of = reflect<T>::size;
template <typename T, std::size_t N>
const char* nth_name = reflect<T>::template nth_member<N>::name();
#endif

} /* mirror */

#endif  // MIRRORPPP_MIRROR_TRAITS_BASE_HPP_INCLUDED
