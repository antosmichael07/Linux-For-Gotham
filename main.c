#include <Windows.h>

DWORD history[7] = {0, 0, 0, 0, 0, 0, 0};

char typed = 0;

void simulate_key_press(DWORD key, char ctrl) {
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;

    if (ctrl == 1) {
        input.ki.wVk = VK_CONTROL;
        input.ki.dwFlags = 0;
        SendInput(1, &input, sizeof(INPUT));
    }

    input.ki.wVk = key;
    input.ki.dwFlags = 0;
    SendInput(1, &input, sizeof(INPUT));

    Sleep(1);

    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));

    if (ctrl == 1) {
        input.ki.wVk = VK_CONTROL;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
}

LRESULT CALLBACK low_level_keyboard_proc(int n_code, WPARAM w_param, LPARAM l_param) {
    if (typed == 1) {
        typed = 0;
        simulate_key_press(VK_BACK, 0);
    }
    if (n_code == HC_ACTION) {
        KBDLLHOOKSTRUCT *keyboard = (KBDLLHOOKSTRUCT *)l_param;
        switch (w_param) {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                for (int i = 0; i < 6; i++) {
                    history[i] = history[i + 1];
                }
                history[6] = keyboard->vkCode;

                if (history[0] == 'W' && history[1] == 'I' && history[2] == 'N' && history[3] == 'D' && history[4] == 'O' && history[5] == 'W' && history[6] == 'S') {
                    simulate_key_press('A', 1);
                    simulate_key_press(VK_BACK, 0);
                    simulate_key_press('L', 0);
                    simulate_key_press('I', 0);
                    simulate_key_press('N', 0);
                    simulate_key_press('U', 0);
                    simulate_key_press('X', 0);
                    simulate_key_press(VK_SPACE, 0);
                    simulate_key_press('F', 0);
                    simulate_key_press('O', 0);
                    simulate_key_press('R', 0);
                    simulate_key_press(VK_SPACE, 0);
                    simulate_key_press('G', 0);
                    simulate_key_press('O', 0);
                    simulate_key_press('T', 0);
                    simulate_key_press('H', 0);
                    simulate_key_press('A', 0);
                    simulate_key_press('M', 0);
                    typed = 1;
                }
        }
    }

    return CallNextHookEx(NULL, n_code, w_param, l_param);
}

int main() {
    HHOOK keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, low_level_keyboard_proc, NULL, 0);
    if (keyboard_hook == NULL) {
        return 1;
    }
    
    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    
    UnhookWindowsHookEx(keyboard_hook);
    return 0;
}
