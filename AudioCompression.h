#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#define TRUE 1
#define FALSE 0


typedef struct header{
    unsigned char riff[4];
    unsigned int overall_size;

    unsigned char wave[4];
    unsigned char fmt_chunk_marker[4];

    unsigned int length_of_fmt;
    // length of the format data

    unsigned int format_type;
    // 1 for PCM

    unsigned int channels;
    unsigned int sample_rate;

    unsigned int byterate; 
    unsigned int block_align;

    unsigned int bits_per_sample;
    // either 8-8 bits or 16-16 bits.

    unsigned char data_chunk_header[4];
    // data string or fllr string

    unsigned int data_size;

}header;


void audioCompression(char *filename);
