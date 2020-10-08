/**
 * Retrieve pathes to wave files in the directory.
 * This file is part of project Wav2Mp3_Encoder.
 * @file files_list.h
 * @author Qing Zhang
 * @version 1.0 05/10/2020
 *
*/

#ifndef FILES_LIST_H
#define FILES_LIST_H

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * Retrieve pathes to wave files in the directory.
 * None-wave file will be ignored
 *
 * @param directory path
 * @return std::vector storing pathes to wave files
*/
std::vector<std::string> make_files_list(const char* dir)
{
    std::vector<std::string> files_list;

    const std::string path = dir;
    for (const auto& entry : fs::directory_iterator(path))
    {
        std::string str = entry.path().string();
        auto found = str.find_last_of(".");
        if (str.substr(found + 1) != "wav") continue;
        files_list.push_back(str);
    }
    return files_list;
}
#endif // FILES_LIST_H
