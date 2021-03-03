#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

class RequestHandler
{
    const char host[25] = "utahavalanchecenter.org";
    const string port = "443";
    const string target;
    int version = 11;

    // The io_context is required for all I/O
    boost::asio::io_context ioc;

    // This buffer is used for reading and must be persisted
    boost::beast::flat_buffer buffer;

    // Declare container to hold the response
    http::response<http::dynamic_body> res;

    //declare a request object to hold request
    http::request<http::string_body> request;

    void set_sni_hostname(ssl::stream<tcp::socket>& stream);

    void establish_connection(ssl::stream<tcp::socket>& stream, const boost::asio::ip::basic_resolver_results<class boost::asio::ip::tcp> results);

    http::request<http::string_body> configure_get_request();

    void verify_error_code(boost::system::error_code& ec);

    public:
        RequestHandler(const string location);

        std::string get_response_body();
};

#endif /*REQUEST_HANDLER_H*/