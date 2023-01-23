CC=gcc
CFLAGS=-fopenmp -fopt-info-vec -O3 -march=native -g -std=c17 -Wno-unused-result

LD=gcc
LDFLAGS=-fopenmp
LDLIBS=

default:
	@[ "${IMPL}" ] || { echo "IMPL is not set"; exit 1; }
	@[ -f "${IMPL}.c" ] || { echo "${IMPL}.c does not exist"; exit 1; }

	$(CC) $(CFLAGS) -c -o main.o main.c
	$(CC) $(CFLAGS) -c -o ${IMPL}.o ${IMPL}.c

	$(LD) $(LDFLAGS) -o main main.o ${IMPL}.o $(LDLIBS)

clean:
	rm -rf main *.o
