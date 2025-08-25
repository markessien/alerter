#ifndef TELEX_H
#define TELEX_H

#include <string>
#include <vector>
#include <map>
#include <ixwebsocket/IXWebSocket.h>
#include <wx/thread.h>
#include <wx/event.h>
#include "job_queue.h"

struct Organisation {
    std::string id;
    std::string name;
};

class Telex : public wxThread {
public:
    Telex(wxEvtHandler* pParent);
    ~Telex();
    void login(const std::string& email, const std::string& password);
    void Exit();
    std::vector<Organisation> getOrganisations();

protected:
    virtual ExitCode Entry();

private:
    bool doLogin(const std::string& email, const std::string& password);
    std::string getConnectionToken();
    std::string getSubscriptionToken(const std::string& channel);
    void fetchUserOrganisations();

    void connectAndSubscribeViaWebSocket(
        const std::string& connectionToken,
        const std::map<std::string, std::string>& subscriptions
    );

    wxEvtHandler* m_pParent;
    std::string base_url;
    std::string notification_token_;
    std::string access_token_;
    std::string user_id_;
    std::vector<Organisation> organisations_;
    uint32_t command_id_ = 1;
    ix::WebSocket webSocket_;
    JobQueue m_queue;
};

#endif // TELEX_H