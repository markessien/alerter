#ifndef TELEX_H
#define TELEX_H

#include <string>

class Telex {
public:
    Telex();
    bool login(const std::string& email, const std::string& password);

private:
    std::string base_url;
};

#endif // TELEX_H