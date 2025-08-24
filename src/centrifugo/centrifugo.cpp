
#include "centrifugo.h"
#include <iostream>

using namespace centrifugal::centrifugo::unistream;

CentrifugoClient::CentrifugoClient() = default;

CentrifugoClient::~CentrifugoClient() {
    // Ensure disconnection when the object is destroyed
    if (IsConnected()) {
        Disconnect();
    }
}

bool CentrifugoClient::Connect(const std::string& server_address, const std::string& auth_token) {
    if (IsConnected()) {
        std::cerr << "Already connected." << std::endl;
        return false;
    }

    // 1. Create channel and stub
    channel_ = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());

    stub_ = CentrifugoUniStream::NewStub(channel_);

    // 2. Prepare the request
    context_ = std::make_unique<grpc::ClientContext>();
    
    ConnectRequest request;
    request.set_token(auth_token);
 
    // Add any existing subscriptions to the request
    {
        std::lock_guard<std::mutex> lock(subs_mutex_);
        for (const auto& pair : subscriptions_) {
            (*request.mutable_subs())[pair.first] = pair.second;
        }
    }
 
    // 3. Initiate the stream
    stream_ = stub_->Consume(context_.get(), request);

    if (!stream_) {
        std::cerr << "Failed to create stream. Check server address and logs." << std::endl;
        return false;
    }

    std::cout << "Stream successfully established. Starting listener thread..." << std::endl;

    // 4. Start the listening thread
    is_running_ = true;
    listener_thread_ = std::thread(&CentrifugoClient::ListenLoop, this);

    return true;
}

void CentrifugoClient::Disconnect() {
    if (!IsConnected()) {
        std::cerr << "Not connected." << std::endl;
        return;
    }

    std::cout << "Disconnecting..." << std::endl;

    // 1. Signal the listening loop to stop
    is_running_ = false;

    // 2. Cancel the context. This will interrupt the blocking stream_->Read() call
    if (context_) {
        context_->TryCancel();
    }

    // 3. Wait for the listener thread to finish its execution
    if (listener_thread_.joinable()) {
        listener_thread_.join();
    }

    // The stream_ and other unique_ptrs will be cleaned up automatically.
    std::cout << "Disconnected." << std::endl;
}

bool CentrifugoClient::IsConnected() const {
    return is_running_.load();
}
 
bool CentrifugoClient::Subscribe(const std::string& channel) {
    if (IsConnected()) {
        std::cerr << "Cannot subscribe while connected. Please subscribe before connecting." << std::endl;
        return false;
    }
 
    std::lock_guard<std::mutex> lock(subs_mutex_);
    subscriptions_[channel] = centrifugal::centrifugo::unistream::SubscribeRequest();
    std::cout << "Channel '" << channel << "' marked for subscription." << std::endl;
    return true;
}
 
void CentrifugoClient::ListenLoop() {
    Push push;
    // This loop continues as long as the stream is valid and we are in a "running" state
    while (is_running_ && stream_->Read(&push)) {
        HandlePush(push);
    }

    // If the loop exits, the stream has either been closed by the server or an error occurred.
    grpc::Status status = stream_->Finish();
    std::cerr << "Stream finished. Reason: " << status.error_message() << std::endl;
    
    // Ensure the state is updated
    is_running_ = false;
}


void CentrifugoClient::HandlePush(const Push& push) {
    // Check for each possible push type using has_*() methods
    if (push.has_connect()) {
        std::cout << "[Push] Connected with Client ID: " << push.connect().client() << std::endl;
    } 
    else if (push.has_pub()) {
        std::cout << "[Push] Publication from '" << push.channel() << "': " 
                  << push.pub().data() << std::endl;
    } 
    else if (push.has_join()) {
        std::cout << "[Push] Client joined '" << push.channel() 
                  << "' (User: " << push.join().info().user() << ")" << std::endl;
    } 
    else if (push.has_leave()) {
        std::cout << "[Push] Client left '" << push.channel() 
                  << "' (User: " << push.leave().info().user() << ")" << std::endl;
    } 
    else {
        std::cout << "[Push] Received a push with no known data type." << std::endl;
    }
}
