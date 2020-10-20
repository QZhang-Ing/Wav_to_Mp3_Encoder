/**
 * Main function.
 * This file is the entry point of project Wav2Mp3_Encoder.
 * @file main.cpp
 * @author Qing Zhang
 * @version 1.0 05/10/2020
*/

#include "thread_helper.h"
#include "files_list.h"



int main(int argc, char** argv)
{
    // Check validity of user input 
    if (argc != 2)
    {
        std::cout << "Invalid input! Usage: ./Wave_to_Mp3 <path-to-wav-files>" << std::endl;
        return 1;
    }

    // Check available cpu cores number
    int NUM_CPUS = get_cpuCount();
    std::cout << NUM_CPUS << " CPU cores detected, set "
        << NUM_CPUS << " threads as default" << std::endl;

    // Retrieve pathes of all wave files in a STL vector
    auto files_list = make_files_list(argv[1]);
    const size_t numFiles = files_list.size();
    std::cout << "There are " << numFiles
        << " wave files in the directory to be converted into mp3 files\n"
        << "--------------------------------------------------------------" << std::endl;
    if (!(numFiles > 0)) return EXIT_SUCCESS;

    ThreadHelper threadHelper(NUM_CPUS);
    // Initialize parameters of thread function
    threadHelper.init(numFiles, files_list);
    // Create and synchronize thread workers 
    threadHelper.threading();
    // Print summary
    threadHelper.summary();

    return 0;
}

