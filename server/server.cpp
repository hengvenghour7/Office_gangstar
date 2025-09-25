#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <set>
#include <mutex>

using boost::asio::ip::tcp;

// Forward declare
struct Session;
using SessionPtr = std::shared_ptr<Session>;
int playerId = 1;

// Store all active clients
std::set<SessionPtr> clients;
std::mutex clients_mutex;

// Each client session
struct Session : std::enable_shared_from_this<Session> {
    tcp::socket socket;
    char data[1024];

    Session(boost::asio::io_context& io) : socket(io) {}

    void start() {
        doRead();
    }

    void doRead() {
        auto self = shared_from_this();
        socket.async_read_some(
            boost::asio::buffer(data, 1024),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string msg(data, length);
                    std::cout << "Client says: " << msg << "\n";
                    broadcast(msg, self);
                    doRead(); // keep reading
                } else {
                    // remove this client if disconnected
                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients.erase(self);
                }
            }
        );
    }

    void send(const std::string& msg) {
        auto self = shared_from_this();
        boost::asio::async_write(
            socket,
            boost::asio::buffer(msg),
            [self](boost::system::error_code, std::size_t) {
                // ignore errors here for simplicity
            }
        );
    }

    // Send message to everyone except the sender
    static void broadcast(const std::string& msg, SessionPtr sender) {
        std::lock_guard<std::mutex> lock(clients_mutex);
        for (auto& client : clients) {
            if (client != sender) {
                client->send(msg);
            }
        }
    }
};

// Accept new clients
void doAccept(tcp::acceptor& acceptor, boost::asio::io_context& io) {
    auto session = std::make_shared<Session>(io);
    acceptor.async_accept(session->socket, 
        [session, &acceptor, &io](boost::system::error_code ec) {
            if (!ec) {
                {
                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients.insert(session);
                }
                session->start();
                std::cout << "New connection!\n";
            }
            doAccept(acceptor, io); // keep accepting next client
        }
    );
}

int main() {
    try {
        boost::asio::io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 197));

        doAccept(acceptor, io);

        io.run(); // event loop
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << "\n";
    }
}
