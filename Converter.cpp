#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>
#include <string>

namespace fs = std::filesystem;
using namespace std;

bool has(string original, string check) {
    return (original.find(check) != string::npos);
}

bool isWhitespaceOnly(const std::string& str) {
    // Iterate through each character in the string
    for (char c : str) {
        // Check if the character is not whitespace
        if (!std::isspace(static_cast<unsigned char>(c))) {
            return false; // If a non-whitespace character is found, return false
        }
    }
    // If all characters are whitespace, return true
    return true;
}

void processFile(const fs::path& filePath, std::vector<std::string>& failures) {
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        failures.push_back("Failed to open file: " + filePath.string());
        return;
    }

    string json = "{";
    string fileName = "";
    string dialogueGroup = "";
    string line;
    int lineNumber = 0;

    int dialogueNumber = 1;

    while (getline(inFile, line)) {
        lineNumber += 1;
        if (fileName == "") {
            fileName = line;
            continue;
        }

        if (line.size() > 0 && line.at(0) == '>') {
            dialogueNumber = 1;

            if (dialogueGroup != "") {
                json += "\n\t},";
            }
            json += "\n\n";
            dialogueGroup = line.substr(1, line.size() - 1);
            json += "\t\"" + dialogueGroup + "\": {";
        
        } else if (line.size() > 0 && line.at(0) == '/') {
            // Comment line
        
        } else if (has(line, ":")) {
            if (dialogueGroup == "") {
                failures.push_back("Line outside of dialogue group: " + filePath.string() + " Line: " + to_string(lineNumber));
                return;
            }

            if (dialogueNumber > 1) {
                json += ",";
            }
            json += "\n\t\t\"" + to_string(dialogueNumber) + "\" : {";
            if (line.at(0) != ':') {
                string speakerName = line.substr(0, line.find(':'));
                json += "\"speaker\":\"" + speakerName + "\", ";
            }
            string lineText = line.substr(line.find(':') + 1);
            if (lineText.at(0) == ' ') {
                lineText = lineText.substr(1);
            }
            json += "\"text\": \"" + lineText + "\"";
            // BUST HERE
            json += "}";
            dialogueNumber += 1;
        } else if (!isWhitespaceOnly(line)) {
            failures.push_back("Line with no formatting: " + filePath.string() + " Line: " + to_string(lineNumber));
            return;
        }

    }
    
    json += "\n\t}\n}";

    fileName += ".json";
    std::ofstream outFile(fileName);
    if (!outFile.is_open()) {
        failures.push_back("Failed to create JSON file for: " + filePath.string());
        return;
    } else {
        outFile << json;
        cout << fileName << " succesfully created.\n";
    }
    outFile.close();
}

int main() {
    std::vector<std::string> failures;

    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.path().extension() == ".txt" && entry.path() != "Instructions.txt") {
            processFile(entry.path(), failures);
        }
    }

    if (!failures.empty()) {
        std::cerr << "Failures encountered:" << std::endl;
        for (const auto& failure : failures) {
            std::cerr << failure << std::endl;
        }
    } else {
        std::cout << "All files processed successfully!" << std::endl;
    }

    return 0;
}