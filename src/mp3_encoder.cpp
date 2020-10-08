/**
 * Define mp3 endcoder class.
 * This file is part of project Wav2Mp3_Encoder.
 * @file mp3_encoder.cpp
 * @author Qing Zhang
 * @version 1.0 05/10/2020
*/
#ifdef _MSC_VER
#pragma warning(disable:4996) // MSVC compiler warning disable (fopen)
#endif // _MSC_VER

#include <cstddef>
#include "mp3_encoder.h"


Encoder::~Encoder()
{
    lame_close(lame);
    fclose(mp3File);
    fclose(wavFile);
}

/**
 * Initialize lame encoder object
 * @param path to wave file
 * @param path to create mp3 file
 * @param samplerate of wave file
 * @param number of channels of wave file
 * @return sample rate and number of channels in a tuple
*/
int Encoder::init(const char* wavFilePath, const char* mp3FilePath,
    int sampleRate, int nChannels)
{
    wavFile = fopen(wavFilePath, "rb");
    if (!wavFile)
    {
        std::cerr << "fail to open " << wavFilePath << " file" << std::endl;
        exit(1);
    }
    fseek(wavFile, 4 * 1024, SEEK_CUR);  // skip wav file header
    mp3File = fopen(mp3FilePath, "wb");
    if (!mp3File)
    {
        std::cerr << "fail to create and open " << mp3FilePath << " file" << std::endl;
        exit(1);
    }

    lame = lame_init();
    lame_set_in_samplerate(lame, sampleRate);
    lame_set_num_channels(lame, nChannels);
    (nChannels == 1) ? lame_set_mode(lame, MONO) : lame_set_mode(lame, STEREO);
    lame_set_quality(lame, 2); // Quality: high
    lame_set_VBR(lame, vbr_default);

    // flag to check if lame_encoder successfully initialized
    int ret = lame_init_params(lame);

    return ret;
}

/**
 * Encode wave file to mp3 file and save to disk
 * @param samplerate of wave file
 * @param number of channels of wave file
*/
void Encoder::encode(int numChannels)
{
    std::size_t read, write;
 
    constexpr auto WAV_SIZE = BUFFER_SIZE;
    constexpr auto MP3_SIZE = BUFFER_SIZE;

    short int* wav_buffer = new short int[WAV_SIZE*numChannels];
    unsigned char* mp3_buffer = new unsigned char[MP3_SIZE];
    //short int wav_buffer[WAV_SIZE* numChannels];
    //unsigned char mp3_buffer[MP3_SIZE];

    do
    {
        read = fread(wav_buffer, numChannels * sizeof(short int), WAV_SIZE, wavFile);
        if (read == 0)
            write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
        else if (numChannels == 1) // Mono
            write = lame_encode_buffer(lame, wav_buffer, NULL, read, mp3_buffer, MP3_SIZE);
        else                     // Stereo
            write = lame_encode_buffer_interleaved(lame, wav_buffer, read, mp3_buffer, MP3_SIZE);
        fwrite(mp3_buffer, write, sizeof(unsigned char), mp3File);
    } while (read != 0);

    delete [] wav_buffer;
    delete[] mp3_buffer;
}

/**
 * Initiate and encode combination
 *
 * @param path to wave file
 * @param path to create mp3 file
 * @param sample rate of wave file
 * @param number of channels of wave file
*/
void Encoder::encode_combo(const char* wavFilePath, const char* mp3FilePath,
    int sampleRate, int nChannels)
{
    // Initialize lame encoder object
    int ret = init(wavFilePath, mp3FilePath, sampleRate, nChannels);
    if (ret < 0)
    {
        std::cerr << "unable to initialize lame encoder" << std::endl;
        exit(-1);
    }

    // Encode wave file to mp3 file and save to disk
    encode(nChannels);
}