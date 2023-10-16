#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include<math.h>

#include "ImageCompression.h"
#include "AudioCompression.h"
#include "textcomp.h"


int main(int argc, char **argv)
{

    if (argc < 2)
    {
        printf("Please Enter the name of file\n");
        return 1;
    }

    char *filename = argv[1];

    printf("Name of the file is %s\n", filename);

    char *lastThree = (char *)malloc(sizeof(char) * 3);

    int i = 0;
    while (filename[i] != '.')
    {
        i++;
    }

    lastThree[0] = filename[++i];
    lastThree[1] = filename[++i];
    lastThree[2] = filename[++i];

    int flag = 0;
    int check = strcmp(lastThree, "bmp");
    if (!check)
    {
        flag = 1;
        printf("\nThe file is Image file\n");
    }

    check = strcmp(lastThree, "wav");
    if (!check)
    {
        flag = 2;
        printf("\nThe file is Audio file\n");
    }
    check = strcmp(lastThree, "txt");
    if (!check)
    {
        flag = 3;
        printf("\nThe file is text file\n");
    }


    if (flag == 0)
    {
        printf("\nThe file name entered is not valid. \n\n For image file please enter the name in format .bmp \n For Audio File please enter the name in format .wav \n For text files, enter the file in .txt format");
        return 1;
    }

    // switch case for flag;

    switch (flag){
        case 1 :{
            // Image Compression;
            imageCompression(filename);
            break;
        }

        case 2:{
            // Audio Compression
            audioCompression(filename);
            break;
        }
        
        case 3:{
            // Text Compression
            textCompression(filename);
            break;
        }
        
        default:
            break;
    }

    return 0;
}
