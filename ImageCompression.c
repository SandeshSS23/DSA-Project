#include "ImageCompression.h"

unsigned char *BMPFullHeader;
unsigned char *arrImage;
unsigned char *arr_decompress;
int imageWidth;
double threshold = 1500.0;
QuadNode root;

unsigned int HextoDec(unsigned char c){

    unsigned int dec;
    dec = ((c >> 4) * 16) + (c & 15);
    return dec;
}

int FourHexBytestoDec(unsigned char *c){
    
    int number =HextoDec(c[3])*( 1 << 24) + HextoDec(c[2])*(1 << 16) + HextoDec(c[1])*(1 << 8) + HextoDec(c[0]);

    return number;
}


void ReadBMPFileHeader(BMPFileheader *fileHeader,int len, FILE *fp, int* fileSize, int *offset){

    fread(fileHeader,sizeof(unsigned char),len,fp);

    printf("bfType 1 = %c\n",fileHeader->bfType1);
    printf("bfType 2 = %c\n",fileHeader->bfType2);

    *fileSize = FourHexBytestoDec(fileHeader->bfSize);
    // printf("bfSize = %d\n",*fileSize);

    printf("reserved 1 = %d\n", fileHeader->bfReserved1);
    printf("reserved 2 = %d\n", fileHeader->bfReserved2);

    *offset = FourHexBytestoDec(fileHeader->bfOffset);
    // printf("bfOffset = %d\n",*offset);

}

void ReadBMPInfoHeader(BMPInfoHeader *infoHeader,int len, FILE* fp,int *w,int *h){

    fread(infoHeader,sizeof(unsigned char),len,fp);
    *w = FourHexBytestoDec(infoHeader->width);
    *h = FourHexBytestoDec(infoHeader->height);

}

void ReadFullImage(FILE* fp,int width,int height){

    imageWidth = width;

    arrImage = (unsigned char*)malloc(sizeof(unsigned char)*width*height);

    fread(arrImage,sizeof(unsigned char),width*height,fp);

}

void print_image_array(unsigned char *arrImage,int width,int height){

    
    FILE* file = fopen("bmpread.txt", "w");

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char ch = (unsigned char)arrImage[i * width + j];
            // printf("%x ", ch);
            fprintf(file,"%x",ch);
        }
        fprintf(file,"\n");
        // printf("\n");
    }


}

void SetAverage_MSE(SubSection *image){

    unsigned long int sum = 0;
    unsigned long int square_sum = 0;

    int n = image->w*image->h;

    for(int i=0;i<image->h;i++){
        for(int j = 0;j<image->w;j++){
            int index = (image->tly+ i)*imageWidth + j + image->tlx; 

            sum+=(unsigned long int)arrImage[index];
            square_sum+=(unsigned long int)(arrImage[index]*arrImage[index]);
        }
    }   

    double average = (sum*1.0)/n;

    double square_average = (square_sum*1.0)/n;

    image->avg = (unsigned char)round(average);
    image->mse = square_average - (average * average);

}

void displaySubSection(SubSection *image,FILE *inter){
    if(inter){
        fprintf(inter,"%d, %d, %d, %d, %x\n",image->tlx,image->tly,image->w,image->h,(unsigned char)image->avg);
    }

    printf("\nTLX = %d, TLY = %d, Width = %d, Height = %d \n", image->tlx, image->tly, image->w, image->h);

    printf("\n Average Color = %x\n",(unsigned char)image->avg);
    printf("MSE Value = %lf\n",image->mse);
}

void splitImageInChilds(SubSection *image, childSections *C){

    // devide the image into four childs.

    SubSection *Node1 = (SubSection *)malloc(sizeof(SubSection));
    Node1->tlx = image->tlx;
    Node1->tly = image->tly;
    Node1->h = image->h/2;
    Node1->w = image->w/2;
    C->ChildNW = Node1;

    SubSection *Node2 = (SubSection *)malloc(sizeof(SubSection));
    Node2->tlx = image->tlx + (image->w/2);
    Node2->tly = image->tly;
    Node2->h = image->h/2;
    Node2->w = image->w/2;
    C->ChildNE = Node2;

    SubSection *Node3 = (SubSection *)malloc(sizeof(SubSection));
    Node3->tlx = image->tlx;
    Node3->tly = image->tly + (image->h/2);
    Node3->h = image->h/2;
    Node3->w = image->w/2;
    C->ChildSW = Node3;

    SubSection *Node4 = (SubSection *)malloc(sizeof(SubSection));
    Node4->tlx = image->tlx + (image->w/2);
    Node4->tly = image->tly + (image->h/2);
    Node4->h = image->h/2;
    Node4->w = image->w/2;
    C->ChildSE = Node4;

    return;
    
}

