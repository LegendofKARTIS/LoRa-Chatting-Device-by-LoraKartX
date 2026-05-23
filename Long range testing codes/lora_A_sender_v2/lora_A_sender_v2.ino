#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>   // ONLY used as display driver for LVGL
#include <Wire.h>
#include <RTClib.h>

// ---------- Objects ----------
lv_obj_t *label_time;
lv_obj_t *label_sender;
lv_obj_t *label_target;
lv_obj_t *label_packet;
lv_obj_t *label_status;

// ---------- Display ----------
TFT_eSPI tft = TFT_eSPI();
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 20];

// ---------- LoRa ----------
HardwareSerial LoRa(1);
int my_address = 2;
int target_address = 3;
int packet_id = 0;

// ---------- RTC ----------
RTC_PCF8563 rtc;

// ---------- Timing ----------
unsigned long last_send = 0;
unsigned long last_ui = 0;



// ---------------- Flush ----------------
void my_disp_flush(lv_disp_drv_t *disp,
                   const lv_area_t *area,
                   lv_color_t *color_p){
  uint32_t w = (uint32_t)(area->x2 - area->x1 + 1);
  uint32_t h = (uint32_t)(area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)color_p, w * h, false);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}


// ---------- UI ----------
void create_ui(){
    lv_obj_t *scr = lv_scr_act();

    label_time = lv_label_create(scr);
    lv_obj_align(label_time, LV_ALIGN_TOP_MID, 0, 10);

    label_sender = lv_label_create(scr);
    lv_obj_align(label_sender, LV_ALIGN_TOP_LEFT, 10, 50);

    label_target = lv_label_create(scr);
    lv_obj_align(label_target, LV_ALIGN_TOP_LEFT, 10, 80);

    label_packet = lv_label_create(scr);
    lv_obj_align(label_packet, LV_ALIGN_TOP_LEFT, 10, 120);

    label_status = lv_label_create(scr);
    lv_obj_align(label_status, LV_ALIGN_TOP_LEFT, 10, 160);

    lv_label_set_text(label_status, "INIT...");
}

// ---------- Helpers ----------
void update_time(){
    DateTime now = rtc.now();
    char buf[16];
    sprintf(buf, "Time: %02d:%02d", now.hour(), now.minute());
    lv_label_set_text(label_time, buf);
}

void update_ids(){
    char buf[32];

    sprintf(buf, "Sender: %d", my_address);
    lv_label_set_text(label_sender, buf);

    sprintf(buf, "Target: %d", target_address);
    lv_label_set_text(label_target, buf);
}

void update_packet(){
    char buf[32];
    sprintf(buf, "Packet: %d", packet_id);
    lv_label_set_text(label_packet, buf);
}

void update_status(const char *txt){
    lv_label_set_text(label_status, txt);
}

// ---------- LoRa ----------
void loraAT(String cmd){
    LoRa.print(cmd + "\r\n");
    delay(300);
}

// ---------- Setup ----------
void setup(){
    Serial.begin(115200);

    // LoRa
    LoRa.begin(9600, SERIAL_8N1, 14, 13);
    delay(300);

    loraAT("AT");
    loraAT("AT+BAND=865000000");
    loraAT("AT+PARAMETER=11,7,1,12");
    loraAT("AT+CRFOP=14");
    loraAT("AT+ADDRESS=" + String(my_address));

    // RTC
    Wire.begin(25, 26);
    rtc.begin();

    // Display
    tft.begin();
    tft.setRotation(3);
    tft.invertDisplay(false);   // <-- add this
  tft.startWrite();
    tft.writecommand(0x36);  // MADCTL command
    tft.writedata(0xA8);     // rotation 3 + BGR bit
    tft.endWrite();
    tft.initDMA();

    // LVGL
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 20);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 240;
    lv_disp_drv_register(&disp_drv);

    create_ui();
    update_ids();
}

// ---------- Loop ----------
void loop(){

    static uint32_t last_tick = 0;

    if(millis() - last_tick >= 5){
        lv_tick_inc(5);
        last_tick = millis();
    }

    lv_timer_handler();

    // UI update
    if(millis() - last_ui > 1000){
        update_time();
        last_ui = millis();
    }

    // Send packet
    if(millis() - last_send > 1000){

        char payload[64];
        sprintf(payload, "ID:%d|T:%lu", packet_id, millis());

        update_packet();
        update_status("SENDING");

        String cmd = "AT+SEND=" + String(target_address) + "," +
                     String(strlen(payload)) + "," + payload + "\r\n";

        LoRa.print(cmd);

        Serial.println(cmd);

        packet_id++;
        last_send = millis();
    }
}