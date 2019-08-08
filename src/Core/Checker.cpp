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
    checkingStructs.clear();
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
    std::string type_1 = r1["type"].get<std::string>(), type_2 = r2["type"].get<std::string>();

    if(r1["type"] != r2["type"]){
        derr()<<std::string(deep*4,' ')<<"Type different: "<<type_1<<" | "<<type_2<<"\n";
        return false;
    }
    if(r1.count("name")!=r2.count("name")){
        derr()<<std::string(deep*4,' ')<<"["<<type_1<<"]"<<" One of variables missing name.\n";
        return false;
    }
    if(r1.count("name") && !compareOnlyStorageSize && r1["name"]!=r2["name"]){
        derr()<<std::string(deep*4,' ')<<"["<<type_1<<"]"<<" Name different: "<<r1["name"].get<std::string>()<<" | "<<r2["name"].get<std::string>()<<"\n";
        return false;
    }
    
    if(type_1=="mapping") return checkMapping(r1, r2, deep);
    if(type_1=="array") return checkArray(r1, r2, deep);
    if(type_1=="struct") return checkStruct(r1, r2, deep);
    
    return true;
}

bool Checker::checkMapping(const nlohmann::json &r1, const nlohmann::json &r2, int deep){
    if(!checkStateVariable(r1["key"], r2["key"], deep+1)){
        derr()<<std::string(deep*4,' ')<<"[Mapping] Key different\n";
        return false;
    }
    if(!checkStateVariable(r1["value"], r2["value"], deep+1)){
        derr()<<std::string(deep*4,' ')<<"[Mapping] Value different\n";
        return false;
    }
    return true;
}

bool Checker::checkArray(const nlohmann::json &r1, const nlohmann::json &r2, int deep){
    if(r1.count("length")!=r2.count("length")){
        derr()<<std::string(deep*4,' ')<<"[Array] One is dynamic length another is not.\n";
        return false;
    }
    if(r1.count("length") && r1["length"]!=r2["length"]){
        derr()<<std::string(deep*4,' ')<<"[Array] Length different: "<<r1["length"].get<std::string>()<<" | "<<r2["length"].get<std::string>()<<"\n";
        return false;
    }
    if(!checkStateVariable(r1["baseType"], r2["baseType"], deep+1)){
        derr()<<std::string(deep*4,' ')<<"[Array] baseType different\n";
        return false;
    }
    return true;
}
bool Checker::checkStruct(const nlohmann::json &r1, const nlohmann::json &r2, int deep){
    if(!compareOnlyStorageSize && r1["structName"]!=r2["structName"]){
        derr()<<std::string(deep*4,' ')<<"[Struct] Struct name are different: "<<r1["structName"]<<" | "<<r2["structName"]<<"\n";
        return false;
    }
    std::string structLocation1 = r1["structLocation"].get<std::string>();
    std::string structName1 = r1["structName"].get<std::string>();
    std::string structLocation2 = r2["structLocation"].get<std::string>();
    std::string structName2 = r2["structName"].get<std::string>();

    if(checkingStructs.count(structLocation1+":"+structName1)){
        return true;
    }
    checkingStructs.insert(structLocation1+":"+structName1);
    const nlohmann::json &s1 = m_jsonLoder_1.json()[structLocation1]["structDefination"][structName1];
    const nlohmann::json &s2 = m_jsonLoder_2.json()[structLocation2]["structDefination"][structName2];
    size_t NumberstateVariables = s1.size();
    if(NumberstateVariables != s2.size()){
        derr()<<std::string(deep*4,' ')<<"[Struct] Number of variables are different.\n";
        return false;
    }
    for(size_t i=0; i<NumberstateVariables; ++i){
        if(!checkStateVariable(s1[i], s2[i], deep+1)){
            derr()<<std::string(deep*4,' ')<<"[Struct] Different at "<<i<<"-th variable.\n";
            return false;
        }
    }
    checkingStructs.erase(structLocation1+":"+structName1);
    return true;
}