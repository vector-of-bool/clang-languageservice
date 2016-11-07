#ifndef LANGSRV_SERIALIZE_HPP_INCLUDED
#define LANGSRV_SERIALIZE_HPP_INCLUDED

#include <mirror/mirror.hpp>

#include <json.hpp>

#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace langsrv {

using nlohmann::json;
using std::string;
using std::vector;
using boost::optional;

#define DECLARE_JSON_SERIALIZE(Type, Name)                                                        \
    ::langsrv::serializer<Type>::save(out_json, #Name, data.Name);

#define DECLARE_JSON_DESERIALIZE(Type, Name)                                                      \
    ret.Name = ::langsrv::serializer<Type>::load(json_data, #Name);

template <typename Type, typename = void> struct serializer;

template <typename T> json to_json(const T& data) { return serializer<T>::save(data); }

template <typename T> T from_json(const json& data) { return serializer<T>::load(data); }

template <typename Type> struct serializer_helpers {
    static Type load(const json& j) { return j.get<Type>(); }
    static Type load(const json& j, string key) { return serializer<Type>::load(j[key]); }

    static json save(const Type& t) { return json(t); }

    static void save(json& out, string key, const Type& t) { out[key] = to_json(t); }
};

template <> struct serializer<json, void> : serializer_helpers<json> {
    using serializer_helpers::save;
    using serializer_helpers::load;

    static json load(const json& j) { return j; }
    static json save(const json& j) { return j; }
};

template <typename Type> struct serializer<vector<Type>, void> : serializer_helpers<vector<Type>> {
    using serializer::serializer_helpers::save;
    using serializer::serializer_helpers::load;

    static json save(const vector<Type>& items) {
        json ret;
        for (auto&& item : items) {
            ret.push_back(to_json(item));
        }
        return ret;
    }
};

template <> struct serializer<int, void> : serializer_helpers<int> {};
template <> struct serializer<char, void> : serializer_helpers<char> {};
template <> struct serializer<bool, void> : serializer_helpers<bool> {};
template <> struct serializer<string, void> : serializer_helpers<string> {};
template <typename Type>
struct serializer<optional<Type>, void> : serializer_helpers<optional<Type>> {

    static optional<Type> load(const json& data) {
        if (data == nullptr) {
            return boost::none;
        } else {
            return from_json<Type>(data);
        }
    }

    static json save(const optional<Type>& opt) {
        if (opt) {
            return to_json(*opt);
        } else {
            return json();
        }
    }

    static optional<Type> load(const json& data, string key) {
        auto found = data.find(key);
        if (found == end(data)) {
            return boost::none;
        }
        return load(*found);
    }

    static void save(json& out, string key, const optional<Type>& o) {
        if (o) {
            out[key] = to_json(*o);
        }
    }
};


template <typename Type>
struct serializer<Type, typename std::enable_if<mirror::is_reflected<Type>::value>::type>
    : serializer_helpers<Type> {

    using serializer::serializer_helpers::save;
    using serializer::serializer_helpers::load;

    static void load_nth(Type&, const json&, mirror::tail_member<Type>) {}

    template <typename Member> static void load_nth(Type& out, const json& data, Member) {
        // member_type::debug_help;
        Member::ref(out) = serializer<typename Member::type>::load(data, Member::name());
        load_nth(out, data, typename Member::next{});
    }
    static Type load(const json& data) {
        Type ret;
        load_nth(ret, data, typename mirror::reflect<Type>::first{});
        return ret;
    }

    static void save_nth(json&, const Type&, mirror::tail_member<Type>) {}

    template <typename Member> static void save_nth(json& ret, const Type& data, Member) {
        // static_assert(mirror::is_reflected<typename Member::type>::value,
        //               "Attribute type is not reflected");
        serializer<typename Member::type>::save(ret, Member::name(), Member::ref(data));
        save_nth(ret, data, typename Member::next{});
    }

    static json save(const Type& data) {
        json ret;
        save_nth(ret, data, typename mirror::reflect<Type>::first{});
        return ret;
    }
};
}

#endif  // LANGSRV_SERIALIZE_HPP_INCLUDED