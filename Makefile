CFLAGS := -Wall -Wextra -fstrict-aliasing -Wfloat-equal -Wundef -Wshadow \
	-Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 \
	-Wwrite-strings -Wcast-qual -Wconversion \
	-Wunreachable-code -Wformat=2 -Isrc

all: interpreter

interpreter: bin/interpreter

debug:
	$(eval CFLAGS += -DDEBUG -g)

bin/interpreter: src/*.c
	@ mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@ rm -r bin/* || true