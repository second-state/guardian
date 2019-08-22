#include <Core/Checker.h>

#include <Common/Algorithm.h>
#include <Loader/JsonLoader.h>

#include <sstream>
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

std::pair<std::string,std::string> split2(const std::string &s){
    std::stringstream ss(s);
    std::pair<std::string,std::string> res;
    getline(ss,res.first);
    getline(ss,res.second);
    return res;
}

Checker::Checker(std::string jsonPath_1, std::string contract_1, std::string jsonPath_2, std::string contract_2, std::ostream &_out, std::ostream &_errout,bool compareOnlyStorageSize, bool compareDetail)
    :m_contract_1(contract_1), m_contract_2(contract_2), m_stdout(_out), m_stderr(_errout), compareOnlyStorageSize(compareOnlyStorageSize), compareDetail(compareDetail)
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
    bool allSame = true;
    if(NumberstateVariables > SVr2.size()){
        derr() << "[!different] Number of stateVariables are different. More parts will not be compared.\n";
        derr() << ">>>First contract:\n";
        derr() << "   Number of stateVariables is" << NumberstateVariables << '\n';
        derr() << "===\n";
        derr() << "   Number of stateVariables is" << SVr2.size() << '\n';
        dout() << "<<<Second contract\n\n";
        allSame = false;
        NumberstateVariables = SVr2.size();
    }
    for(size_t i=0; i<NumberstateVariables; ++i){
        if(!checkStateVariable(SVr1[i], SVr2[i], "", "", 1)){
            derr()<<"Different at "<<i<<"-th stateVariable.\n\n";
            allSame =  false;
        }
    }
    return allSame;
}

bool Checker::checkStateVariable(const nlohmann::json &r1, const nlohmann::json &r2, std::string sourcePart_1, std::string sourcePart_2, int deep){
    if(r1.count("sourceInfo"))
        sourcePart_1 = r1["sourceInfo"].get<std::string>();
    if(r2.count("sourceInfo"))
        sourcePart_2 = r2["sourceInfo"].get<std::string>();
        
    std::string type_1 = r1["type"].get<std::string>(), type_2 = r2["type"].get<std::string>();
    auto deepStr = std::string(deep*compareDetail*4,' ');

    if(r1["type"] != r2["type"]){
        auto sourcePartPair_1 = split2(sourcePart_1);
        auto sourcePartPair_2 = split2(sourcePart_2);
        derr() << deepStr << "Type different:\n";
        derr() << deepStr << ">>>First contract:\n";
        derr() << deepStr << "   Type of stateVariable is \"" << type_1 << "\""  << '\n';
        derr() << deepStr << sourcePartPair_1.first << '\n';
        derr() << deepStr << sourcePartPair_1.second << '\n';
        derr() << deepStr << "===\n";
        derr() << deepStr << "   Type of stateVariable is \"" << type_2 << "\""  << '\n';
        derr() << deepStr << sourcePartPair_2.first << '\n';
        derr() << deepStr << sourcePartPair_2.second << '\n';
        dout() << deepStr << "<<<Second contract\n";
        return false;
    }
    bool isSame = true;
    if(r1.count("name")!=r2.count("name")){
        derr()<<deepStr<<"["<<type_1<<"]"<<" One of variables missing name.\n";
        isSame =  false;
    }
    if(r1.count("name") && !compareOnlyStorageSize && r1["name"]!=r2["name"]){
        auto sourcePartPair_1 = split2(sourcePart_1);
        auto sourcePartPair_2 = split2(sourcePart_2);
        derr() << deepStr << "[" << type_1 << "]" << " Name different:\n";
        derr() << deepStr << ">>>First contract:\n";
        derr() << deepStr << "   Name of stateVariable is \"" << r1["name"].get<std::string>() << "\"" << '\n';
        derr() << deepStr << sourcePartPair_1.first << '\n';
        derr() << deepStr << sourcePartPair_1.second << '\n';
        derr() << deepStr << "===\n";
        derr() << deepStr << "   Name of stateVariable is \"" << r2["name"].get<std::string>() << "\"" << '\n';
        derr() << deepStr << sourcePartPair_2.first << '\n';
        derr() << deepStr << sourcePartPair_2.second << '\n';
        dout() << deepStr << "<<<Second contract\n";
        isSame = false;
    }
    
    if(type_1=="mapping") return checkMapping(r1, r2, sourcePart_1, sourcePart_2, deep);
    if(type_1=="array") return checkArray(r1, r2, sourcePart_1, sourcePart_2, deep);
    if(type_1=="struct") return checkStruct(r1, r2, sourcePart_1, sourcePart_2, deep);
    
    return isSame;
}

bool Checker::checkMapping(const nlohmann::json &r1, const nlohmann::json &r2, std::string sourcePart_1, std::string sourcePart_2, int deep){
    auto deepStr = std::string(deep*compareDetail*4,' ');
    if(!checkStateVariable(r1["key"], r2["key"], sourcePart_1, sourcePart_2, deep+1)){
        if(compareDetail) derr()<<deepStr<<"[Mapping] Key different\n";
        return false;
    }
    if(!checkStateVariable(r1["value"], r2["value"], sourcePart_1, sourcePart_2, deep+1)){
        if(compareDetail) derr()<<deepStr<<"[Mapping] Value different\n";
        return false;
    }
    return true;
}

