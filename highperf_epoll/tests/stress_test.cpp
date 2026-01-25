#include <asio.hpp>
#include <thread>
#include <vector>
#include <iostream>
#include <string>
#include <random>
#include "../include/protocol.hpp"

std::atomic<int> g_sent{0};
std::atomic<int> g_errors{0};

void run_client(const std::string& host, unsigned short port, bool valid) {
    try {
        asio::io_context ioc;
        asio::ip::tcp::socket socket(ioc);
        asio::ip::tcp::resolver resolver(ioc);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        asio::connect(socket, endpoints);

        std::string body;
        if (valid) {
            int id = ++g_sent;
            std::string name = "User" + std::to_string(id);
            body = name + " " + std::to_string(id) + " " + name + "@example.com";
        } else {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> dis(0, 2);
            switch (dis(gen)) {
                case 0: body = "OnlyTwoFields 123"; break;
                case 1: body = "user zero user@test.com"; break;
                case 2: body = "user 0 user@test.com"; break;
            }
        }

        if (body.empty()) return;

        MessageHeader hdr;
        hdr.magic = htonl(PROTOCOL_MAGIC);
        hdr.length = htonl(static_cast<uint32_t>(body.size()));
        hdr.type = htons(MSG_USER_TEXT);
        hdr.reserved = 0;

        std::vector<char> buf(sizeof(MessageHeader) + body.size());
        std::memcpy(buf.data(), &hdr, sizeof(hdr));
        std::memcpy(buf.data() + sizeof(hdr), body.data(), body.size());

        asio::write(socket, asio::buffer(buf));
        
        // ✅ Ensure data is sent before closing
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } catch (...) {
        ++g_errors;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: stress_test <host> <port> <num_clients>\n";
        return 1;
    }

    std::string host = argv[1];
    unsigned short port = static_cast<unsigned short>(std::stoi(argv[2]));
    int total = std::stoi(argv[3]);

    std::cout << "Starting " << total << " clients...\n";

    std::vector<std::thread> threads;
    const int batch = 50;
    for (int i = 0; i < total; ++i) {
        bool valid = (i == 0) ? true : (i % 10 != 0);
        threads.emplace_back(run_client, host, port, valid);
        if (threads.size() >= batch || i == total - 1) {
            for (auto& t : threads) t.join();
            threads.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

    std::cout << "Done. Sent valid: " << g_sent.load()
              << ", Errors: " << g_errors.load() << "\n";
    return 0;
}
