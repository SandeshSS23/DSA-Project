#include "AudioCompression.h"

unsigned char buffer4[4];
unsigned char buffer2[2];

// Mapped=32767*(pow (2, sample/65356)

header myheader;

double power(double base, int exponent)
{
    double result = 1.0;

    if (exponent > 0)
    {
        for (int i = 0; i < exponent; i++)
        {
            result *= base;
        }
    }
    else if (exponent < 0)
    {
        for (int i = 0; i < -exponent; i++)
        {
            result /= base;
        }
    }
    return result;
}

int8_t MuLaw_Encode(int16_t number)
{
    const uint16_t MULAW_MAX = 0x1FFF;
    const uint16_t MULAW_BIAS = 33;
    uint16_t mask = 0x1000;
    uint8_t sign = 0;
    uint8_t position = 12;
    uint8_t lsb = 0;
    if (number < 0)
    {
        number = -number;
        sign = 0x80;
    }
    number += MULAW_BIAS;
    if (number > MULAW_MAX)
    {
        number = MULAW_MAX;
    }
    for (; ((number & mask) != mask && position >= 5); mask >>= 1, position--)
        ;
    lsb = (number >> (position - 4)) & 0x0f;
    return (~(sign | ((position - 5) << 4) | lsb));
}

int16_t MuLaw_Decode(int8_t number)
{
    const uint16_t MULAW_BIAS = 33;
    uint8_t sign = 0, position = 0;
    int16_t decoded = 0;
    number = ~number;
    if (number & 0x80)
    {
        number &= ~(1 << 7);
        sign = -1;
    }
    position = ((number & 0xF0) >> 4) + 5;
    decoded = ((1 << position) | ((number & 0x0F) << (position - 4)) | (1 << (position - 5))) - MULAW_BIAS;
    return (sign == 0) ? (decoded) : (-(decoded));
}

void intToBinary8(int num, char *binary)
{
    *binary = 0; // Initialize the char variable

    for (int i = 0; i < 8; i++)
    {
        *binary <<= 1;                   // Left shift the bits in the char by 1 position
        *binary |= (num >> (7 - i)) & 1; // Set the least significant bit with the corresponding bit from the integer
    }
}

void intToBinary16(int num, char *binary)
{
    binary[0] = num & 0xFF;        // Store the most significant 8 bits in the first element
    binary[1] = (num >> 8) & 0xFF; // Store the least significant 8 bits in the second element
}

int convertToBinary2(int num)
{
    if (num == 0)
    {
        return 0; // Base case: num is 0
    }
    else
    {
        return (num % 2) + 10 * convertToBinary2(num / 2);
    }
}

void convertToBinary(int num, int numBits, char *binary)
{
    unsigned int mask = 1 << (numBits - 1); // Create a mask to extract each bit

    for (int i = 1; i < numBits; i++)
    {
        if (num & mask)
            binary[i] = '1';
        else
            binary[i] = '0';

        mask >>= 1; // Shift the mask to the right
    }
}

uint8_t convertToBinary3(uint8_t number)
{
    uint8_t result = 0;
    for (int i = 7; i >= 0; i--)
    {
        uint8_t bit = (number >> i) & 1;
        result = (result << 1) | bit;
    }
    return result;
}

uint16_t convertToBinary4(uint16_t number)
{
    uint8_t result = 0;
    for (int i = 15; i >= 0; i--)
    {
        uint16_t bit = (number >> i) & 1;
        result = (result << 1) | bit;
    }
    return result;
}