void divideImage(QuadNode* root){

    childSections C;
    splitImageInChilds(root->Image,&C);

    QuadNode *node1 = (QuadNode *)malloc(sizeof(QuadNode));
    node1->parent = root;
    node1->ChildNW = (QuadNode *)NULL;
    node1->ChildNE = (QuadNode *)NULL;
    node1->ChildSW = (QuadNode *)NULL;
    node1->ChildSE = (QuadNode *)NULL;

    root->ChildNW = node1;
    node1->Image = C.ChildNW;

    SetAverage_MSE(C.ChildNW);
    if(node1->Image->mse > threshold){
        divideImage(node1);
    }


    QuadNode *node2 = (QuadNode *)malloc(sizeof(QuadNode));
    node2->parent = root;
    node2->ChildNE = (QuadNode *)NULL;
    node2->ChildNW = (QuadNode *)NULL;
    node2->ChildSE = (QuadNode *)NULL;
    node2->ChildSW = (QuadNode *)NULL;

    root->ChildNE = node2;
    node2->Image = C.ChildNE;

    SetAverage_MSE(C.ChildNE);
    if(node2->Image->mse > threshold){
        divideImage(node2);
    }

    QuadNode *node3 = (QuadNode *)malloc(sizeof(QuadNode));
    node3->parent = root;
    node3->ChildNE = (QuadNode *)NULL;
    node3->ChildNW = (QuadNode *)NULL;
    node3->ChildSE = (QuadNode *)NULL;
    node3->ChildSW = (QuadNode *)NULL;

    root->ChildSW = node3;
    node3->Image = C.ChildSW;

    SetAverage_MSE(C.ChildSW);
    if(node3->Image->mse > threshold){
        divideImage(node3);
    }

    QuadNode *node4 = (QuadNode *)malloc(sizeof(QuadNode));
    node4->parent = root;
    node4->ChildNE = (QuadNode *)NULL;
    node4->ChildNW = (QuadNode *)NULL;
    node4->ChildSE = (QuadNode *)NULL;
    node4->ChildSW = (QuadNode *)NULL;

    root->ChildSE = node4;
    node4->Image = C.ChildSE;

    SetAverage_MSE(C.ChildSE);
    if(node4->Image->mse > threshold){
        divideImage(node4);
    }

}

void postOrderTraversal(QuadNode *root,int level,FILE *inter){
    if(root == (QuadNode *)NULL){
        return;
    }

    if(root->ChildNW == (QuadNode *)NULL && root->ChildNE == (QuadNode *)NULL &&root->ChildSW == (QuadNode *)NULL && root->ChildSE == (QuadNode *)NULL){
        printf(" ************* %d *********\n",level);
        displaySubSection(root->Image,inter);
        printf(" ************* \n");
        return;
    }

    if(root->ChildNW != (QuadNode *)NULL){
        postOrderTraversal(root->ChildNW,level+1,inter);
    }

    if(root->ChildNE != (QuadNode *)NULL){
        postOrderTraversal(root->ChildNE,level+1,inter);
    }

    if(root->ChildSW != (QuadNode *)NULL){
        postOrderTraversal(root->ChildSW,level+1,inter);
    }

    if(root->ChildSE != (QuadNode *)NULL){
        postOrderTraversal(root->ChildSE,level+1,inter);
    }
}

void freeQuadTree(QuadNode *root){
    if(root == (QuadNode *)NULL){
        return;
    }

    freeQuadTree(root->ChildNE);
    freeQuadTree(root->ChildNW);
    freeQuadTree(root->ChildSW);
    freeQuadTree(root->ChildSE);
    free(root->Image);
    free(root);
}


void imageCompression(char *filename){

    FILE *fp;
    BMPFileheader fileHeader;
    int fileSize;
    int fileOffset;

    double avg = 0.0;
    double mse = 0.0;
    printf("\nImage Compression called for the file named %s\n",filename);


    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("The file %s cannot be opened.\n", filename);
        return;
    }

    printf("\nEnter the Threshold value for compression. More the threshold value more will be distortion in the decompressed file = ");
    scanf("%lf",&threshold);

    printf("Threshold value set as %lf\n", threshold);

    // Read header of BMP image by using two functions
    ReadBMPFileHeader(&fileHeader,sizeof(fileHeader),fp,&fileSize,&fileOffset);
    printf("FileSize = %d\n", fileSize);
    printf("FileOffset = %d\n", fileOffset);

    int width;
    int height;

    BMPInfoHeader infoHeader;
    ReadBMPInfoHeader(&infoHeader,sizeof(infoHeader),fp,&width,&height);

    printf("Image Width = %d \nImage Heigt = %d \n",width,height);

    fseek(fp,(long int)0,SEEK_SET);  // take file pointer to start of file.

