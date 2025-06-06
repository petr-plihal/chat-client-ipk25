#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include "Common/serverOptions.h"

#include <string>

class ArgumentParser {
public:

    ArgumentParser(int argc, char* argv[])
        : argc(argc), argv(argv) {};

    void Parse(serverOptions& options);

private:

    int argc;
    char** argv;

    std::string ResolveHostname(const std::string& hostname);
    void PrintHelp() const;

};

#endif // ARGUMENTPARSER_H