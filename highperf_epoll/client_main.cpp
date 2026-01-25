#include <asio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "include/protocol.hpp"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: client <host> <port> <username>\n";
        return 1;
    }

    std::string username = argv[3];
    std::string body = username + " 12345 " + username + "@example.com";

    if (body.size() > MAX_MESSAGE_BODY_SIZE - 1) {
        std::cerr << "Message too long\n";
        return 1;
    }

    asio::io_context ioc;
    asio::ip::tcp::socket socket(ioc);
    asio::ip::tcp::resolver resolver(ioc);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    try {
        asio::connect(socket, endpoints);
    } catch (const std::system_error& e) {
        std::cerr << "Failed to connect to server: " << e.what() << "\n";
        std::cerr << "Is the server running on 127.0.0.1:8080?\n";
        return 1;
    }

    MessageHeader hdr;
    hdr.magic = htonl(PROTOCOL_MAGIC);
    hdr.length = htonl(static_cast<uint32_t>(body.size()));
    hdr.type = htons(MSG_USER_TEXT);
    hdr.reserved = 0;

    std::vector<char> buf(sizeof(MessageHeader) + body.size());
    std::memcpy(buf.data(), &hdr, sizeof(hdr));
    std::memcpy(buf.data() + sizeof(hdr), body.data(), body.size());

    asio::write(socket, asio::buffer(buf));
    return 0;
}
