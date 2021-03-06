CFLAGS := -Wall -Wextra -fstrict-aliasing -Wfloat-equal -Wundef -Wshadow \
	-Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 \
	-Wwrite-strings -Wcast-qual -Wconversion \
	-Wunreachable-code -Wformat=2 -Isrc

all: compiler interpreter disassembler

compiler: bin/compiler
interpreter: bin/interpreter
disassembler: bin/disassembler

debug:
	$(eval CFLAGS += -DDEBUG -g)

bin/compiler: src/*.c
	@ mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^ -DSTANDALONE_COMPILER

bin/interpreter: src/*.c
	@ mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^ -DSTANDALONE_INTERPRETER

bin/disassembler: src/*.c
	@ mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^ -DSTANDALONE_DISASSEMBLER

clean:
	@ rm -r bin/* || true
