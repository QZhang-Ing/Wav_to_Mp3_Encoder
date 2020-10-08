/**
 * Declare mp3 endcoder class.
 * This file is part of project Wav2Mp3_Encoder.
 * @file mp3_encoder.h
 * @author Qing Zhang
 * @version 1.0 03/10/2020
 *
*/



#ifndef MP3_ENCODER_H
#define MP3_ENCODER_H
#include <stdio.h>
#include <iostream>
#include <cstddef>
#include <lame/lame.h>

constexpr std::size_t BUFFER_SIZE = 8192;

class Encoder
{
public:
    Encoder() {}
    ~Encoder();
    int init(const char* wavFilePath, const char* mp3FilePath, int sampleRate, int nChannels);
    void encode(int numChannels);
    void encode_combo(const char* wavFilePath, const char* mp3FilePath,
        int sampleRate, int nChannels);

private:
    FILE* wavFile;
    FILE* mp3File;
    lame_t lame;
};

#endif // MP3_ENCODER_H
