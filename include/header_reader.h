/**
 * Helper to extract necessary information from wav file header.
 * This file is part of project Wav2Mp3_Encoder.
 * @file header_reader.h
 * @author Qing Zhang
 * @version 1.0 02/10/2020
 *
*/

#ifdef _MSC_VER
#pragma warning(disable:4996) // MSVC compiler warning disable (fopen)
#endif // _MSC_VER

#ifndef HEADER_READER_H
#define HEADER_READER_H

#include <stdio.h>
#include <iostream>
#include <tuple>

struct wavHeader
{
    unsigned char chunckID[4];
    unsigned int chunckSize;
    unsigned char format[4];
    unsigned char subchunk1ID[4];
    unsigned int subchunk1Size;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    unsigned char subchunk2ID[4];
    unsigned int subchunk2Size;

};

/**
 * Read the header of a single wave file
 * @param path of a wave file
 * @return sample rate and number of channels in a tuple
*/
std::tuple<unsigned short, unsigned int> head_reader(const char* str)
{
    wavHeader wavHeader;
    FILE* pFile = fopen(str, "r");

    if (pFile != NULL)
    {
        fread(&wavHeader, sizeof(wavHeader), 1, pFile);
        fclose(pFile);
        return std::make_tuple(wavHeader.numChannels, wavHeader.sampleRate);
    }
    else
    {
        std::cout << "File " << str << " doesn't exit or can not be opened" << std::endl;
        exit(1);
    }
}
#endif // HEADER_READER_H
