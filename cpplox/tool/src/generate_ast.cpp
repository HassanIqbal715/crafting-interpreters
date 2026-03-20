#include "generate_ast.h"
#include "utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>

void GenerateAst::defineAst(std::string outputDir, std::string baseName, 
        std::vector<std::string> types) {
    std::filesystem::path path = outputDir + "/";
    
    if (!std::filesystem::exists(path)) {
        std::cerr << "Target path \"" << outputDir << "\" not found.\n";
        return;
    }

    path += baseName + ".h";

    std::fstream file;
    file.open(path, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Could not open file!\n";
        return;
    }

    file << "#pragma once" << std::endl;
    file << "#include \"token.h\"" << std::endl;
    file << "#include <vector>" << std::endl;
    file << "#include <variant>" << std::endl;
    file << "#include <memory>\n" << std::endl;
    file << "struct " << capitalize(baseName) << ";\n" << std::endl;

    file << "using Object = ";
    file << "std::variant<bool, double, std::string, std::monostate>;";
    file << std::endl;
    
    file << std::endl;
    for (std::string type : types) {
        std::string className = trim(split(type, ":")[0]);
        std::string fields = trim(split(type, ":")[1]);
        defineType(file, capitalize(baseName), className, fields);
        file << std::endl;
    }

    defineVariant(file, capitalize(baseName), types);

    if (file.is_open())
        file.close();
}

void GenerateAst::defineType(std::fstream &file, std::string baseName,
        std::string className, std::string fieldList) {
    file << "struct " << className << " {" << std::endl; 
    
    std::vector<std::string> fields = split(fieldList, ", ");
    for (std::string field : fields) {
        std::vector<std::string> fieldSplit = split(field, " "); 
        file << "\t";
        if (fieldSplit[0].compare(baseName) == 0) {
            file << "std::unique_ptr<";
        }
        file << fieldSplit[0];
        if (fieldSplit[0].compare(baseName) == 0) {
            file << "> ";
        }
        else {
            file << " ";
        }
        file << fieldSplit[1] + ";" << std::endl;
    }

    file << "};" << std::endl;
}

void GenerateAst::defineVariant(std::fstream &file, std::string variantName, 
        std::vector<std::string> alternativeTypes) {
    // A variable to keep track of 80 characters long line
    int newLineLimit = 0;
    
    file << "using " << variantName << "Variant" << " = std::variant<";
    
    newLineLimit += ((std::string)"using ").length() + variantName.length();
    newLineLimit += ((std::string) "Variant = std::variant<").length();

    for (int i = 0; i < alternativeTypes.size(); i++) {
        std::string type = trim(split(alternativeTypes[i], ":")[0]);
        if (newLineLimit + type.length() + 2 >= 80) {
            newLineLimit = 8;
            file << std::endl << "\t\t";
        }
        newLineLimit += type.length() + 2;
        file << type;
        if (i < alternativeTypes.size() - 1) {
            file << ", ";
        }
        else {
            file << ">;";
        }
    }

    file << '\n' << std::endl;

    newLineLimit = 0;
    newLineLimit += ((std::string)"struct ").length() + variantName.length();
    newLineLimit += ((std::string) "Variant").length();

    file << "struct " << variantName << " : " << variantName << "Variant";

    if (newLineLimit + 2 >= 80) {
        newLineLimit = 4;
        file << std::endl << "\t";
    }

    file << " {" << std::endl;
    file << "\tusing " << variantName << "Variant" << "::variant;" << std::endl;

    file << "};" << std::endl;
}