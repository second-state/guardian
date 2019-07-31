#pragma once

#include <nlohmann/json.hpp>
#include <ostream>
#include <string>

class JsonLoader
{
public:
    JsonLoader();
    ~JsonLoader();
    void clear();
    bool load(std::string _base, std::ostream &derr);
    const std::vector<nlohmann::json> &json();
private:
    std::vector<nlohmann::json> m_json;
};
