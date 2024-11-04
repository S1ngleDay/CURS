#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Open the file for writing
    FILE* f = fopen("file.txt", "w");

    // Check if the file opened successfully
    if (f == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE; // Exit with a failure code
    }

    // Close the file
    fclose(f);

    return EXIT_SUCCESS; // Exit with a success code
}
