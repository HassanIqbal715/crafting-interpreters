#include "utils.h"

// Alternative to strtok() from C. Use a delimiter to split a string
std::vector<std::string> split(std::string str, std::string delimiter) {
    std::vector<std::string> output = {};

    if (str.length() < delimiter.length()) {
        output.push_back(str);
        return output;
    }

    int start = 0;

    for (int i = 0; i < str.length(); i++) {
        bool found = true;
        for (int j = 0; j < delimiter.length() && i < str.length(); j++) {
            if (str[i + j] != delimiter[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            output.push_back(str.substr(start, i - start));
            start = i + delimiter.length();
        }
    }

    output.push_back(str.substr(start, str.length() - start));
    return output;
}

// Remove trailing spaces on both ends
std::string trim(std::string str) {
    int left = 0;
    while(str[left] == ' ' && left < str.length()) left++;

    int right = str.length() - 1;
    while(str[right] == ' ' && right >= 0) right--;

    if (left < right) {
        return str.substr(left, right - left + 1);
    }

    return "";
}

// Capitalize the first character of every word
std::string capitalize(std::string str) {
    for (int i = 0; i < str.length(); i++) {
        if (i < str.length() - 1) {
            if (i == ' ' || i == '\n' || i == '\t' || i == '\r') {
                if (str[i + 1] >= 'a' && str[i - 1] <= 'z') {
                    str[i + 1] -= 32;
                }
                continue;
            }
        }
        if (i == 0 && (str[i] >= 'a' && str[i] <= 'z')) {
            str[i] -= 32;
        }
    }
    
    return str;
}