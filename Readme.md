# Debugslider

An stb-style single header debug slider library

## Usage 

Example

```C
#include <stdio.h>

#define ENABLE_VISUAL_STYLES
#define SLIDER_IMPLEMENTATION
#include "slider.h"

int main(void) {
    Slider slider = slider_new(u8"amount", 1, 400);
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
```
This will print the slider value on every iteration of the loop.

## API

```C
// create a new slider with the title <name>, and the range from min to max

Slider slider_new(const char* name, int min, int max);

// retrieve the slider value
int slider_val(Slider* slider);
```

## Visual Styles

You can enable a more modern appearance by: `
```C
    #define ENABLE_VISUAL_STYLES
```
if you are using Clang or MSVC. If you use GCC you have to haggle with your linker.

Appearance without visual styles

![debugslider](./doc/no_visual_styles.png)

and with visual styles

![debugslider](./doc/visual_styles.png)

## License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
