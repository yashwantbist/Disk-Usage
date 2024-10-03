#pragma once
/*
Yashwant Bist
August 12, 2024

Class declarations
Function declarations
using replace data types
*/


#include <iostream>
#include <filesystem>
#include <vector>
#include <sstream>

namespace fs = std::filesystem;

// Struct to hold various options specified by the user via command-line arguments
struct Options {
    bool summaryOnly = false;
    bool humanReadable = false;
    bool sortBySize = false;
    bool sortByName = false;
    bool reverseOrder = false;
    bool outputBytes = false;
    bool help = false;
    bool version = false;
    size_t blockSize = 4096;
};

// Struct to store information about each file or folder
struct FileInfo {
    std::string path;
    uintmax_t size;
};

// Function declarations


void printHelp();
void printVersion();
uintmax_t getFolderSize(const fs::path& folder, size_t blockSize, std::vector<FileInfo>& files);
std::string humanReadableSize(uintmax_t size);
void parseArguments(int argc, char* argv[], Options& opts, std::vector<std::string>& folders);
void validateOptions(const Options& opts);
void displayDiskUsage(const std::vector<FileInfo>& files, const Options& opts);


