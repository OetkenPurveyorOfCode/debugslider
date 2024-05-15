#pragma once
// (c) Alexander Oetken 2024
/*
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
*/
#include <windows.h>
#include <commctrl.h>

typedef enum {
    SLIDER_OK,
    SLIDER_CLOSED,
    SLIDER_INVALID_TITLE,
    SLIDER_INVALID_INSTANCE,
    SLIDER_REGISTER_CLASS_FAILED,
    SLIDER_WINDOW_CREATION_FAILED,
    SLIDER_COMMON_CONTROLS_INIT_FAILED,
} Slider_Error;

typedef struct {
    HWND hwnd;
    HWND slider_handle;
    HWND value_label;
    int min;
    int err;
} Slider;

// create a new slider with the title name, and the range from min to max
Slider slider_new(const char* name, int min, int max);

// retrieve the slider value 
int slider_val(Slider* slider);

//////////////////////////////////////////////////////////
#ifdef SLIDER_IMPLEMENTATION
#ifdef ENABLE_VISUAL_STYLES
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#if defined(_MSC_VER) || defined(__clang__)
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "user32.lib")
#endif

void update_label(Slider* slider);
static LRESULT CALLBACK slider_windproc(HWND hwnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam) {
  Slider* slider = (Slider*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  switch (msg) {
    break;case WM_CREATE: {
    }
    break;case WM_CLOSE: {
        DestroyWindow(hwnd);
    }
    break;case WM_HSCROLL: {
        update_label(slider);
    }
    break;case WM_DESTROY: {
        PostQuitMessage(0);
    }
    break;default: {
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
  }
  return 0;
}

Slider create_slider_control(HWND hwnd, int min, int max);

Slider slider_new(const char* name, int min, int max) {
    wchar_t wname[255] = {0};
    int num_chars =  MultiByteToWideChar(CP_UTF8, 0, name, (int)strlen(name), wname, 255);
    if (num_chars == 0) {
        return (Slider){.min = min, .err=SLIDER_INVALID_TITLE};
    }
    wname[254] = L'\0';
    HINSTANCE instance = GetModuleHandleW(0);
    if (instance == NULL) {
        return (Slider){.min = min, .err=SLIDER_INVALID_INSTANCE};
    }
    WNDCLASSEXW wc = {
        .cbSize = sizeof(WNDCLASSEXW),
        .style = CS_VREDRAW | CS_HREDRAW,
        .lpfnWndProc = slider_windproc,
        .hInstance = instance,
        .lpszClassName = wname,
        .hbrBackground = GetSysColorBrush(COLOR_3DFACE),
    };
    if(!RegisterClassExW(&wc)) {
        return (Slider){.min= min, .err=SLIDER_REGISTER_CLASS_FAILED};
    }
    HWND hwnd = CreateWindowExW(
        0,
        wname, 
        wname, 
        WS_OVERLAPPEDWINDOW,
        100, 100, 340, 20+60+20,
        0,
        0, 
        instance, 
        0
    );
    if (hwnd == 0) {
        return (Slider){.min= min, .err=SLIDER_WINDOW_CREATION_FAILED};
    }
    // Slider time !
    INITCOMMONCONTROLSEX common_control_sex = {
        .dwSize = sizeof(INITCOMMONCONTROLSEX),
        .dwICC = ICC_LISTVIEW_CLASSES,
    };
    if (InitCommonControlsEx(&common_control_sex) == FALSE) {
        return (Slider){.min=min, .err=SLIDER_COMMON_CONTROLS_INIT_FAILED};
    }
    Slider slider = create_slider_control(hwnd, (int)min, (int)max);
    SetFocus(slider.slider_handle);
    SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)&slider);
    ShowWindow(hwnd, SW_NORMAL);
    UpdateWindow(hwnd);
    return slider;
}

Slider create_slider_control(HWND window, int min, int max) {
    wchar_t left_label_text[33];
    _snwprintf(left_label_text, 33, L"%d", min);
    HWND left_label = CreateWindowW(L"Static", left_label_text, 
        WS_CHILD | WS_VISIBLE, 0, 20, 30, 30, window, 0, NULL, NULL
    );
    if (left_label == NULL) {
        return (Slider){.min=min, .err=SLIDER_WINDOW_CREATION_FAILED};
    }
    wchar_t right_label_text[33];
    _snwprintf(right_label_text, 33, L"%d", max);
    HWND right_label = CreateWindowW(L"Static", right_label_text, 
        WS_CHILD | WS_VISIBLE, 240, 20, 30, 30, window, (HMENU)2, NULL, NULL);
    if (right_label == NULL) {
        return (Slider){.min=min, .err=SLIDER_WINDOW_CREATION_FAILED};
    }
    HWND label_handle = CreateWindowW(
        L"Static", L"0", WS_CHILD | WS_VISIBLE | SS_CENTER, 280, 20, 30, 30, window, (HMENU)3, NULL, NULL
    );
    if (label_handle == NULL) {
        return (Slider){.min=min, .err=SLIDER_WINDOW_CREATION_FAILED};
    }
    HWND slider_handle = CreateWindowW(
        TRACKBAR_CLASSW,
        L"Slider value",
        WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS,
        30,
        20,
        200,
        30,
        window,
        (HMENU)3,
        0,
        0
    );
    if (slider_handle == 0) {
        return (Slider){.min=min, .err=SLIDER_WINDOW_CREATION_FAILED};
    }
    SendMessageW(
        slider_handle,
        TBM_SETRANGE, 
        (WPARAM) TRUE,                   // redraw flag 
        (LPARAM) MAKELONG((int)min, (int)max)
    );
    //SendMessageW(slider_handle, TBM_SETBUDDY, TRUE, (LPARAM) left_label); 
    //SendMessageW(slider_handle, TBM_SETBUDDY, FALSE, (LPARAM) right_label); 
    //SendMessageW(slider_handle, TBM_SETPAGESIZE, 0, 1);
    SendMessageW(slider_handle, TBM_SETTICFREQ, (WPARAM)(max-min)/10, 0); 
    return (Slider){
        .hwnd = window,
        .min = min,
        .slider_handle = slider_handle,
        .value_label = label_handle,
    };
}

int slider_val(Slider* slider) {
    MSG msg;
    int result = (int)SendMessageW(slider->slider_handle, TBM_GETPOS, 0, 0);

    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            result = slider->min;
            slider->err = SLIDER_CLOSED;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return result;
}

void update_label(Slider* slider) {
    int pos = (int)SendMessageW(slider->slider_handle, TBM_GETPOS, 0, 0);
    wchar_t value_label_text[33];
    _snwprintf(value_label_text, 33, L"%d", pos);
    SetWindowTextW(slider->value_label, value_label_text);
}
#endif


