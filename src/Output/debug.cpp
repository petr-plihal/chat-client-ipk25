#include "Output/debug.h"

#include <cctype>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdio>

std::string EscapeNonPrintable(const std::string& input) {

    std::string escaped_output;

    for (char char_from_input : input) {

        switch (char_from_input) {

            // Escapes non-printable characters used by the protocol
            case '\n':
                escaped_output += "\\n";
                break;
            case '\r':
                escaped_output += "\\r";
                break;

            // Every other character is added back as is
            default:
                escaped_output += char_from_input; // Přidejte ostatní znaky beze změny
                break;
        }

    }
    return escaped_output;
}