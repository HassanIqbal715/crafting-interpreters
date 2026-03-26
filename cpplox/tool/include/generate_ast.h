#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace GenerateAst {
    const std::string uniqueKeywords;
    void defineAst(std::string outputDir, std::string baseName,
        std::vector<std::string> types, bool includeExpr = false);
    
    void defineForward(std::fstream &file,
        std::vector<std::string> declarations);
        
    void defineType(std::fstream &file, std::string baseName,
        std::string className, std::string fieldList);

    void defineVariant(std::fstream &file, std::string variantName, 
        std::vector<std::string> alternativeTypes );
};