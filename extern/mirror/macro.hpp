#ifndef MIRRORPP_MIRROR_MACRO_HPP_INCLUDED
#define MIRRORPP_MIRROR_MACRO_HPP_INCLUDED

#include <boost/preprocessor.hpp>

#include <type_traits>

#include <mirror/traits_base.hpp>

namespace mirror
{

namespace detail
{

struct dummy;

} /* detail */

} /* mirror */

#define MIRRORPP_DETAIL_CHECK_IN_GLOBAL_NS                                     \
    namespace mirror                                                           \
    {                                                                          \
    namespace detail                                                           \
    {                                                                          \
    struct dummy;                                                              \
    static_assert(std::is_same<dummy, ::mirror::detail::dummy>::value,         \
                  "This macro should only be used at the global namespace");   \
    }                                                                          \
    }                                                                          \
    static_assert(true, "")

#define MIRRORPP_DETAIL_OPEN_NAMESPACE                                         \
    namespace mirror                                                           \
    {                                                                          \
    namespace detail                                                           \
    {                                                                          \
    static_assert(true, "")

#define MIRRORPP_DETAIL_CLOSE_NAMESPACE                                        \
    }                                                                          \
    }                                                                          \
    static_assert(true, "")

#define MIRRORPP_DETAIL_REFLECT_BEGIN(reflected_type, nmembers)                \
    template <> struct basic_type_info<reflected_type>                         \
    {                                                                          \
        using type = reflected_type;                                           \
        enum                                                                   \
        {                                                                      \
            num_members = nmembers                                             \
        };                                                                     \
        static const char* name()                                              \
        {                                                                      \
            return BOOST_PP_STRINGIZE(reflected_type);                         \
        }                                                                      \
    }

#define MIRRORPP_DETAIL_DECL_MEMBER_TRAITS(nth, parent_type, member)           \
    template <> struct nth_member_info<parent_type, nth - 2>                   \
    {                                                                          \
        enum                                                                   \
        {                                                                      \
            index = nth - 2                                                    \
        };                                                                     \
        using owner_type = parent_type;                                        \
        using type = decltype(std::declval<parent_type>().member);             \
        static type& ref(parent_type& this_) { return this_.member; }          \
        static const type& ref(parent_type const& this_)                       \
        {                                                                      \
            return this_.member;                                               \
        }                                                                      \
        static volatile type& ref(parent_type volatile& this_)                 \
        {                                                                      \
            return this_.member;                                               \
        }                                                                      \
        static const volatile type& ref(parent_type const volatile& this_)     \
        {                                                                      \
            return this_.member;                                               \
        }                                                                      \
        static const char* name() { return BOOST_PP_STRINGIZE(member); }       \
        using next = typename std::                                            \
            conditional<index + 1                                              \
                            == basic_type_info<parent_type>::num_members,      \
                        tail_member<parent_type>,                              \
                        detail::nth_member_info<parent_type,                   \
                                                index + 1>>::type;             \
    };

#define MIRRORPP_REFLECT(type, items)                                          \
    MIRRORPP_DETAIL_CHECK_IN_GLOBAL_NS;                                        \
    MIRRORPP_DETAIL_OPEN_NAMESPACE;                                            \
    MIRRORPP_DETAIL_REFLECT_BEGIN(type, BOOST_PP_SEQ_SIZE(items));             \
    BOOST_PP_SEQ_FOR_EACH(MIRRORPP_DETAIL_DECL_MEMBER_TRAITS, type, items)     \
    MIRRORPP_DETAIL_CLOSE_NAMESPACE;                                           \
    static_assert(true, "")


#endif  // MIRRORPP_MIRROR_MACRO_HPP_INCLUDED