bool Checker::checkArray(const nlohmann::json &r1, const nlohmann::json &r2, std::string sourcePart_1, std::string sourcePart_2, int deep){
    auto deepStr = std::string(deep*compareDetail*4,' ');
    if(r1.count("length")!=r2.count("length")){
        derr()<<deepStr<<"[Array] One is dynamic length another is not.\n";
        return false;
    }
    if(r1.count("length") && r1["length"]!=r2["length"]){
        auto sourcePartPair_1 = split2(sourcePart_1);
        auto sourcePartPair_2 = split2(sourcePart_2);
        derr() << deepStr<<"[Array] Length different:\n";
        derr() << deepStr << ">>>First contract:\n";
        derr() << deepStr << "   Length is " << r1["length"].get<std::string>() << '\n';
        derr() << deepStr << sourcePartPair_1.first << '\n';
        derr() << deepStr << sourcePartPair_1.second << '\n';
        derr() << deepStr << "===\n";
        derr() << deepStr << "   Length is " << r2["length"].get<std::string>() << '\n';
        derr() << deepStr << sourcePartPair_2.first << '\n';
        derr() << deepStr << sourcePartPair_2.second << '\n';
        dout() << deepStr << "<<<Second contract\n";
        return false;
    }
    if(!checkStateVariable(r1["baseType"], r2["baseType"], sourcePart_1, sourcePart_2, deep+1)){
        if(compareDetail) derr()<<deepStr<<"[Array] baseType different\n";
        return false;
    }
    return true;
}
bool Checker::checkStruct(const nlohmann::json &r1, const nlohmann::json &r2, std::string sourcePart_1, std::string sourcePart_2, int deep){
    auto deepStr = std::string(deep*compareDetail*4,' ');
    bool isSame = true;
    if(!compareOnlyStorageSize && r1["structName"]!=r2["structName"]){
        auto sourcePartPair_1 = split2(sourcePart_1);
        auto sourcePartPair_2 = split2(sourcePart_2);
        derr() << deepStr << "[Struct] Struct name are different:\n";
        derr() << deepStr << ">>>First contract:\n";
        derr() << deepStr << "   Struct name is \"" << r1["structName"] << "\"" << '\n';
        derr() << deepStr << sourcePartPair_1.first << '\n';
        derr() << deepStr << sourcePartPair_1.second << '\n';
        derr() << deepStr << "===\n";
        derr() << deepStr << "   Struct name is \"" << r2["structName"] << "\"" << '\n';
        derr() << deepStr << sourcePartPair_2.first << '\n';
        derr() << deepStr << sourcePartPair_2.second << '\n';
        dout() << deepStr << "<<<Second contract\n";
        isSame = false;
    }
    std::string structLocation1 = r1["structLocation"].get<std::string>();
    std::string structName1 = r1["structName"].get<std::string>();
    std::string structLocation2 = r2["structLocation"].get<std::string>();
    std::string structName2 = r2["structName"].get<std::string>();

    std::string cmpStruct = structLocation1+":"+structName1+" "+structLocation2+":"+structName2;

    if(checkingStructs.count(cmpStruct)){
        if(!checkingStructs[cmpStruct]){
            auto sourcePartPair_1 = split2(sourcePart_1);
            auto sourcePartPair_2 = split2(sourcePart_2);
            derr() << deepStr << "[Struct] Different but already compared abrove.\n";
            derr() << deepStr << ">>>First contract:\n";
            derr() << deepStr << "   Struct name is \"" << structLocation1+":"+structName1 << "\"" << '\n';
            derr() << deepStr << sourcePartPair_1.first << '\n';
            derr() << deepStr << sourcePartPair_1.second << '\n';
            derr() << deepStr << "===\n";
            derr() << deepStr << "   Struct name is \"" << structLocation2+":"+structName2 << "\"" << '\n';
            derr() << deepStr << sourcePartPair_2.first << '\n';
            derr() << deepStr << sourcePartPair_2.second << '\n';
            dout() << deepStr << "<<<Second contract\n";
            return false;
        }
        return true;
    }
    const nlohmann::json &s1 = m_jsonLoder_1.json()[structLocation1]["structDefination"][structName1]["variables"];
    const nlohmann::json &s2 = m_jsonLoder_2.json()[structLocation2]["structDefination"][structName2]["variables"];
    size_t NumberstateVariables = s1.size();
    if(NumberstateVariables != s2.size()){
        derr() << deepStr << "[Struct] Number of variables are different.\n";
        derr() << deepStr << ">>>First contract:\n";
        derr() << deepStr << "   Number of variables is" << NumberstateVariables << '\n';
        derr() << deepStr << "===\n";
        derr() << deepStr << "   Number of variables is" << s2.size() << '\n';
        dout() << deepStr << "<<<Second contract\n";
        return false;
    }
    for(size_t i=0; i<NumberstateVariables; ++i){
        if(!checkStateVariable(s1[i], s2[i], "", "", deep+1)){
            if(compareDetail) derr()<<deepStr<<"[Struct] Different at "<<i<<"-th variable.\n";
            isSame = false;
        }
    }
    checkingStructs[cmpStruct] = isSame;
    return isSame;
}