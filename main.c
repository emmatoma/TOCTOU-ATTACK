#include "functions/functions.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *input = NULL;
    char *output = NULL;

    /* Parse user inputs and check conformance */
    int error = parse_options(argc, argv, &input, &output);
    if (error || input == NULL || output == NULL) {
        fprintf(stderr, "Error: '-o <out>' and '-i <in>' must be provided.\n");
        goto cleanup;
    }

    /* Securely copy input file to output file */
    int result = secure_copy_file(input, output);
    if (result != EXIT_SUCCESS) {
        fprintf(stderr, "Error: Failed to copy file '%s' to '%s'.\n", input, output);
    }

cleanup:
    if (input) free(input);
    if (output) free(output);
    return (error || result) ? EXIT_FAILURE : EXIT_SUCCESS;
}
