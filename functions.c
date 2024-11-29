#define _XOPEN_SOURCE 1
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "functions.h"
#include "hidden_functions/hidden_functions.h"

int parse_options(int argc, char *const *argv, char **in, char **out) {
    optind = 1; // Reset getopt index
    for (int32_t i = 0; i < argc; i++) {
        const int option = getopt(argc, argv, "i:o:");
        switch (option) {
            case -1:
                /* No more options */
                i = INT32_MAX - 1;
                break;
            case 'i':
                /* Input file */
                *in = strdup(optarg);
                if (*in == NULL) {
                    perror("Failed to allocate memory");
                    return EXIT_FAILURE;
                }
                break;
            case 'o':
                /* Output file */
                *out = strdup(optarg);
                if (*out == NULL) {
                    perror("Failed to allocate memory");
                    return EXIT_FAILURE;
                }
                break;
            case '?':
                /* Ambiguous or unknown */
                fprintf(stderr, "Unknown or ambiguous value.\n");
                return EXIT_FAILURE;
            default:
                fprintf(stderr, "An unexpected error occurred.\n");
                return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int secure_copy_file(const char *in, const char *out) {
    int input_fd = open(in, O_RDONLY | O_NOFOLLOW);
    if (input_fd < 0) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    struct stat input_stat;
    if (fstat(input_fd, &input_stat) == -1 || !S_ISREG(input_stat.st_mode)) {
        perror("Input file is not a regular file");
        close(input_fd);
        return EXIT_FAILURE;
    }

    int output_fd = open(out, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (output_fd < 0) {
        perror("Failed to open output file");
        close(input_fd);
        return EXIT_FAILURE;
    }

    // Perform file copy securely
    char buffer[4096];
    ssize_t bytes_read, bytes_written;
    while ((bytes_read = read(input_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(output_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Failed to write data");
            close(input_fd);
            close(output_fd);
            return EXIT_FAILURE;
        }
    }
    if (bytes_read < 0) {
        perror("Failed to read data");
        close(input_fd);
        close(output_fd);
        return EXIT_FAILURE;
    }

    close(input_fd);
    close(output_fd);
    return EXIT_SUCCESS;
}
