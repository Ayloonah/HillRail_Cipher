/*
Assignment:
hillrail - Hill cipher followed by Rail Fence transposition
Author: Ayloonah
Language: C
To Compile:
gcc -O2 -std=c11 -o hillrail hillrail.c
To Execute (on Eustis):
./ hillrail encrypt key.txt plain.txt <depth>
where:
key.txt = key matrix file
plain.txt = plaintext file
<depth> = integer >= 2 (Rail Fence)
Notes:
- Input is 8-bit ASCII; process only A - Z (uppercase).
- Tested on Eustis.
Class: CIS3360 - Security in Computing - Fall 2025
Instructor: Dr. Jie Lin
Due Date: September 12th 2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ROW_LENGTH 81

// Struct to store the encription key:
typedef struct key {
    int dimension;
    int** matrix;
    int count;
} Key;

// Struct to store the arguments passed when program is invoked:
typedef struct arguments {
    char* keyPath;
    char* ptPath;
    int railDepth;
} Args;

// Struct to store the hill cipher text, the row count, and the character count:
typedef struct hillText {
    char** text;
    int rowCount;
    int charCount;
} HillCipher;

// Function to read the encription key file, store the key in a key struct, and print it to the console:
    // In: Str with file location.
    // Out: Key struct ptr
Key* getKey(char* filePath){
    // Creating a struct ptr and allocating memory to it:
    Key* k = malloc(sizeof(Key));
    
    // Accessing the encription key file:
    FILE* fp = fopen(filePath, "r");

    if(fp == NULL){
        printf("No file found. Exiting.");
        exit(EXIT_FAILURE);
    }

    // Grabbing the matrix's dimension, allocating memory for the matric int array, and storing the matrix's values in the struct:
    fscanf(fp, "%d", &k->dimension);
    k->matrix = calloc(k->dimension, sizeof(int*));
    for(int i = 0; i < k->dimension; i++) {
        k->matrix[i] = calloc(k->dimension, sizeof(int));
        for(int j = 0; j < k->dimension; j++){
            fscanf(fp, "%d", &k->matrix[i][j]);
        }
    }

    // Storing the number of matrix values:
    k->count = (k->dimension * k->dimension);

    // Closing the file:
    fclose(fp);

    // Printing the key to the console: 
    printf("Key matrix:\n");
    for(int i = 0; i < k->dimension; i++){
        for(int j = 0; j < k->dimension; j++){
            if(j == (k->dimension - 1)){
                printf("%d\n", k->matrix[i][j]);
            } else {
                printf("%d\t", k->matrix[i][j]);
            }
        }
    }
    printf("\n");

    return k;
}

// Function to convert a char to an int, apply the key matrix to it, convert back to a char, and return the encrypted char.
    // In: Key struct ptr, char to be encrypted.
    // Out: N/A
void hillEncrypt(Key* k, HillCipher* hc){
    // Initializing required variables:
    int tracker = 0;
    int chunkVals[k->dimension], processedVals[k->dimension];

    while (tracker < hc->charCount){
        // Grabbing the first nbr of char = to k->dimension:
        for(int i = 0; i < k->dimension; i++){
            int pos = tracker + i;
            int row = pos / (ROW_LENGTH - 1);
            int col = pos % (ROW_LENGTH - 1);
            chunkVals[i] = hc->text[row][col] - 'A';
        }

        // Multiplying the key matrix with the char values and applying modulo 26:
        for(int r = 0; r < k->dimension; r++){
            processedVals[r] = 0;
            for(int c = 0; c < k->dimension; c++){
                processedVals[r] += k->matrix[r][c] * chunkVals[c];
            }
            processedVals[r] %= 26;
        }

        //  Storing the associated character back into the original 2D char array:
        for (int i = 0; i < k->dimension; i++) {
            int pos = tracker + i;
            int row = pos / (ROW_LENGTH - 1);
            int col = pos % (ROW_LENGTH - 1);
            hc->text[row][col] = processedVals[i] + 'A';
        }
        
        tracker += k->dimension;
    }
}

// Function to apply the rail transposition cipher to the already hill ciphered text: 
    // In: Key struct ptr, hillciper struct ptr.
    // Out: N/A
void railEncrypt(HillCipher* hc, int depth){
    // For depth of less than 2, skipping the rail transposition and printing the text:
    if(depth < 2){
        int pos = 0;
        printf("Ciphertext:\n");
        for(int i = 0; (i <= hc->rowCount) && (pos < hc->charCount); i++){
            for(int j = 0; (j < (ROW_LENGTH - 1)) && (pos < hc->charCount); j++){
                printf("%c", hc->text[i][j]);
                pos++;
                if(((pos % 80) == 0) && (pos != hc->charCount)){
                    printf("\n");
                }
            }                
        }
        printf("\n\n");

        return;
    }
    
    // Applying the rail transposition for every other depth values, starting with:
    // Creating a 2D char array with depth number of rows and length charCount/depth:        
    int* railLength = calloc(depth, sizeof(int));
    char** rail = calloc(depth, sizeof(char*));
    for(int i = 0; i < depth; i++){
        rail[i] = calloc(hc->charCount + 1, sizeof(char));
    }

    // Grabbing characters from the HiillCipher struct text and putting them into the right spot in the rail char array:
    int railRow = 0, direction = 1;
    for(int i  = 0; i < hc->charCount; i++){
        int row = (i / (ROW_LENGTH - 1)), col = (i % (ROW_LENGTH - 1));
        rail[railRow][railLength[railRow]] = hc->text[row][col];
        railLength[railRow]++;
        railRow += direction;
        if(railRow == (depth - 1) || railRow == 0){
            direction *= -1;
        }
    }

    // Printing the ciphertext:
    printf("Ciphertext:\n");
    int pos = 0;
    for(int i = 0; i < depth; i++){
        for(int j = 0; j < railLength[i]; j++){
            if((pos > 0) && ((pos % 80) == 0)){
                printf("\n");
            }
            printf("%c", rail[i][j]);
            pos++;
        }
    }
    printf("\n\n");

    // Freeing memory:
    for(int i = 0; i < depth; i++){
        free(rail[i]);
    }
    free(rail);
    free(railLength);
}

// Function to read the plain text file, remove unwanted characters, converting what's left to uppercase, adding padding if required.
// Will print the plain text, apply the hill cipher to it, and store it in a 2D char array before returning it. 
    // In: File path str, ptr to Key struct. 
    // Out: 2D char array with the processed plain text.
HillCipher* getPlainText(char* filePath, Key* key){
    // Creating a struct ptr and allocating memory to it:
    HillCipher* hc = malloc(sizeof(HillCipher));

    // Accessing the encription key file:
    FILE* fp = fopen(filePath, "r");

    if(fp == NULL){
        printf("No file found. Exiting.");
        exit(EXIT_FAILURE);
    }
    
    // Grabbing each char from the file one at a time, excluding anything unwanted, making sure it's uppercase, 
    // applying the hill cipher to it, and storing it in a character array:
    char curr;
    hc->rowCount = 0;
    hc->charCount = 0;
    int col = 0;

    hc->text = malloc(sizeof(char*));
    hc->text[hc->rowCount] = calloc(ROW_LENGTH, sizeof(char));
    
    while((curr = fgetc(fp)) != EOF){
        if(isalpha(curr)){
            if(islower(curr)) {
                curr = toupper(curr);
            }
            hc->text[hc->rowCount][col] = curr;
            col++;
            hc->charCount++;
        }
        if(col == (ROW_LENGTH - 1)){
            col = 0;
            hc->rowCount++;
            hc->text = realloc(hc->text, (hc->rowCount + 1) * sizeof(char*));
            hc->text[hc->rowCount] = calloc(ROW_LENGTH, sizeof(char));
        }
    }

    // Printing the plaintext:
    int pos = 0;
    printf("Plaintext:\n");
    for(int i = 0; (i <= hc->rowCount) && (pos < hc->charCount); i++){
        for(int j = 0; (j < (ROW_LENGTH - 1)) && (pos < hc->charCount); j++){
            printf("%c", hc->text[i][j]);
            pos++;
            if((pos % 80) == 0){
                printf("\n");
            }
        }                
    }
    printf("\n\n");

    // Check if padding is required, and if so, add it:
    int pad = 0;
    if((hc->charCount % key->dimension) != 0) {
        pad = key->dimension - (hc->charCount % key->dimension);
    }
    if(pad != 0){
        for(int i = 0; i < pad; i++){
            hc->text[hc->rowCount][col] = 'X';
            col++;
            if(col == (ROW_LENGTH - 1)){
                col = 0;
                hc->rowCount++;
                hc->text = realloc(hc->text, (hc->rowCount + 1) * sizeof(char*));
                hc->text[hc->rowCount] = calloc(ROW_LENGTH, sizeof(char));
            }
        }
        hc->charCount += pad;
    }

    fclose(fp);

    return hc;
}

// Function to free all allocated memory.
    // In: Key struct ptr, hillciper struct ptr.
    // Out: N/A
void freeMemory(Key* k, HillCipher* hc){
    for(int i = 0; i < k->dimension; i++){
        free(k->matrix[i]);
    }
    free(k->matrix);
    free(k);

    for(int i = 0; i < hc->rowCount + 1; i++){
        free(hc->text[i]);
    }
    free(hc->text);
    free(hc);
}

int main(int argc, char* argv[]){
    // Initializing required variables:
    Args args;
    Key* key;
    HillCipher* hillText;

    // Rejecting an invalid argument count:
    if(argc != 5){
        printf("Invalid argument count. Exiting.");
        for(int i = 0; i < argc; i++){
            printf("%d %s", argc, argv[i]);
        }
        return -1;
    }

    // Rejecting any command other than encrypt:
    if(strcmp(argv[1], "encrypt") != 0){
        printf("Invalid command. Exiting.");
        return -1;
    }

    // Storing arguments in a struct:
    args.keyPath = argv[2];
    args.ptPath = argv[3];
    args.railDepth = atoi(argv[4]);
    
    // Function calls to read the files and print to console:
    key = getKey(args.keyPath);
    hillText = getPlainText(args.ptPath, key);

    // Now applies hill cipher: 
    hillEncrypt(key, hillText);

    // Now applies the rail transposition cipher:
    railEncrypt(hillText, args.railDepth);

    // Prints the depth:
    printf("Depth: %d\n", args.railDepth);

    // Freeing memory:
    freeMemory(key, hillText);

    return 0;
}