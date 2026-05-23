#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *rssi_text;
    lv_obj_t *snr_text;
    lv_obj_t *packets_text;
    lv_obj_t *last_received_text;
    lv_obj_t *obj3;
    lv_obj_t *net_id_2;
    lv_obj_t *obj4;
    lv_obj_t *net_id;
    lv_obj_t *net_id_1;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *time_display;
    lv_obj_t *rssi_status;
    lv_obj_t *snr_status;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
};

void create_screen_main();
void tick_screen_main();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/