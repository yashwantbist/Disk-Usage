
/*
Yashwant Bist
August 12, 2024
Overall description of du project
Purpose:
1.• To display allocation space (not absolute file space) for each file and
directory contained in each pathname for a given root directory
Usage:
• du takes at least a single argument, specifying a pathname
• If no pathname is provided, the current directory is used
du reports:
• Pathnames and the sum of the clusters allocated for each directory
• The sum of clusters for the given root directory
*/

#include "du_header.hpp"  // Include the custom header file for disk usage functions and types

int main(int argc, char* argv[]) {
    Options opts;                        // Struct to hold command-line options
    std::vector<std::string> folders;    // Vector to store folder paths to process

    try {
        parseArguments(argc, argv, opts, folders);  // Parse command-line arguments into opts and folders
        validateOptions(opts);                     // Validate the parsed options
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n'; // Print any exception messages
        return 1;  // Exit with error code if an exception occurs
    }

    // Handle help option: If --help is passed, display help and exit
    if (opts.help) {
        printHelp();
        return 0;
    }

    // Handle version option: If --version is passed, display version and exit
    if (opts.version) {
        printVersion();
        return 0;
    }

    // If no folders were specified, default to the current working directory
    if (folders.empty()) {
        folders.push_back(fs::current_path().string());
    }

    std::vector<FileInfo> fileInfos;  // Vector to store information about each file/folder
    for (const auto& folder : folders) {
        uintmax_t size = getFolderSize(folder, opts.blockSize, fileInfos);  // Calculate folder size

        // If --summary-only is specified, print folder path only
        if (opts.summaryOnly) {
            std::cout << folder << '\n';
        }
        else {
            fileInfos.push_back({ folder, size });  // Otherwise, add folder info to fileInfos
        }
    }

    // Handle sorting: Sort by size if --sort-by-size is passed
    if (opts.sortBySize) {
        std::sort(fileInfos.begin(), fileInfos.end(), [](const FileInfo& a, const FileInfo& b) {
            return a.size < b.size;
            });
    }
    // Sort by name if --sort-by-name is passed
    else if (opts.sortByName) {
        std::sort(fileInfos.begin(), fileInfos.end(), [](const FileInfo& a, const FileInfo& b) {
            return a.path < b.path;
            });
    }

    // Handle reverse order: Reverse the sorted order if --reverse is passed
    if (opts.reverseOrder) {
        std::reverse(fileInfos.begin(), fileInfos.end());
    }

    // Display disk usage information unless --summary-only is passed
    if (!opts.summaryOnly) {
        displayDiskUsage(fileInfos, opts);
    }

    return 0;  // Exit successfully
}
