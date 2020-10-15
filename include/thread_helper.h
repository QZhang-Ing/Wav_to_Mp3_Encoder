/**
 * Declare helper class and define helper function for Pthread
 * This file is part of project Wav2Mp3_Encoder.
 * @file thread_helper.h
 * @author Qing Zhang
 * @version 1.0 06/10/2020
*/
#ifndef THREAD_HELPER_H
#define THREAD_HELPER_H

#include <pthread.h>
#include <vector>
#include <string>

/**
 * detect machine cpu infomation
*/
int get_cpuCount();


/**
 * structure for creating input argument to thread function
*/
struct WorkerArgs
{
    std::vector<std::string> wavNames;
    bool* completed;
    int numFiles;
    int threadId;
    int processedFiles;
};

/**
 * Helper class to controll Pthread
*/
class ThreadHelper
{
public:
    ThreadHelper() {}
    ThreadHelper(int num) : NUM_THREADS(num) {}
    ~ThreadHelper() {}
    void init(int numFiles, std::vector<std::string> files_list);
    static void* encode_worker(void* arg);
    void threading();
    void summary();

private:
    std::vector<WorkerArgs> thread_args;
    std::vector<pthread_t> threads;
    int NUM_THREADS;
    double time = 0.0;
};


#endif // THREAD_HELPER_H
