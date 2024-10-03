/*
Yashwant Bist
August 12, 2024

Implementation of functions
*/
#include "du_header.hpp"
#include <algorithm>
#include <iomanip>
#include <stdexcept>



// Function to print help information
void printHelp() {
    std::cout << "Usage: du [-skhznrb] [--help] [--version] [--block-size=dddd] [folder]*\n";
    std::cout << "-s : Display only the final summary of each root folder\n";
    std::cout << "-k : Set the cluster size to 1024\n";
    std::cout << "-h : Make human readable. Output is in KiB, MiB, GiB, TiB\n";
    std::cout << "-z : Sort rows by size\n";
    std::cout << "-n : Sort rows by folder name\n";
    std::cout << "-r : Display rows in reverse order\n";
    std::cout << "-b : Output in bytes, not clusters\n";
    std::cout << "--help : Display the help listing\n";
    std::cout << "--version : Display the version number\n";
    std::cout << "--block-size=ddd : Set the cluster size to the specified value > 0\n";
}

// Function to print version information
void printVersion() {
    std::cout << "du version 1.0.0\n";
}

// Function to calculate the size of a folder, considering block size and storing file info
uintmax_t getFolderSize(const fs::path& folder, size_t blockSize, std::vector<FileInfo>& files) {
    uintmax_t totalSize = 0;  // Initialize total size

    try {
        // Iterate recursively through directory entries, skipping permission errors
        for (const auto& entry : fs::recursive_directory_iterator(folder, fs::directory_options::skip_permission_denied)) {
            if (fs::is_regular_file(entry.path())) {  // Only consider regular files
                uintmax_t fileSize = fs::file_size(entry.path());  // Get file size
                files.push_back({ entry.path().string(), fileSize });  // Store file info
                // Calculate size considering block size, rounding up to the nearest block
                totalSize += (fileSize + blockSize - 1) / blockSize * blockSize;
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error accessing directory or file: " << e.what() << '\n';  // Handle filesystem errors
    }

    return totalSize;  // Return total size of the folder
}

// Function to convert a file size into a human-readable string (e.g., KiB, MiB)
std::string humanReadableSize(uintmax_t size) {
    constexpr const char* sizes[] = { "B", "KiB", "MiB", "GiB", "TiB" };  // Units of size
    int i = 0;
    double dSize = size;
    while (dSize >= 1024 && i < 4) {  // Convert size to appropriate unit
        dSize /= 1024;
        ++i;
    }
    std::stringstream ss;
    if (dSize < 10) {
        ss << std::fixed << std::setprecision(1) << dSize << sizes[i];  // Format size with 1 decimal place if < 10
    }
    else {
        ss << std::fixed << std::setprecision(0) << dSize << sizes[i];  // Otherwise, no decimal places
    }
    return ss.str();  // Return formatted size string
}

// Function to parse command-line arguments into options and folder paths
void parseArguments(int argc, char* argv[], Options& opts, std::vector<std::string>& folders) {
    for (int i = 1; i < argc; ++i) {  // Iterate over each argument
        std::string arg = argv[i];
        if (arg == "-s") opts.summaryOnly = true;
        else if (arg == "-k") opts.blockSize = 1024;
        else if (arg == "-h") opts.humanReadable = true;
        else if (arg == "-z") opts.sortBySize = true;
        else if (arg == "-n") opts.sortByName = true;
        else if (arg == "-r") opts.reverseOrder = true;
        else if (arg == "-b") opts.outputBytes = true;
        else if (arg == "--help") opts.help = true;
        else if (arg == "--version") opts.version = true;
        else if (arg.rfind("--block-size=", 0) == 0) opts.blockSize = std::stoull(arg.substr(13));  // Extract block size
        else folders.push_back(arg);  // Assume remaining arguments are folder paths
    }
}

// Function to validate the parsed options for consistency
void validateOptions(const Options& opts) {
    if (opts.outputBytes && opts.humanReadable) {
        throw std::runtime_error("Cannot use -b and -h switches together.");  // Check for incompatible options
    }
    if (opts.blockSize <= 0) {
        throw std::runtime_error("Invalid block size: " + std::to_string(opts.blockSize));  // Ensure block size is valid
    }
    if (opts.sortBySize && opts.sortByName) {
        throw std::runtime_error("-z and -n switches are incompatible.");  // Ensure only one sort option is chosen
    }
}

// Function to display disk usage based on the file information and options
void displayDiskUsage(const std::vector<FileInfo>& files, const Options& opts) {
    for (const auto& file : files) {  // Iterate over each file/folder info
        if (opts.humanReadable) {
            std::cout << humanReadableSize(file.size) << "   " << file.path << '\n';  // Output size in human-readable format
        }
        else if (opts.outputBytes) {
            std::cout << file.size << "   " << file.path << '\n';  // Output size in bytes
        }
        else {
            std::cout << (file.size / opts.blockSize) << "   " << file.path << '\n';  // Output size in blocks
        }
    }
}
