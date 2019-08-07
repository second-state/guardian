#include <Loader/JsonLoader.h>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <string>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>

#include <Common/FileUtils.h>
#include <Filesystem/FileIterator.h>

namespace fs = std::filesystem;

JsonLoader::JsonLoader()
{

}

JsonLoader::~JsonLoader()
{

}


void JsonLoader::clear()
{
    m_json.clear();
}

bool JsonLoader::load(std::string _base, std::ostream &derr)
{
    FileIterator fit;
    fit.visit(_base, [&](fs::path file){
        if( file.has_extension() )
        {
            const std::string ext = file.extension().string();
            const std::string full_name = file.string();

            if( ext != ".json" )
                return ;

            std::string json_str = GetFileContent(full_name);

            nlohmann::json json;
            json = nlohmann::json::parse(json_str.begin(), json_str.end());
            m_json[json.begin().key()] = json.begin().value();
        }
    });
    return true;
}

const nlohmann::json &JsonLoader::json()
{
    return m_json;
}
