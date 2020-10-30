#include <cstdio>
#include <cstdlib>

extern "C" {
#include "fileutil.h"
}

#include "vasm/vdisassembler.hpp"
#include "vasm/bytecode.hpp"

int main(int argc, char **argv) {
    using namespace mipt;
    if (argc != 3) {
        return 1;
    }
    const char *inputName = argv[1];
    const char *outputName = argv[2];

    FILE *input = fopen(inputName, "r");
    auto inputLength = static_cast<size_t>(getFileSize(input));

    auto data = static_cast<unsigned char *>(malloc(inputLength));
    fread(data, 1, inputLength, input);

    fclose(input);
    auto metadata = reinterpret_cast<vasm::ByteCodeMetadata *>(data);
    if (metadata->version != vasm::VERSION) {
        fprintf(stderr, "Wrong version! \n"
                        "Current: %lg\n"
                        "File: %lg", vasm::VERSION, metadata->version);
        return 1;
    }
    FILE *output = fopen(outputName, "w");
    vasm::decompile(*metadata, data + sizeof(vasm::ByteCodeMetadata),output);

    fclose(output);
    free(data);
}