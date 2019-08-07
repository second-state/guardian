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

Checker::Checker(std::string jsonPath_1, std::string contract_1, std::string jsonPath_2, std::string contract_2, std::ostream &_out, std::ostream &_errout,bool compareOnlyStorageSize)
    :m_contract_1(contract_1), m_contract_2(contract_2), m_stdout(_out), m_stderr(_errout), compareOnlyStorageSize(compareOnlyStorageSize)
{
    m_jsonLoder_1.clear();
    m_jsonLoder_1.load(jsonPath_1, derr());
    m_jsonLoder_2.clear();
    m_jsonLoder_2.load(jsonPath_2, derr());
}

bool Checker::run()
{
    return checkContarct(m_jsonLoder_1.json()[m_contract_1], m_jsonLoder_2.json()[m_contract_2]);
}

std::ostream &Checker::dout()
{
    return m_stdout;
}

std::ostream &Checker::derr()
{
    return m_stderr;
}

bool Checker::checkContarct(const nlohmann::json &r1, const nlohmann::json &r2){
    const auto SVr1 = r1["stateVariables"], SVr2 = r2["stateVariables"];
    size_t NumberstateVariables = SVr1.size();
    if(NumberstateVariables != SVr2.size()){
        derr()<<"Number of stateVariables are different.\n";
        return false;
    }
    for(size_t i=0; i<NumberstateVariables; ++i){
        if(!checkStateVariable(SVr1[i], SVr2[i], 1)){
            derr()<<"Different at "<<i<<"-th stateVariable.\n";
            return false;
        }
    }
    return true;
}

bool Checker::checkStateVariable(const nlohmann::json &r1, const nlohmann::json &r2, int deep){
    std::stringstream ss1(r1["type"].get<std::string>()), ss2(r2["type"].get<std::string>());
    std::string type_first_1, type_second_1;
    std::string type_first_2, type_second_2;
    ss1>>type_first_1;
    ss2>>type_first_2;
    if(type_first_1 != type_first_2){
        derr()<<std::string(deep*4,' ')<<"Type different: "<<type_first_1<<" | "<<type_first_2<<"\n";
        return false;
    }
    if(r1.count("name")!=r2.count("name")){
        derr()<<std::string(deep*4,' ')<<"One of variables missing name.\n";
        return false;
    }
    if(r1.count("name") && !compareOnlyStorageSize && r1["name"]!=r2["name"]){
        derr()<<std::string(deep*4,' ')<<"Name different: "<<r1["name"].get<std::string>()<<" | "<<r2["name"].get<std::string>()<<"\n";
        return false;
    }
    /*
    if(type_first_1=="mapping") return checkMapping(r1, r2, deep);
    if(type_first_1=="array") return checkArray(r1, r2, deep);
    if(type_first_1=="struct") return checkStruct(r1, r2, deep);
    */
    return true;
}