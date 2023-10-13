#include <stdio.h>
#include <stdlib.h>

// Structure to hold grayscale pixel data
typedef struct
{
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray **readPGM(const char *filename, int *width, int *height);

// Function to write a 2D matrix as a PGM image
void writePGM(const char *filename, PixelGray **matrix, int *width, int *height);

// Function to threshold the image matrix
PixelGray **threshold(PixelGray **matrix, int *width, int *height);

// Function to rotate the image matrix
PixelGray **rotate(PixelGray **matrix, int *width, int *height);

// main function - DO NOT MODIFY
int main()
{
    int width, height; // variable to hold width and height. Use reference in other functions

    PixelGray **image_original = readPGM("lenna.pgm", &width, &height);
    // Now you have the grayscale image data in the 'image_original' 2D array

    // Access pixel data using image[row][col].value
    // For example, to access the pixel at row=2, col=3:
    // unsigned char pixel_value = image[2][3].value;

    // Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray **image_thresh = threshold(image_original, &width, &height);
    // write the image data as "threshold.pgm"
    writePGM("threshold.pgm", image_thresh, &width, &height);

    // Create a new 2D array 'image_rotate' to store the rotated image
    PixelGray **image_rotate = rotate(image_original, &width, &height);
    // write the image data as "rotate.pgm"
    writePGM("rotate.pgm", image_rotate, &width, &height);

    image_rotate = rotate(image_rotate, &width, &height);
    // write the image data as "rotate_again.pgm"
    writePGM("rotate_again.pgm", image_rotate, &width, &height);

    // Free the allocated memory when you're done
    for (int i = 0; i < height; ++i)
    {
        free(image_original[i]);
        free(image_thresh[i]);
        free(image_rotate[i]);
    }
    free(image_original);
    free(image_thresh);
    free(image_rotate);
    return 0;
}

PixelGray **readPGM(const char *filename, int *width, int *height)
{
    // Opens the file specified by the pointer in the paramater and reads it as a binary file and creates a file pointer.
    FILE *file = fopen(filename, "rb");
    // Check if the file can be opened for reading
    if (file == NULL)
    {
        printf("File cannot be opened");
        return NULL;
    }
    // Creates a temporary buffer to store the header metadeta using the fgets statements
    char temp_buffer[200];
    // This is another character buffer to store each 8 bits from the binary P5 image using fread
    unsigned char buffer;
    // This repositions the file pointer to the beginning of the file to ensure consistency when reading from the file
    rewind(file);

    // This gets the first string of the header section up until the whitespace character. Thus this will extract "P5".
    fgets(temp_buffer, 200, file);
    // This will scan for the next integer in the header section, which will represent the width of the P5 image. This will store the result in the memory address referened by the width pointer.
    fscanf(file, "%d", width);
    // This will scan for the next integer in the header section, which will represent the height of the P5 image. This will store the result in the memory address referened by the height pointer.
    fscanf(file, "%d", height);
    // This will extract the newline character after the height metadta and store it in the temporary char array buffer.
    fgets(temp_buffer, 200, file);
    // This will extract the maximum pixel value and store it in the temporary char array buffer. The file pointer now points to the final newline character in the header section.
    fgets(temp_buffer, 200, file);

    // This initializes the 2D PixelGray array using **, or double pointer. This intiial row will have length equal to *height and each cell is the size of an unsigned char (PixelGray)
    PixelGray **matrix = (PixelGray **)malloc(*height * sizeof(PixelGray *));

    // This for loop will create another array using a single pointer * for each element of the array above. The size of each of these single pointer arrays is *width. The size of each cell is PixelGray (8 bits).
    // Using this for loop allows us to create the 2D dynamically allocated array
    for (int i = 0; i < *height; i++)
    {
        matrix[i] = (PixelGray *)malloc(*width * sizeof(PixelGray));
    }

    // Iterate through each row of the 2d array
    for (int row = 0; row < *height; ++row)
    {
        // Iterate through each column of the current row
        for (int col = 0; col < *width; ++col)
        {
            // This will read each 8 bits (sizeof(unsigned char)) of the pixel data from the P5 image. It will store the character inside of the buffer variable.
            fread(&buffer, sizeof(unsigned char), 1, file);
            // Stores the pixel value stored in buffer into the next available element of the 2D matrix. Since each element is a PixelGray type, we use matrix[row][col].value = buffer to store each pixel value unsigned character.
            matrix[row][col].value = buffer;
        }
    }
    // Close the P5 file
    fclose(file);
    // Returns the pointer of the dynamically allocated matrix created above
    return matrix;
}

void writePGM(const char *filename, PixelGray **matrix, int *width, int *height)
{
    // Creates a fie pointer object after attempting to open the file with name *filename for writing binary.
    FILE *file = fopen(filename, "wb");
    // Check if the binary file can be opened for writing
    if (file == NULL)
    {
        printf("File cannot be opened");
        return;
    }
    // This will print the P5 metadeta at the top of the new file followed by a newline character
    fprintf(file, "P5\n");
    // This will print width and height metadeta below the P5 string. This is also followed by a newline character.
    fprintf(file, "%d %d\n", *width, *height);
    // This prints the maximum pixel value (255) followed by a newline character. This is the end of he header section.
    fprintf(file, "255\n");

    for (int row = 0; row < *height; ++row)
    {
        for (int col = 0; col < *width; ++col)
        {
            // For each row and each column, create a new variable called buffer which stores each unsigned char value from the current element (row and column) in the matrix.
            unsigned char buffer = matrix[row][col].value;
            // This will write the pixel data in binary from the buffer into the file 8 bits at a time.
            fwrite(&buffer, sizeof(unsigned char), 1, file);
        }
    }
    fclose(file);
}
PixelGray **threshold(PixelGray **matrix, int *width, int *height)
{
    // This initializes a new 2D PixelGray array using **, or double pointer. This intiial row will have length equal to *height and each cell is the size of an unsigned char (PixelGray)
    PixelGray **array = (PixelGray **)malloc(*width * sizeof(PixelGray *));

    // This for loop will create another array using a single pointer * for each element of the array above. The size of each of these single pointer arrays is *width. The size of each cell is PixelGray (8 bits).
    // Using this for loop allows us to create the 2D dynamically allocated array
    for (int i = 0; i < *width; ++i)
    {
        array[i] = (PixelGray *)malloc(*height * sizeof(PixelGray));
    }
    // Iterates through each row of the 2D matrix
    for (int row = 0; row < *height; ++row)
    {
        // For the current row, iterate through all of the columns of the 2D matrix
        for (int col = 0; col < *width; ++col)
        {
            // This condition checks if the pixel value of the current matrix element is greater than 80
            if (matrix[row][col].value > 80)
            {
                // Creates a new PixelGray variable with the value = 255. This means that the current matrix element from the passed in 2D array called 'matrix' passes the threshold
                PixelGray num = {.value = 255};
                // This assigns the new PixelGray variable created above to the current array position of the passed in 2D matrix. This ensures consistency across both 2d matrices.
                array[row][col] = num;
            }
            else
            {
                // This execuetes when the current matrix element does not meat the treshold, so it creates a new PixelGray variable with value = 0.
                PixelGray num = {.value = 0};
                // This assigns the new PixelGray variable into the same position of the newly created 2D matrix called "array" which represents the threshold converted values of "matrix"
                array[row][col] = num;
            }
        }
    }
    // Return the pointer of the newly created treshold matrix called 'array'
    return array;
}
PixelGray **rotate(PixelGray **matrix, int *width, int *height)
{
    // This initializes a new 2D PixelGray rotated_array using **, or double pointer. This intiial row will have length equal to *height and each cell is the size of an unsigned char (PixelGray)
    PixelGray **rotated_matrix = (PixelGray **)malloc(sizeof(PixelGray *) * *width);

    // This for loop will create another array using a single pointer * for each element of the array above. The size of each of these single pointer arrays is *width. The size of each cell is PixelGray (8 bits).
    // Using this for loop allows us to create the 2D dynamically allocated array
    for (int i = 0; i < *width; ++i)
    {
        rotated_matrix[i] = (PixelGray *)malloc(*height * sizeof(PixelGray));
    }

    // Iterates through each row of the 2D matrix
    for (int col = 0; col < *width; ++col)
    {
        // For the current row, iterate through all of the columns of the 2D matrix
        for (int row = 0; row < *height; ++row)
        {
            // This creates a new PixelGray variable with the value matrix[row][col].value.
            PixelGray val = {.value = matrix[row][col].value};
            // This performs row and column swapping (matrix transpose) by taking the current element of the oriignal matrix and swapping the indices. So the current row becomes the current col and current col becomes current row.
            rotated_matrix[col][row] = val;
        }
    }
    // Returns the newly created rotated_matrix pointer
    return rotated_matrix;
}