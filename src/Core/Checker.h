#pragma once

#include <Loader/JsonLoader.h>

#include <ostream>
#include <string>
#include <map>

class Checker
{
public:
    Checker(std::string jsonPath_1, std::string contract_1, std::string jsonPath_2, std::string contract_2, std::ostream &_out, std::ostream &_errout, bool compareOnlyStorageSize, bool compareDetail);

    bool run();

private:

    std::ostream& dout();
    std::ostream& derr();

    bool checkContarct(const nlohmann::json &r1, const nlohmann::json &r2);
    bool checkStateVariable(const nlohmann::json &r1, const nlohmann::json &r2, std::string sourcePart_1, std::string sourcePart_2, int deep);
    bool checkMapping(const nlohmann::json &r1, const nlohmann::json &r2, std::string sourcePart_1, std::string sourcePart_2, int deep);
    bool checkArray(const nlohmann::json &r1, const nlohmann::json &r2, std::string sourcePart_1, std::string sourcePart_2, int deep);
    bool checkStruct(const nlohmann::json &r1, const nlohmann::json &r2, std::string sourcePart_1, std::string sourcePart_2, int deep);

    JsonLoader m_jsonLoder_1, m_jsonLoder_2;
    std::string m_contract_1, m_contract_2;
    bool compareOnlyStorageSize;
    bool compareDetail;

    std::map<std::string, bool> checkingStructs;

    std::ostream &m_stdout;
    std::ostream &m_stderr;
};