int decompressValue(int compressedValue, int outputbits, int sample_bits)
{
    int decompressedValue;
    if (outputbits == 8 && sample_bits == 16)
    {
        if (compressedValue > 0)
        {
            decompressedValue = (int)(65356 * ((log((double)compressedValue / 127) / log(2))));
        }
        else
        {
            decompressedValue = (int)(-65356 * (log((double)(-compressedValue) / 127) / log(2)));
        }
        return decompressedValue;
    }
    else if (outputbits == 4 && sample_bits == 16)
    {
        if (compressedValue > 0)
        {
            decompressedValue = (int)(65356 * ((log((double)compressedValue / (15)) / log(2))));
        }
        else
        {
            decompressedValue = (int)(-65356 * (log((double)(-compressedValue) / (15)) / log(2)));
        }
        return decompressedValue;
    }
    else if (outputbits == 2 && sample_bits == 16)
    {
        if (compressedValue > 0)
        {
            decompressedValue = (int)(65356 * ((log((double)compressedValue / (3)) / log(2))));
        }
        else
        {
            decompressedValue = (int)(-65356 * (log((double)(-compressedValue) / (3)) / log(2)));
        }
        return decompressedValue;
    }
    else if (outputbits == 4 && sample_bits == 8)
    {
        if (compressedValue > 0)
        {
            decompressedValue = (int)(127 * ((log((double)compressedValue / (15)) / log(2))));
        }
        else
        {
            decompressedValue = (int)(-127 * (log((double)(-compressedValue) / (15)) / log(2)));
        }
        return decompressedValue;
    }
    else if (outputbits == 2 && sample_bits == 8)
    {
        if (compressedValue > 0)
        {
            decompressedValue = (int)(127 * ((log((double)compressedValue / (3)) / log(2))));
        }
        else
        {
            decompressedValue = (int)(-127 * (log((double)(-compressedValue) / (3)) / log(2)));
        }
        return decompressedValue;
    }
}

int compressValue(int value, int outputbits, int sample_bits)
{
    int compressedValue;
    if (outputbits == 8 && sample_bits == 16)
    {
        if (value < 0)
        {
            value = -value;
            compressedValue = (int)(-127 * (pow(2, (double)value / 65356)));
        }
        else
        {
            compressedValue = (int)(127 * (pow(2, (double)value / 65356)));
        }
        return compressedValue;
    }
    else if (outputbits == 4 && sample_bits == 16)
    {
        if (value < 0)
        {
            value = -value;
            compressedValue = (int)(-15 * (pow(2, (double)value / 65356)));
        }
        else
        {
            compressedValue = (int)(15 * (pow(2, (double)value / 65356)));
        }
        return compressedValue;
    }
    else if (outputbits == 2 && sample_bits == 16)
    {
        if (value < 0)
        {
            value = -value;
            compressedValue = (int)(-3 * (pow(2, (double)value / 65356)));
        }
        else
        {
            compressedValue = (int)(3 * (pow(2, (double)value / 65356)));
        }
        return compressedValue;
    }
    else if (outputbits == 4 && sample_bits == 8)
    {
        if (value < 0)
        {
            value = -value;
            compressedValue = (int)(-15 * (pow(2, (double)value / 127)));
        }
        else
        {
            compressedValue = (int)(15 * (pow(2, (double)value / 127)));
        }
        return compressedValue;
    }
    else if (outputbits == 2 && sample_bits == 16)
    {
        if (value < 0)
        {
            value = -value;
            compressedValue = (int)(-3 * (pow(2, (double)value / 127)));
        }
        else
        {
            compressedValue = (int)(3 * (pow(2, (double)value / 127)));
        }
        return compressedValue;
    }
}

