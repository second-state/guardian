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

    app.add_option("--jsonPath_1", jsonPath_1, "Path of first state-variables json folder.")->required();
    app.add_option("--contract_1", contract_1, "The first contract you want to compare.")->required();

    app.add_option("--jsonPath_2", jsonPath_2, "Path of second state-variables json folder.")->required();
    app.add_option("--contract_2", contract_2, "The second contract you want to compare.")->required();

    app.add_flag("--typeOnly", compareTypeOnly, "Add this flag will only compare the variable storage size.");
    
    CLI11_PARSE(app, argc, argv);

    Checker checker(jsonPath_1, contract_1, jsonPath_2, contract_2, std::cout, std::cerr, compareTypeOnly);

    if(checker.run()) std::cout<<"same\n";
    else std::cout<<"not same\n";
    
    return 0;
}