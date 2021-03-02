#include "RequestHandler.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

RequestHandler::RequestHandler()
{
    try
    {
        ssl::context ctx(ssl::context::sslv23);
        ctx.set_default_verify_paths();

        // These objects perform our I/O
        tcp::resolver resolver{ ioc };
        ssl::stream<tcp::socket> stream{ ioc, ctx };

        // Set SNI Hostname (many hosts need this to handshake successfully)
        this->set_sni_hostname(stream);

        // Look up the domain name
        const boost::asio::ip::basic_resolver_results<class boost::asio::ip::tcp> domain_name = resolver.resolve(host, port);

        // Establish connection/handshake
        establish_connection(stream, domain_name);

        // Set up an HTTP GET request message
        request = configure_get_request();

        // Send the HTTP request to the remote host
        http::write(stream, request);

        // Receive the HTTP response
        http::read(stream, buffer, res);

        // Write the message to standard out
        std::string body{ boost::asio::buffers_begin(res.body().data()),
                   boost::asio::buffers_end(res.body().data()) };

        std::cout << "Body: " << body << "\n";

        // Gracefully close the stream
        boost::system::error_code ec;
        stream.shutdown(ec);

        verify_error_code(ec);

        if (ec)
            throw boost::system::system_error{ ec };

        // If we get here then the connection is closed gracefully
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw "Critical Failure!";
    }
}

void RequestHandler::set_sni_hostname(ssl::stream<tcp::socket>& stream)
{
    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(stream.native_handle(), host))
    {
        boost::system::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
        throw boost::system::system_error{ ec };
    }
}

void RequestHandler::establish_connection(ssl::stream<tcp::socket>& stream, const boost::asio::ip::basic_resolver_results<class boost::asio::ip::tcp> results)
{
    // Make the connection on the IP address we get from a lookup
    boost::asio::connect(stream.next_layer(), results.begin(), results.end());

    // Perform the SSL handshake
    stream.handshake(ssl::stream_base::client);
}

http::request<http::string_body> RequestHandler::configure_get_request()
{
    // Set up an HTTP GET request message
    http::request<http::string_body> req{ http::verb::get, target, version };
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    return req;
}

void RequestHandler::verify_error_code(boost::system::error_code& ec)
{
    if (ec == boost::asio::error::eof || ec == boost::asio::ssl::error::stream_truncated)
    {
        // Rationale:
        // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
        ec.assign(0, ec.category());
    }
}


