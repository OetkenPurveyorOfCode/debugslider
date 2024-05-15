#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define SLIDER_IMPLEMENTATION
#include "slider.h"
#include <math.h>

int main(void) {
    Slider slider = slider_new(u8"amount", /*min*/ 1, /*max*/ 400);
    if (slider.err != 0) {
        printf("I am so sad! Err: %d\n", slider.err);
    }
    for(;;) {
        int val = slider_val(&slider);
        printf("amount: %d\n", val);
        if (slider.err == SLIDER_CLOSED) {
            break;
        }   
    }
    return 0;
}
