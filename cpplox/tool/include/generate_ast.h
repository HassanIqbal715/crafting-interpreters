#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace GenerateAst {
    void defineAst(std::string outputDir, std::string baseName,
        std::vector<std::string> types);

    void defineType(std::fstream &file, std::string baseName,
        std::string className, std::string fieldList);

    void defineVariant(std::fstream &file, std::string variantName, 
        std::vector<std::string> alternativeTypes );
};