void parse_wav_file(FILE *fileptr)
{

    int read = 0;

    FILE *fp = fopen("output_of_test10.wav", "wb");
    // FILE *input_header = fopen("sample1.wav", "rb");
    unsigned char data_header[44];
    fread(&data_header, sizeof(data_header), 1, fileptr);
    fwrite(&data_header, sizeof(data_header), 1, fp);
    fseek(fileptr, -44, SEEK_CUR);

    FILE *intermediate = fopen("intermediate_of_test10.txt", "wb");
    // reading the header of input  wav file with fread() function

    read = fread(myheader.riff, sizeof(myheader.riff), 1, fileptr);
    // fwrite(&myheader.riff,sizeof(myheader.riff),1,fp);
    printf("(1-4): %s \n", myheader.riff);

    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    // fwrite(&buffer4,sizeof(buffer4),1,fp);
    printf("%u %u %u %u \n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    // convert little endian to big endian 4 byte int
    myheader.overall_size = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

    printf("(5-8) Overall size: bytes: %u, Kb:%u \n", myheader.overall_size, myheader.overall_size / 1024);

    read = fread(myheader.wave, sizeof(myheader.wave), 1, fileptr);
    // fwrite(&myheader.wave,sizeof(myheader.wave),1,fp);
    printf("(9-12) Wave marker: %s \n", myheader.wave);

    read = fread(myheader.fmt_chunk_marker, sizeof(myheader.fmt_chunk_marker), 1, fileptr);
    // fwrite(&myheader.fmt_chunk_marker,sizeof(myheader.fmt_chunk_marker),1,fp);
    printf("(13-16) Fmt marker: %s \n", myheader.fmt_chunk_marker);

    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    // fwrite(&buffer4,sizeof(buffer4),1,fp);
    printf("%u %u %u %u \n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    // converting little endian to big endian 4 byte integer
    myheader.length_of_fmt = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
    printf("(17-20) Length of Fmt header: %u \n", myheader.length_of_fmt);

    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    // fwrite(&buffer2,sizeof(buffer2),1,fp);
    printf("%u %u \n", buffer2[0], buffer2[1]);

    // converting little endian to big endian
    myheader.format_type = buffer2[0] | (buffer2[1] << 8);
    char format_name[10] = "";
    if (myheader.format_type == 1)
    {
        strcpy(format_name, "PCM");
    }
    else if (myheader.format_type == 6)
    {
        strcpy(format_name, "A-Law");
    }
    else if (myheader.format_type == 7)
    {
        strcpy(format_name, "Mu-law\n");
    }
    else
    {
        strcpy(format_name, "Unknown");
    }

    printf("(21-22) Format type: %u %s \n", myheader.format_type, format_name);

    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    // fwrite(&buffer2,sizeof(buffer2),1,fp);
    printf("%u %u \n", buffer2[0], buffer2[1]);

    myheader.channels = buffer2[0] | (buffer2[1] << 8);
    printf("(23-24) channels: %u \n", myheader.channels);

    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    // fwrite(&buffer4,sizeof(buffer4),1,fp);
    printf("%u %u %u %u \n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    myheader.sample_rate = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

    printf("(25-28) Sample rate: %u\n", myheader.sample_rate);

    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    // fwrite(&buffer4,sizeof(buffer4),1,fp);
    printf("%u %u %u %u \n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    myheader.byterate = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
    printf("(29-32) Byte Rate: %u, Bit Rate: %u \n", myheader.byterate, myheader.byterate * 8);

    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    // fwrite(&buffer2,sizeof(buffer2),1,fp);
    printf("%u %u \n", buffer2[0], buffer2[1]);

    myheader.block_align = buffer2[0] | (buffer2[1] << 8);
    printf("(33-34) Block Alignment: %u \n", myheader.block_align);

    read = fread(buffer2, sizeof(buffer2), 1, fileptr);
    // fwrite(&buffer2,sizeof(buffer2),1,fp);
    printf("%u %u \n", buffer2[0], buffer2[1]);

    myheader.bits_per_sample = buffer2[0] | (buffer2[1] << 8);
    printf("(35-36) Bits per sample: %u \n", myheader.bits_per_sample);

    read = fread(myheader.data_chunk_header, sizeof(myheader.data_chunk_header), 1, fileptr);
    // fwrite(&myheader.data_chunk_header,sizeof(myheader.data_chunk_header),1,fp);
    printf("(37-40) Data Marker: %s \n", myheader.data_chunk_header);

    read = fread(buffer4, sizeof(buffer4), 1, fileptr);
    // fwrite(&buffer4,sizeof(buffer4),1,fp);
    printf("%u %u %u %u \n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    myheader.data_size = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
    printf("(41-44) Size of data chunk: %u \n", myheader.data_size);

    // calculation of number of samples
    long num_samples = (8 * myheader.data_size) / (myheader.channels * myheader.bits_per_sample);
    printf("number of samples: %ld  \n", num_samples);

    long size_of_each_sample = (myheader.channels * myheader.bits_per_sample) / 8;

    printf("Size of each sample: %ld bytes \n", size_of_each_sample);

    // calculate duration of file
    // float duration_in_seconds = (float)myheader.overall_size/myheader.byterate;
    // printf("Approx Duration in  seconds = %f\n", duration_in_seconds);
    // printf("Approx Duration in h:m:s = %s \n",seconds_to_time(duration_in_seconds));

    // read each sample fron data chunk if the file format is PCM
    if (myheader.format_type == 1)
    { // PCM

        long i = 0;
        char data_buffer[size_of_each_sample];
        int size_is_correct = TRUE;

        // make sure that the bytes-per-sample is completely divisible by num.of channels

        long bytes_in_each_channel = (size_of_each_sample / myheader.channels);

        if ((bytes_in_each_channel * myheader.channels) != size_of_each_sample)
        {

            printf("Error: %ld x %u not equal to %ld \n", bytes_in_each_channel, myheader.channels, size_of_each_sample);

            size_is_correct = FALSE;
            return;
        }

        if (size_is_correct)
        {

            // the valid amplitude range for values based on the bits per sample

            long low_limit = 0l;

            long high_limit = 0l;

            switch (myheader.bits_per_sample)
            {

            case 8:

                low_limit = -128;

                high_limit = 127;

                break;

            case 16:

                low_limit = -32768;

                high_limit = 32767;

                break;

            case 32:

                low_limit = -2147483648;

                high_limit = 2147483647;

                break;
            }

            printf("............Valid range for data values : %ld to %ld ..............\n\n\n", low_limit, high_limit);
            printf("\n\n Which method you want to use for compression: 1. Non-linear companding 2. U-law companding\n");
            int choice;
            printf("Enter your choice:");
            scanf("%d",&choice);
            printf("choice = %d\n",choice);
            
            if (choice == 1)
            {
                printf("Non- Linear Companding\n");
                int outputbits;
                int flag = 0;
                if (bytes_in_each_channel == 1)
                {
                    printf("Enter the number of bits required for output code(out of the following): \n 1. 4 bits \n  2. 2 bits\n");

                    printf("please enter a valid option: ");
                    scanf("%d", &outputbits);

                    flag = 10 + outputbits;
                }
                else
                {
                    int choice;
                    printf("Enter the number of bits required for output code(out of the following): \n 1. 8 bits \n  2. 4 bits\n  3. 2 bits \n");

                    printf("please enter a valid option: ");
                    scanf("%d", &choice);

                    // printf("%d ",outputbits);
                    // flag = 20 + outputbits;
                    
                    switch(choice){
                        case 1:{
                            outputbits = 8;
                            break;
                        }
                        case 2:{
                            outputbits = 4;
                            break;
                        }
                        case 3:{
                            outputbits = 2;
                            break;
                        }
                        default:{
                            printf("Enter the valid choice\n");
                            return ;
                        }

                    }

                }
                // printf("hi");
                int *lookuptable;
                if (bytes_in_each_channel == 1)
                {
                    lookuptable = (int *)malloc(sizeof(int) * 256);
                    for (int i = -128; i < 128; i++)
                    {
                        lookuptable[i + 128] = compressValue(i, outputbits, 8);
                    }
                }
                else
                {
                    // printf("hi");
                    lookuptable = (int *)malloc(sizeof(int) * 65356);
                    if (!lookuptable)
                        printf("memory not allocated");
                    for (int i = -32768; i < 32768; i++)
                    {
                        lookuptable[i + 32768] = compressValue(i, outputbits, 16);
                        // printf("%d ",lookuptable[i+32768]);
                    }
                }

                int compressed_value = 0;
                int count24 = 0, i24 = 1;
                int count22 = 0, i22 = 3;
                int count14 = 0, i14 = 1;
                int count12 = 0, i12 = 3;
                unsigned char binary = 0;
                for (i = 1; i <= num_samples; i++)
                {

                    read = fread(data_buffer, sizeof(data_buffer), 1, fileptr);

                    if (read == 1)
                    {

                        unsigned int xchannels = 0;

                        int data_in_channel = 0;

                        int offset = 0; // move the offset for every iteration in the loop below

                        for (xchannels = 0; xchannels < myheader.channels; xchannels++)
                        {
                            if (bytes_in_each_channel == 2)
                            {
                                // printf("Hi");
                                data_in_channel = (data_buffer[offset] & 0x00ff) | (data_buffer[offset + 1] << 8);
                                if (outputbits == 8)
                                {
                                    compressed_value = lookuptable[data_in_channel + 32768];
                                    binary = compressed_value & 0xff;
                                    fwrite(&binary, 1, 1, intermediate);
                                }
                                else if (outputbits == 4)
                                {
                                    compressed_value = lookuptable[data_in_channel + 32768];
                                    if (count24 == 0)
                                    {
                                        binary = (compressed_value & 0x0F) << 4;
                                        count24++;
                                    }
                                    else if (count24 == 1)
                                    {
                                        binary |= (compressed_value & 0x0F);
                                        fwrite(&binary, 1, 1, intermediate);
                                        count24 = 0;
                                        i24 = 1;
                                    }
                                }
                                else if (outputbits == 2)
                                {
                                    compressed_value = lookuptable[data_in_channel + 32768];
                                    if (count22 == 0)
                                    {
                                        binary = (compressed_value & 0x03) << 6;
                                        count22++;
                                    }
                                    else if (count22 == 1)
                                    {
                                        binary |= (compressed_value & 0x03) << 4;
                                        count22++;
                                    }
                                    else if (count22 == 2)
                                    {
                                        binary |= (compressed_value & 0x03) << 2;
                                        count22++;
                                    }
                                    else if (count22 == 3)
                                    {
                                        binary |= (compressed_value & 0x03);
                                        fwrite(&binary, 1, 1, intermediate);
                                        count22 = 0;
                                    }
                                }
                            }
                            else if (bytes_in_each_channel == 1)
                            {
                                data_in_channel = data_buffer[offset] & 0x00ff;
                                data_in_channel -= 128; // in wave, 8-bit are unsigned, so shifting to signed
                                if (outputbits == 4)
                                {
                                    compressed_value = lookuptable[data_in_channel + 128];
                                    if (count14 == 0)
                                    {
                                        binary = (compressed_value & 0x0F) << 4;
                                        count14++;
                                    }
                                    else if (count14 == 1)
                                    {
                                        binary |= (compressed_value & 0x0F);
                                        fwrite(&binary, 1, 1, intermediate);
                                        count14 = 0;
                                        i14 = 1;
                                    }
                                }
                                else if (outputbits == 2)
                                {
                                    compressed_value = lookuptable[data_in_channel + 128];
                                    if (count12 == 0)
                                    {
                                        binary = (compressed_value & 0x03) << 6;
                                        count12++;
                                    }
                                    else if (count12 == 1)
                                    {
                                        binary |= (compressed_value & 0x03) << 4;
                                        count12++;
                                    }
                                    else if (count12 == 2)
                                    {
                                        binary |= (compressed_value & 0x03) << 2;
                                        count12++;
                                    }
                                    else if (count12 == 3)
                                    {
                                        binary |= (compressed_value & 0x03);
                                        fwrite(&binary, 1, 1, intermediate);
                                        count12 = 0;
                                    }
                                }
                            }

                            offset += bytes_in_each_channel;

                            if (data_in_channel < low_limit || data_in_channel > high_limit)
                            {
                                printf("**value out of range\n");
                            }
                        }
                        // printf("\n");
                    }
                    else
                    {
                        printf("Error reading file. %d bytes\n", read);
                        break;
                    }
                } //    for (i =1; i <= num_samples; i++) {
                fclose(intermediate);
                FILE *intermediate_file_pointer = fopen("intermediate_of_test10.txt", "rb");
                unsigned char reader;
                int decompressed_value;
                int count = 0;
                int reader_decimal;
                for (int i = 0; i < (myheader.channels) * num_samples; i++)
                {
                    read = fread(&reader, sizeof(reader), 1, intermediate_file_pointer);
                    if (read)
                    {
                        if (outputbits == 8 && bytes_in_each_channel == 2)
                        {
                            reader_decimal = (int)reader;
                            decompressed_value = decompressValue(reader_decimal, outputbits, 16);
                            unsigned char binary[2];
                            binary[1] = (decompressed_value >> 8) & 0x00ff;
                            binary[0] = decompressed_value & 0x00ff;
                            fwrite(&binary, 2, 1, fp);
                        }
                        else if (outputbits == 4 && bytes_in_each_channel == 2)
                        {
                            for (int i = 1; i <= 2; i++)
                            {
                                reader_decimal = (int)((reader >> 4 * (2 - i)) & 0x0f);
                                decompressed_value = decompressValue(reader_decimal, outputbits, 16);
                                unsigned char binary[2];
                                binary[1] = (decompressed_value >> 8) & 0x00ff;
                                binary[0] = decompressed_value & 0x00ff;
                                fwrite(&binary, 2, 1, fp);
                            }
                        }
                        else if (outputbits == 2 && bytes_in_each_channel == 2)
                        {
                            int j = 3;
                            for (int i = 1; i <= 4; i++)
                            {
                                reader_decimal = (int)((reader >> 2 * (j--)) & 0x03);
                                decompressed_value = decompressValue(reader_decimal, outputbits, 16);
                                unsigned char binary[2];
                                binary[1] = (decompressed_value >> 8) & 0x00ff;
                                binary[0] = decompressed_value & 0x00ff;
                                fwrite(&binary, 2, 1, fp);
                            }
                        }
                        else if (outputbits == 4 && bytes_in_each_channel == 1)
                        {
                            for (int i = 1; i <= 2; i++)
                            {
                                reader_decimal = (int)((reader >> 4 * (2 - i)) & 0x0f);
                                decompressed_value = decompressValue(reader_decimal, outputbits, 8);
                                unsigned char binary[1];
                                binary[0] = decompressed_value & 0x00ff;
                                fwrite(&binary, 2, 1, fp);
                            }
                        }
                        else if (outputbits == 2 && bytes_in_each_channel == 1)
                        {
                            int j = 3;
                            for (int i = 1; i <= 4; i++)
                            {
                                reader_decimal = (int)((reader >> 4 * (j--)) & 0x0f);
                                decompressed_value = decompressValue(reader_decimal, outputbits, 8);
                                unsigned char binary[1];
                                binary[0] = decompressed_value & 0x00ff;
                                fwrite(&binary, 2, 1, fp);
                            }
                        }
                    }
                }
                printf(".......................................Closing file.....................................\n");
                fclose(fileptr);
                fclose(fp);
            }
            else if(choice == 2)
            {
                printf(" mu law companding\n");

                int16_t sample;
                int8_t encoded_sample;
                for (i = 1; i <= num_samples; i++)
                {

                    read = fread(data_buffer, sizeof(data_buffer), 1, fileptr);

                    if (read == 1)
                    {

                        unsigned int xchannels = 0;

                        int data_in_channel = 0;

                        int offset = 0; // move the offset for every iteration in the loop below

                        for (xchannels = 0; xchannels < myheader.channels; xchannels++)
                        {
                            if (bytes_in_each_channel == 2)
                            {
                                data_in_channel = (int16_t)(data_buffer[offset] & 0x00ff) | (data_buffer[offset + 1] << 8);
                                encoded_sample = MuLaw_Encode(data_in_channel);
                                fwrite(&encoded_sample, sizeof(int8_t), 1, intermediate);
                            }
                            else if (bytes_in_each_channel == 1)
                            {
                                data_in_channel = data_buffer[offset] & 0x00ff;

                                data_in_channel -= 128; // in wave, 8-bit are unsigned, so shifting to signed
                            }

                            offset += bytes_in_each_channel;

                            if (data_in_channel < low_limit || data_in_channel > high_limit)

                                printf("**value out of range\n");
                        }
                    }
                    else
                    {
                        printf("Error reading file. %d bytes\n", read);
                        break;
                    }
                } //    for (i =1; i <= num_samples; i++) {
                fclose(intermediate);
                FILE *intermediate_file_pointer = fopen("intermediate_of_test10.txt", "rb");
                int8_t reader;
                int count = 0;
                for (int i = 0; i < 2 * num_samples; i++)
                {
                    read = fread(&reader, sizeof(int8_t), 1, intermediate_file_pointer);
                    if (read)
                    {
                        int16_t decoded_sample = MuLaw_Decode(reader);
                        fwrite(&decoded_sample, sizeof(int16_t), 1, fp);
                    }
                }
                printf("Closing file..\n");
                fclose(fileptr);
                fclose(fp);
            }
            else{
                printf("Enter the valid choice\n");
                return;
            }
        }
    }
    return;
}

void audioCompression(char *filename)
{
    // char *filename = (char *)malloc(sizeof(char) * 1024);
    if (!filename)
    {
        printf("Error in mallocation of 1024 bytes buffer!\n");
        exit(1);
    }

    // open file
    printf("Opening file..");
    printf("\n");
    // sleep(1000);

    FILE *fileptr = fopen(filename, "rb");
    if (!fileptr)
    {
        printf("Unable to open file, somry :(\n");
        exit(1);
    }

    parse_wav_file(fileptr);

    // free(filename);

    return;
}
