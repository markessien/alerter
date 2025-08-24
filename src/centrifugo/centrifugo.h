
#ifndef CENTRIFUGO_CLIENT_H
#define CENTRIFUGO_CLIENT_H

#include <string>
#include <memory>
#include <thread>
#include <atomic>

#include <grpcpp/grpcpp.h>
#include "unistream.grpc.pb.h"

// Forward declaration to avoid including grpc::ClientReader in the header
namespace grpc {
    template <class R>
    class ClientReader;
}

class CentrifugoClient {
public:
    // Constructor and Destructor
    CentrifugoClient();
    ~CentrifugoClient();

    /**
     * @brief Connects to the Centrifugo unidirectional stream and starts listening.
     * @param server_address The address of the Centrifugo server (e.g., "localhost:11000").
     * @param jwt_token The JWT for authentication.
     * @return True if the connection was successfully established, false otherwise.
     */
    bool Connect(const std::string& server_address, const std::string& auth_token);

    /**
     * @brief Disconnects from the stream and stops the listening thread.
     */
    void Disconnect();

    /**
     * @brief Checks if the client is currently connected and listening.
     * @return True if connected, false otherwise.
     */
    bool IsConnected() const;

private:
    /**
     * @brief The main loop that runs on a separate thread to read messages from the stream.
     */
    void ListenLoop();

    /**
     * @brief A callback-like function to process incoming push messages.
     * @param push The received push message.
     */
    // Corrected the namespace for the Push message type
    void HandlePush(const centrifugal::centrifugo::unistream::Push& push);

    // --- Member Variables ---

    // Atomic flag to control the listening loop's execution
    std::atomic<bool> is_running_{false};

    // The thread that runs the ListenLoop
    std::thread listener_thread_;

    // gRPC-related objects
    std::unique_ptr<grpc::ClientContext> context_;
    std::shared_ptr<grpc::Channel> channel_;
    
    // Corrected the service name to CentrifugoUniStream and its namespace
    std::unique_ptr<centrifugal::centrifugo::unistream::CentrifugoUniStream::Stub> stub_;

    // Corrected the namespace for the Push type in the ClientReader
    std::unique_ptr<grpc::ClientReader<centrifugal::centrifugo::unistream::Push>> stream_;
};

#endif // CENTRIFUGO_CLIENT_H

