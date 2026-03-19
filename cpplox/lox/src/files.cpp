#include "files.h"
#include <filesystem>
#include <fstream>
#include <string_view>

std::string_view Files::ReadAllBytes(std::string path) {
    std::filesystem::path filePath = path;

    if (!std::filesystem::exists(filePath)) {
        std::cout << "File \"" << path << "\" not found.\n";
        return "";
    }

    std::fstream file;

    file.open(filePath, std::ios::in);

    if (!file.is_open()) {
        std::cout << "Could not open file.\n";
        return "";
    }

    file.seekp(0, std::ios_base::beg);

    std::string data;
    std::string line;

    while(std::getline(file, line)) {
        data.append(line);
        data.append("\n");
    }

    if (file.is_open())
        file.close();

    return data;
}