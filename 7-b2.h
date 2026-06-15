/* 7-b2.h */
#pragma once

struct PopMenu {
    const char *title;
    int start_x;
    int start_y;
    int width;
    int high;
    int bg_color;
    int fg_color;
};

#define MAX_ITEM_LEN 80

int pop_menu(const char menu[][MAX_ITEM_LEN], const struct PopMenu *para);
