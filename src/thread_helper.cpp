/**
 * Define helper class for implementing Pthread
 * This file is part of project Wav2Mp3_Encoder.
 * @file thread_helper.cpp
 * @author Qing Zhang
 * @version 1.0 05/10/2020
 *
*/
#include <iostream>

#include "thread_helper.h"
#include "header_reader.h"
#include "mp3_encoder.h"

#ifdef _WIN32   // win32 & win64 OS
#include <windows.h>
#endif

#ifdef __linux__     // linux OS
#include <unistd.h>
#endif

/**
 * retrieve cpu core counts on the machine used during compile time
 * based on different OS
 *
 * @return number of cpu cores available
*/
int get_cpuCount()
{
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif __linux__
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

/**
 * Initialize input arguments for creating threads and thread function
 *
 * @param number of wave files
 * @param lists of file pathes
*/
void ThreadHelper::init(int numFiles, std::vector<std::string> files_list)
{
    // array as indication that which file is already encoded
    // default -> false -> not encoded
    bool* filesFinished = new bool[numFiles];
    for (int i = 0; i < numFiles; i++) filesFinished[i] = false;

    thread_args.reserve(NUM_THREADS);
    threads.reserve(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        // Initialize argument vectors of thread function
        thread_args.push_back(WorkerArgs());
        thread_args[i].wavNames = files_list;
        thread_args[i].completed = filesFinished;
        thread_args[i].numFiles = numFiles;
        thread_args[i].threadId = i;
        thread_args[i].processedFiles = 0;

        // Initialize threads vector
        pthread_t thread;
        threads.push_back(thread);
    }
    delete[] filesFinished;
}

// or defined as static member of class ThreadHelper
pthread_mutex_t mutex_locker = PTHREAD_MUTEX_INITIALIZER;
// Flag to determine if all files are encoded to mp3
static int count_encoded_files = 0;

/**
 * Thread function
*/
void* ThreadHelper::encode_worker(void* arg)
{

    // Covert void * to struct {WorkerArgs}
    WorkerArgs* args = (WorkerArgs*)arg;

    while (count_encoded_files < args->numFiles)
    {
        int fileNo = 0;    // Which file is currently converted

        // Lock access to disk
        pthread_mutex_lock(&mutex_locker);
        for (int i = 0; i < args->numFiles; ++i)
        {
            if (!args->completed[i]) // if ith file not converted yet
            {
                args->completed[i] = true; // mark as being converted
                fileNo = i;
                break;  // found one file to convert, break, no need to check other files
            }
        }
        pthread_mutex_unlock(&mutex_locker);

        // Update flag
        ++count_encoded_files;

        // String to const char *
        std::string str = (args->wavNames)[fileNo];
        const char* inPath = str.c_str();
        std::string output = str.substr(0, str.find_last_of('.')) + ".mp3";
        const char* outPath = output.c_str();

        // Retrieve sample rate and number of channels from wav file header
        std::tuple<unsigned short, unsigned int> t = head_reader(inPath);
        int nChannels = std::get<0>(t);
        int sampleRate = std::get<1>(t);

        Encoder encoder;
        // Encode and save to disk
        encoder.encode_combo(inPath, outPath, sampleRate, nChannels);

        std::cout << "thread " << args->threadId << " ....... "
            << inPath << std::endl;
        std::cout << "--------------------------------------------------------------"
            << std::endl;
        // Increment count of processed Files
        ++args->processedFiles;
    }
    return NULL;
    pthread_exit((void*)0);
}


/**
 * Create and synchronize thread workers
*/
void ThreadHelper::threading()
{
    // timestamp
    clock_t tBegin = clock();

    // Create worker threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(&threads[i], NULL, encode_worker, (void*)&thread_args[i]);
    }

    // Synchronize and join threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        int ret = pthread_join(threads[i], NULL);
        if (ret != 0)
        {
            std::cerr << "Fail to join POSIX threads" << std::endl;
        }
    }

    // timestamp
    clock_t tEnd = clock();
    time = double(tEnd - tBegin) / CLOCKS_PER_SEC;
}

/**
 * Print short summary of encoding process
*/
void ThreadHelper::summary()
{
    int count = 0;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        std::cout << "Thread ID " << i << " processed " << thread_args[i].processedFiles
            << " files." << std::endl;
        count += thread_args[i].processedFiles;
    }

    std::cout << "Successfully encoded " << count << " wave files to mp3 files in "
        << time << " seconds." << std::endl;
}
