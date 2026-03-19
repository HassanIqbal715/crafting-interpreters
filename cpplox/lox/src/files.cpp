#include "files.h"
#include <filesystem>
#include <fstream>
#include <string_view>

string_view Files::ReadAllBytes(string path) {
    filesystem::path filePath = path;

    if (!filesystem::exists(filePath)) {
        cout << "File \"" << path << "\" not found.\n";
        return "";
    }

    fstream file;

    file.open(filePath, ios::in);

    if (!file.is_open()) {
        cout << "Could not open file.\n";
        return "";
    }

    file.seekp(0, ios_base::beg);

    string data;
    string line;

    while(getline(file, line)) {
        data.append(line);
        data.append("\n");
    }

    if (file.is_open())
        file.close();

    return data;
}