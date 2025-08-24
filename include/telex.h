#ifndef TELEX_H
#define TELEX_H

#include <string>
#include <vector>
#include <map>
#include <ixwebsocket/IXWebSocket.h>

struct Organisation {
    std::string id;
    std::string name;
};

class Telex {
public:
    Telex();
    bool login(const std::string& email, const std::string& password);
    std::string getConnectionToken();
    std::string getSubscriptionToken(const std::string& channel);
    std::vector<Organisation> getUserOrganisations();
    void testTelex();

private:
    std::string base_url;
    std::string notification_token_;
    std::string access_token_;
    std::string user_id_;

    void connectAndSubscribeViaWebSocket(
        const std::string& connectionToken,
        const std::map<std::string, std::string>& subscriptions
    );

    ix::WebSocket webSocket_;
};

#endif // TELEX_H