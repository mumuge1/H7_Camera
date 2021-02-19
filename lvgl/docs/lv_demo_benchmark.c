/**
 * @file lv_demo_benchmark.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
//#include "../../lv_examples.h"
#include "lv_demo_benchmark.h"
#include "../lvgl/lvgl.h"
#include "main.h"
#include "lv_conf.h"
#include "main.h"
#include "usb_device.h"
extern lv_indev_t * indev_keypad;
//LV_IMG_DECLARE(image_scen);
//LV_IMG_DECLARE(CSGO);
//LV_IMG_DECLARE(color);
static void lv_demo_printer_anim_in(lv_obj_t * obj, uint32_t delay);
static void btn_cb(lv_obj_t * obj,lv_event_t event);
lv_task_t* task1 = NULL;
lv_obj_t * img;
lv_img_dsc_t img_list[] = {0};
extern USBD_HandleTypeDef hUsbDeviceFS;
void task1_cb(lv_task_t* task)
{
	
	if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED)
		HAL_GPIO_WritePin(E3_GPIO_Port,E3_Pin,GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(E3_GPIO_Port,E3_Pin,GPIO_PIN_SET);
}

void lv_demo_benchmark(void)
{
    lv_theme_t * theme = lv_theme_material_init(LV_COLOR_MAKE(0xff,0,0),LV_COLOR_MAKE(0,0,0xff),0,
		&lv_font_montserrat_14,&lv_font_montserrat_14,&lv_font_montserrat_14,&lv_font_montserrat_14);//创建主题
//    lv_theme_t * theme = lv_theme_material_init(LV_COLOR_MAKE(0xff,0,0),LV_COLOR_MAKE(0,0,0xff),0,
//		&myFont1,&myFont1,&myFont1,&myFont1);//创建主题
    lv_theme_set_act(theme);

    lv_obj_t *scr = lv_scr_act();//获取当前活跃的屏幕对象

	img = lv_img_create(scr,NULL);
	lv_img_set_src(img,"1:color.bin");
	lv_demo_printer_anim_in(img,400);
	
    lv_obj_t * btn = lv_btn_create(scr,NULL);
    lv_obj_set_size(btn, 50,20);

    lv_obj_align(btn,scr,LV_ALIGN_IN_BOTTOM_MID,0,0);
    lv_obj_t *label_btn = lv_label_create(btn, NULL);
    lv_label_set_text(label_btn,LV_SYMBOL_HOME "wuhu");
    lv_obj_set_event_cb(btn,btn_cb);
	
    lv_group_t *group = lv_group_create();
    lv_indev_set_group(indev_keypad, group);
	lv_group_add_obj(group ,btn);
	lv_group_set_editing(group,false);
	
	lv_obj_t* label = lv_label_create(lv_scr_act(),NULL);
	lv_label_set_text(label,LV_SYMBOL_BATTERY_2 LV_SYMBOL_WIFI LV_SYMBOL_BLUETOOTH LV_SYMBOL_SD_CARD LV_SYMBOL_USB LV_SYMBOL_VOLUME_MAX
							LV_SYMBOL_TRASH LV_SYMBOL_KEYBOARD LV_SYMBOL_AUDIO LV_SYMBOL_VIDEO LV_SYMBOL_CLOSE );
	lv_label_set_align(label,LV_ALIGN_IN_TOP_LEFT);
    task1 = lv_task_create(task1_cb,1000,LV_TASK_PRIO_MID,NULL);
    lv_task_ready(task1);
	
	

}





static void btn_cb(lv_obj_t * obj,lv_event_t event)
{
	static int i = 0;
    if(event == LV_EVENT_CLICKED)
    {
		i++;
		if(i % 3 == 0){
			lv_img_set_src(img,"1:image_scen.bin");
			lv_demo_printer_anim_in(img,400);
		}else if(i % 3 == 1){
			lv_img_set_src(img,"1:CSGO.bin");
			lv_demo_printer_anim_in(img,400);
		}else if(i % 3 == 2){
			lv_img_set_src(img,"1:color.bin");
			lv_demo_printer_anim_in(img,400);
		}
    }
}
static void lv_demo_printer_anim_in(lv_obj_t * obj, uint32_t delay)
{
    if (obj != lv_scr_act()) {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_time(&a, 150);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(obj) - LV_VER_RES/20,
                lv_obj_get_y(obj));
        //lv_anim_set_path(&a, lv_anim_path_bounce);
        lv_anim_start(&a);

        lv_obj_fade_in(obj, 150 - 50, delay);
    }
}
