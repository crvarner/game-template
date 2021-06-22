#include <stdio.h>

#include "gd.h"

int main() {
    double a = get_double(0, 100);
    printf("%f\n", a);
    return 0;
}