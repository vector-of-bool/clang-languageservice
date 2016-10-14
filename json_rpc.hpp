#ifndef CLS_JSON_RPC_HPP_INCLUDED
#define CLS_JSON_RPC_HPP_INCLUDED

#include <functional>
#include <boost/thread/future.hpp>

#include <json.hpp>

namespace json_rpc
{

using nlohmann::json;
using std::string;

using method_handler = std::function<boost::future<json>(const string&, const json&)>;

}

#endif // CLS_JSON_RPC_HPP_INCLUDED