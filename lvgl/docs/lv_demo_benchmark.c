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
extern lv_indev_t * indev_keypad;
LV_IMG_DECLARE(image_scen);
LV_IMG_DECLARE(CSGO);
LV_IMG_DECLARE(main_img);

static void btn_cb(lv_obj_t * obj,lv_event_t event);
lv_task_t* task1 = NULL;
lv_obj_t * img;

void task1_cb(lv_task_t* task)
{
	HAL_GPIO_TogglePin(E3_GPIO_Port,E3_Pin);
}
void lv_demo_benchmark(void)
{
    lv_theme_t * theme = lv_theme_material_init(LV_COLOR_MAKE(0xff,0,0),LV_COLOR_MAKE(0,0,0xff),NULL,&lv_font_montserrat_8,&lv_font_montserrat_14,NULL,NULL);//创建主题
    lv_theme_set_act(theme);

    lv_obj_t *scr = lv_scr_act();//获取当前活跃的屏幕对象

	img = lv_img_create(scr,NULL);
	lv_img_set_src(img,&main_img);

    lv_obj_t * btn = lv_btn_create(scr,NULL);
    lv_obj_set_size(btn, 50,20);

    lv_obj_align(btn,scr,LV_ALIGN_IN_BOTTOM_MID,0,0);
    lv_obj_t *label_btn = lv_label_create(btn, NULL);
    lv_label_set_text(label_btn,"ok");
    lv_obj_set_event_cb(btn,btn_cb);
	
    lv_group_t *group = lv_group_create();
    lv_indev_set_group(indev_keypad, group);
	lv_group_add_obj(group ,btn);
	lv_group_set_editing(group,false);
	
    task1 = lv_task_create(task1_cb,1000,LV_TASK_PRIO_MID,NULL);
    lv_task_ready(task1);
}


static void btn_cb(lv_obj_t * obj,lv_event_t event)
{
	static int i = 0;
    if(event == LV_EVENT_CLICKED)
    {
		if(i++%2 == 0)
			lv_img_set_src(img,&CSGO);
		else
			lv_img_set_src(img,&image_scen);
    }
}
