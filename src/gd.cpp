#include <stdio.h>

double get_double(double min, double max) {
    double input;
    do {
        printf("enter double between %f and %f\n", min, max);
        scanf("%lf", &input);
        if (input < min) printf("Must be at least %f\n", min);
        if (input > max) printf("Must be atmost %f\n", max);
    }
    while (input < min || input > max);
    return input;
}