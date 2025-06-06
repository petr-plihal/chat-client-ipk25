#ifdef DEBUG_PRINT

#include <string>

// Debug printing macro from assignment
#define printf_debug(format, ...) \
    do { \
        if (*#format) { \
            fprintf(stderr, "%s:%-4d | %15s | " format "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        } \
        else { \
            fprintf(stderr, "%s:%-4d | %15s | \n", __FILE__, __LINE__, __func__); \
        } \
    } while (0)


// Function to escape non-printable characters
std::string EscapeNonPrintable(const std::string& input);

#else
#define printf_debug(format, ...) (0)
#endif