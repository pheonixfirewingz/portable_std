#include "test.h"
#include <unistd.h>
#include <string.h>

int main() {
    // The linker will place all the function pointers into a contiguous block.
    for (InitFunc* f = __start_myinit_funcs; f != __stop_myinit_funcs; ++f) {
        (*f)();
    }
    return 0;
}

void print(const char *str)
{
    write(STDOUT_FILENO, str, strlen(str));
}
