#include <Core/Checker.h>

#include <Common/Algorithm.h>
#include <Loader/JsonLoader.h>

#include <chrono>
#include <cstring>
#include <ctime>
#include <ostream>
#include <iomanip>
#include <string>
#include <set>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/utsname.h>
#include <errno.h>
#endif

Checker::Checker(std::string jsonPath_1, std::string contract_1, std::string jsonPath_2, std::string contract_2, std::ostream &_out, std::ostream &_errout)
    :m_contract_1(contract_1), m_contract_2(contract_2), m_stdout(_out), m_stderr(_errout)
{
    m_jsonLoder_1.clear();
    m_jsonLoder_1.load(jsonPath_1, derr());
    m_jsonLoder_2.clear();
    m_jsonLoder_2.load(jsonPath_2, derr());
}

bool Checker::run()
{
    showOSInfo();
    return m_jsonLoder_1.json() == m_jsonLoder_2.json();
}

void Checker::showOSInfo()
{
    dout() << "=== OS Information\n";
#ifdef _WIN32
    dout() << "OS     : Windows\n";
    dout() << "Release: ?\n";
    dout() << "Version: ?\n";
#else
    utsname ust;
    if (uname(&ust) == 0)
    {
        dout() << "OS     : " << ust.sysname << "\n";
        dout() << "Release: " << ust.release << '\n';
        dout() << "Version: " << ust.version << "\n";
    }
    else
    {
        dout() << "uname error:" << errno << std::endl;
    }
#endif
}

std::ostream &Checker::dout()
{
    return m_stdout;
}

std::ostream &Checker::derr()
{
    return m_stderr;
}
