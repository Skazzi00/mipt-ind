#include "vasm/processor.hpp"
#include "vasm/bytecode.hpp"

extern "C" {
#include "fileutil.h"
}

int main(int argc, char **argv) {
    using namespace mipt::vasm;
    if (argc != 2) {
        return 1;
    }

    const char *inputName  = argv[1];
    FILE       *input      = fopen(inputName, "r");
    size_t     inputLength = static_cast<size_t>(getFileSize(input));

    auto *data = static_cast<unsigned char *>(malloc(inputLength));
    fread(data, 1, inputLength, input);

    fclose(input);
    auto metadata = reinterpret_cast<ByteCodeMetadata *>(data);
    if (metadata->version != VERSION) {
        fprintf(stderr, "Wrong version! \n"
                        "Current: %lg\n"
                        "File: %lg", VERSION, metadata->version);
        return 1;
    }
    unsigned char * byteCode = data + sizeof(ByteCodeMetadata);
    Processor proc(byteCode, 1280000);
    proc.start();
    free(data);
}