#include <stdio.h>
#include <assert.h>
#include <string.h>

void memcpy_(void const* src, void* dst, int size) {
	const char* src_ = (const char*)src;
	char* dst_ = (char*)dst;

	for (int i = 0; i < size; i++) {
		dst_[i] = src_[i];
	}
}

void memcpy_256(void const* src, void* dst, int size);
void memcpy_128(void const* src, void* dst, int size);
void memcpy_64(void const* src, void* dst, int size);

#define N 50000000
	int x[N], y[N];

	 int DEBUG = 0;

int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("%s", "Need argument\n");
		return 0;
	}
	for (int i = 0; i < N; i++) {
		x[i] = i;
		if (DEBUG)
			printf("%i ", x[i]);
	}
	if (DEBUG)
		printf("\n");
	if (strncmp(argv[1], "256", 4))
		memcpy_256(x, y, N * sizeof(int));
	else if (strncmp(argv[1], "128", 4))
		memcpy_128(x, y, N * sizeof(int));
	else if (strncmp(argv[1], "64", 4))
		memcpy_64(x, y, N * sizeof(int));
	else if(strncmp(argv[1], "stupid", 8))
		memcpy_(x, y, N * sizeof(int));
	if (DEBUG)
		for (int i = 0; i < N; i++)
			printf("%i ", y[i]);
	for (int i = 0; i < N; i++) {
                assert(y[i] == i);
	}
}
