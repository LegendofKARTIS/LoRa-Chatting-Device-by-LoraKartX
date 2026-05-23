#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_group_icon;
extern const lv_img_dsc_t img_packet_icon;
extern const lv_img_dsc_t img_settings_icon;
extern const lv_img_dsc_t img_settings_icon_1;
extern const lv_img_dsc_t img_signal_icon;
extern const lv_img_dsc_t img_target;
extern const lv_img_dsc_t img_tower;
extern const lv_img_dsc_t img_packets;
extern const lv_img_dsc_t img_clock;
extern const lv_img_dsc_t img_time;
extern const lv_img_dsc_t img_router;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[11];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/