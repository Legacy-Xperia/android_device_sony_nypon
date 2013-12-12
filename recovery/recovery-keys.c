/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>

#include "recovery_ui.h"
#include "common.h"
#include "extendedcommands.h"
#include "adefines.h"

int device_toggle_display(volatile char* key_pressed, int key_code) {
    // hold power and press volume-up
    return key_pressed[KEY_POWER] && key_code == KEY_VOLUMEUP;
}

int vibrate(int timeout_ms) {
    char str[20];
    int fd;
    int ret;

    fd = open(VIBRATOR_TIMEOUT_FILE, O_WRONLY);
    if (fd < 0)
        return -1;

    ret = snprintf(str, sizeof(str), "%d", timeout_ms);
    ret = write(fd, str, ret);
    close(fd);

    if (ret < 0)
       return -1;

    return 0;
}

int device_handle_key(int key_code, int visible) {

    if (visible) {

        switch (key_code) {
            case KEY_CAPSLOCK:
            case KEY_DOWN:
#ifndef XPERIA_CWM_TOUCH
            case KEY_MENU:
#endif
                vibrate(VIBRATOR_TIME_MS);
                return HIGHLIGHT_DOWN;

            case KEY_VOLUMEDOWN:
                return HIGHLIGHT_DOWN;

            case KEY_LEFTSHIFT:
            case KEY_UP:
                vibrate(VIBRATOR_TIME_MS);
                return HIGHLIGHT_UP;

            case KEY_VOLUMEUP:
                return HIGHLIGHT_UP;

            case KEY_POWER:
#ifndef XPERIA_CWM_TOUCH
                if (ui_get_showing_back_button()) {
#endif
                    return SELECT_ITEM;
#ifndef XPERIA_CWM_TOUCH
                }
                if (!ui_root_menu) {
                    return GO_BACK;
                }
#endif
                break;

#ifndef XPERIA_CWM_TOUCH
            case KEY_HOME:
#endif
            case KEY_LEFTBRACE:
            case KEY_ENTER:
            case BTN_MOUSE:
            case KEY_CAMERA:
            case KEY_F21:
            case KEY_SEND:
                vibrate(VIBRATOR_TIME_MS);
                return SELECT_ITEM;

            case KEY_END:
            case KEY_BACKSPACE:
            case KEY_SEARCH:
                if (ui_get_showing_back_button()) {
                    vibrate(VIBRATOR_TIME_MS);
                    return SELECT_ITEM;
                }
                if (!ui_root_menu) {
                    vibrate(VIBRATOR_TIME_MS);
                    return GO_BACK;
                }
#ifndef XPERIA_CWM_TOUCH
            case KEY_BACK:
                if (!ui_root_menu) {
                    vibrate(VIBRATOR_TIME_MS);
                    return GO_BACK;
                }
#endif
        }
    }

    return NO_ACTION;
}
