#include <cstdio>
#include <cstdlib>

extern "C" {
#include "fileutil.h"
}

#include "vasm/vassembler.hpp"
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

    char *data = static_cast<char *>(malloc(inputLength));
    fread(data, 1, inputLength, input);

    fclose(input);

    auto byteCode = vasm::compile(data);
    FILE *output = fopen(outputName, "w");
    vasm::ByteCodeMetadata metadata = {byteCode.size()};

    fwrite(&metadata, sizeof(vasm::ByteCodeMetadata), 1, output);
    fwrite(byteCode.data(), 1, byteCode.size(), output);

    fclose(output);
    free(data);
}