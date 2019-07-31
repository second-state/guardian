#pragma once

#include <Loader/JsonLoader.h>

#include <ostream>
#include <string>

class Checker
{
public:
    Checker(std::string jsonPath_1, std::string contract_1, std::string jsonPath_2, std::string contract_2, std::ostream &_out, std::ostream &_errout);

    bool run();

private:
    void showOSInfo();

    std::ostream& dout();
    std::ostream& derr();

    JsonLoader m_jsonLoder_1, m_jsonLoder_2;

    std::string m_contract_1, m_contract_2;

    std::ostream &m_stdout;
    std::ostream &m_stderr;
};
