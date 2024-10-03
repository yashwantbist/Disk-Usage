# Disk-Usage
This C++ console application estimates file and directory space usage, mimicking the behavior of the UNIX du command. It calculates the allocation space for files and directories, providing detailed reports of cluster allocation. The program supports various command-line switches and handles complex input scenarios efficiently, making it a useful tool for analyzing disk space utilization.

Features
Displays allocation space for each file and directory.
Supports multiple command-line options for customized usage reports.
Summarizes cluster allocation for directories and files.
Recursive directory traversal to account for nested files.
Error handling for incorrect file paths and input formats.
Cross-platform compatibility with Windows, compiled using /std
++20.
How to Use
Clone the repository.
Compile the code with a C++20 compliant compiler.
Run the executable, specifying the root directory and optional switches for detailed space usage.
