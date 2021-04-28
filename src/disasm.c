#ifdef STANDALONE_DISASSEMBLER

#include "common.h"
#include "chunk.h"
#include "qst.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
    for (size_t i = 1; i < (size_t) argc; i++) {
        FILE* fd = fopen(argv[i], "rb");
        if (fd == NULL) {
            fprintf(stderr, "error: failed to open input file \"%s\"\n", argv[i]);
            return 1;
        }

        QstBin bin;
        int ret = read_bin(fd, &bin);
        if (ret < 0) {
            fprintf(stderr, "error: failed to read input file \"%s\"\n", argv[i]);
        }

        fclose(fd);

        Chunk chunk;
        init_chunk(&chunk);
        bin_to_chunk(&bin, &chunk);

        disassemble_chunk(&chunk, argv[i], false);

        free_chunk(&chunk);
        free_bin(&bin);
    }
}

#endif
