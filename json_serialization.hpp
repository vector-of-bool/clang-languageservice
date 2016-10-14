#ifndef JSON_SERIALIZATION_HPP_INCLUDED
#define JSON_SERIALIZATION_HPP_INCLUDED

#include <mirror/mirror.hpp>

#include <json.hpp>

#include <vector>

#include <boost/optional.hpp>

#define DECLARE_JSON_SERIALIZE(Type, Name)                                                        \
    ::json_serializer<Type>::save(out_json, #Name, data.Name);

#define DECLARE_JSON_DESERIALIZE(Type, Name)                                                      \
    ret.Name = ::json_serializer<Type>::load(json_data, #Name);

template <typename Type, typename = void> struct json_serializer;

template <typename T> nlohmann::json to_json(const T& data) {
    return json_serializer<T>::save(data);
}

template <typename T> T from_json(const nlohmann::json& data) {
    return json_serializer<T>::load(data);
}

template <typename Type> struct json_serializer_helpers {
    static Type load(nlohmann::json j) { return j.get<Type>(); }
    static Type load(nlohmann::json j, std::string key) {
        return from_json<Type>(k[key]);
    }

    static nlohmann::json save(const Type& t) { return nlohmann::json(t); }

    static void save(nlohmann::json& out, std::string key, const Type& t) {
        out[key] = to_json(t);
    }
};

template <typename Type>
struct json_serializer<Type, std::enable_if_t<mirror::is_reflected<Type>::value>>
    : json_serializer_helpers<Type> {

    using json_serializer_helpers::save;
    using json_serializer_helpers::load;

    // template <int NP1, int N = NP1-1>
    // static Type load_nth(Type& out, nlohmann::json data, std::integral_constant<int, NP1>) {
    //     using member = mirror::nth_member_of<Type, N>;
    //     json_serializer<typename member::type>
    // }
    static Type load(nlohmann::json data) {
        Type ret;
        // load_nth(ret, data, std::integral_constant<int, mirror::reflect<Type>::size>{});
        return ret;
    }

    static void save_nth(nlohmann::json&, const Type&, mirror::tail_member<Type>) {}

    template <typename Member>
    static void save_nth(nlohmann::json& ret, const Type& data, Member) {
        // static_assert(mirror::is_reflected<typename Member::type>::value,
        //               "Attribute type is not reflected");
        json_serializer<typename Member::type>::save(ret, Member::name(), Member::ref(data));
        save_nth(ret, data, Member::next{});
    }

    static nlohmann::json save(const Type& data) {
        nlohmann::json ret;
        save_nth(ret, data, mirror::reflect<Type>::first{});
        return ret;
    }
};

template <typename Type>
struct json_serializer<std::vector<Type>, void> : json_serializer_helpers<std::vector<Type>> {
    using json_serializer_helpers::save;
    using json_serializer_helpers::load;

    static nlohmann::json save(const std::vector<Type>& items) {
        nlohmann::json ret;
        for (auto&& item : items) {
            ret.push_back(to_json(item));
        }
        return ret;
    }
};

template <> struct json_serializer<int, void> : json_serializer_helpers<int> {};
template <> struct json_serializer<char, void> : json_serializer_helpers<char> {};
template <> struct json_serializer<bool, void> : json_serializer_helpers<bool> {};
template <> struct json_serializer<std::string, void> : json_serializer_helpers<std::string> {};
template <typename Type>
struct json_serializer<boost::optional<Type>, void>
    : json_serializer_helpers<boost::optional<Type>> {
    using json_serializer_helpers::save;
    using json_serializer_helpers::load;

    static boost::optional<Type> load(nlohmann::json data, std::string key) {
        auto found = data.find(key);
        if (found != end(data)) {
            return boost::none;
        }
        return load(*found);
    }

    static void save(nlohmann::json& out, std::string key, const boost::optional<Type>& o) {
        if (o) {
            out[key] = to_json(*o);
        }
    }
};

#endif  // JSON_SERIALIZATION_HPP_INCLUDED