#include <Core/Checker.h>
#include <Common/Algorithm.h>
#include <CLI/CLI.hpp>

#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    CLI::App app{"This tool can check two state-varables json output from lity is different or not."};

    std::string jsonPath_1, jsonPath_2;
    std::string contract_1, contract_2;
    bool compareTypeOnly = false;
    bool compareDetail = false;

    app.add_option("--base_jsonPath", jsonPath_1, "Path of base state-variables json folder.")->required();
    app.add_option("--base_contract", contract_1, "The base contract you want to compare.")->required();

    app.add_option("--new_jsonPath", jsonPath_2, "Path of new state-variables json folder.")->required();
    app.add_option("--new_contract", contract_2, "The new contract you want to compare.")->required();

    app.add_flag("--typeOnly", compareTypeOnly, "Add this flag will only compare the variable storage size.");
    app.add_flag("--detail", compareDetail, "Add this flag will print detail message when compare two contract.");
    
    CLI11_PARSE(app, argc, argv);

    Checker checker(jsonPath_1, contract_1, jsonPath_2, contract_2, std::cout, std::cerr, compareTypeOnly, compareDetail);

    if(checker.run()) std::cout<<"same\n";
    else std::cout<<"not same\n";
    
    return 0;
}