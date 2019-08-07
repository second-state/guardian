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
    const nlohmann::json &json();
private:
    nlohmann::json m_json;
};