// unsigned char *
    //Now by using offset read full header and store in intermediate BMPheader.bin file   

    BMPFullHeader = (unsigned char *)malloc(sizeof(unsigned char)*fileOffset);

    fread(BMPFullHeader,sizeof(unsigned char),fileOffset,fp);

    FILE *intermediateFile = fopen("BMPHeader.bin","w");

    fwrite(BMPFullHeader,sizeof(unsigned char),fileOffset,intermediateFile);
    fclose(intermediateFile);

    // Now read the whole image data which is stored after header.

    ReadFullImage(fp,width,height);
    fclose(fp);
    // print_image_array(arrImage,width,height);


    // Quad Tree Implementation starts.

    SubSection *FullImage = malloc(sizeof(SubSection));
    FullImage->tlx = 0;
    FullImage->tly = 0;
    FullImage->w = width;
    FullImage->h = height;

    SetAverage_MSE(FullImage);
    displaySubSection(FullImage,NULL);

    // initialise root node.
    root.Image = FullImage;
    root.ChildNE = (QuadNode *)NULL;
    root.ChildNW = (QuadNode *)NULL;
    root.ChildSE = (QuadNode *)NULL;
    root.ChildSW = (QuadNode *)NULL;
    root.parent = (QuadNode *)NULL;

    // now the quad tree is formed by dividing the image into four parts and the division is stopped when the mse value of the current root is less than thresshold value.

    if(root.Image->mse > threshold){
        divideImage(&root);
    }

    FILE *inter = fopen("intermediate.txt","w");
    fprintf(inter,"%d,%d\n",width,height);
    postOrderTraversal(&root,0,inter);
    fclose(inter);
    free(BMPFullHeader);



// ************************         DECOMPRESSION **********************************

    avg = 0.0;
    mse = 0.0;
    fp = fopen("BMPHeader.bin", "rb");
    if (fp == NULL)
    {
        printf("The file BMPHeader.bin cannot be opened.\n");
        return;
    }

    ReadBMPFileHeader(&fileHeader,sizeof(fileHeader),fp,&fileSize,&fileOffset);
    printf("FileSize = %d\n", fileSize);
    printf("FileOffset = %d\n", fileOffset);

    // BMPInfoHeader infoHeader;
    ReadBMPInfoHeader(&infoHeader,sizeof(infoHeader),fp,&width,&height);
    printf("Height = %d, Width = %d\n", height, width);


    fseek(fp, (long int)0, SEEK_SET);
    BMPFullHeader = (unsigned char *)malloc(sizeof(unsigned char) * fileOffset);
    fread(BMPFullHeader, sizeof(unsigned char), fileOffset, fp);
    fclose(fp);

    char *name; // declare a pointer to char
    printf("Enter the name of Output Image File : ");
    scanf("%ms", &name);

    inter = fopen(name, "wb");
    fwrite(BMPFullHeader, sizeof(unsigned char), fileOffset, inter);

    FILE *qfp = fopen("intermediate.txt", "r");
    int w, h;
    fscanf(qfp, "%d,%d", &w, &h);

    if ((w != width) || (h != height))
    {
        printf("Mismatch between bin file and QTree file\n");
        // return (-1);
        return;
    }

    arr_decompress = (unsigned char *)malloc(sizeof(unsigned char) * height * width);

    int tlx, tly;
    unsigned int val;

    while (fscanf(qfp, "%d,%d,%d,%d,%x", &tlx, &tly, &w, &h, &val) != EOF)
    {
        // printf("Read - %d %d %d %d %x\n", tlx, tly, ss_w, ss_h, val);
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                int indl = (tly + i) * width + j + tlx;
                arr_decompress[indl] = (unsigned char)val;
            }
        }
    }

    fclose(qfp);

    print_image_array(arr_decompress, width, height);
    fwrite(arr_decompress, sizeof(unsigned char), height * width, inter);

    fclose(inter);
    free(BMPFullHeader);
    free(arrImage);
    free(arr_decompress);

    freeQuadTree(&root);
    

}