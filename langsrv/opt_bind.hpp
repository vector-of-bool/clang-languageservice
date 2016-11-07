#ifndef LANGSRV_OPT_BIND_HPP_INCLUDED
#define LANGSRV_OPT_BIND_HPP_INCLUDED

#include <boost/optional.hpp>

namespace langsrv {

namespace optional_bind_op {

using boost::optional;
using boost::none;

namespace detail {

template <typename Result> struct bind_helper {
    using type = optional<Result>;

    template <typename T, typename Func> static type bind(optional<T> opt, Func&& fn) {
        if (opt) {
            std::forward<Func>(fn)(std::move(*opt));
        } else {
            return none;
        }
    }
};

template <> struct bind_helper<void> {
    using type = void;

    template <typename T, typename Func> static type bind(optional<T> opt, Func&& fn) {
        if (opt) {
            std::forward<Func>(fn)(std::move(*opt));
        }
    }
};

template <typename Result> struct bind_helper<optional<Result>> {
    using type = optional<Result>;

    template <typename T, typename Func> static type bind(optional<T> opt, Func&& fn) {
        if (opt) {
            auto intermediate = std::forward<Func>(fn)(std::move(*opt));
            if (intermediate) {
                return *intermediate;
            } else {
                return none;
            }
        } else {
            return none;
        }
    }
};

template <typename Func, typename T>
using bind_helper_t = bind_helper<decltype(std::declval<Func>()(std::declval<T>()))>;
}

template <typename T, typename Func>
auto operator|(optional<T> opt, Func&& fn) -> typename detail::bind_helper_t<Func, T>::type {
    return detail::bind_helper_t<Func, T>::bind(opt, fn);
}

} // optional_bind_op
} // langsrv


#endif  // LANGSRV_OPT_BIND_HPP_INCLUDED