#include "lv_demo_benchmark.h"
#include "../lvgl/lvgl.h"
#include "main.h"
#include "lv_conf.h"
#include "main.h"
#include "usb_device.h"
#include "camera.h"
//按键输入设备声明
extern lv_indev_t * indev_keypad;
//静态函数声明
static void lv_anim_Start(lv_obj_t * obj, uint32_t delay,uint32_t way);
//回调函数声明
static void btn_cb(lv_obj_t * obj,lv_event_t event);
//控件对象声明
lv_task_t* task1 = NULL;
lv_obj_t * img;
lv_obj_t* img_label;
//其他声明或定义
char* app_img_path[APP_NUM] = {"1:APP/Picture.bin",
						"1:APP/Camera.bin",
						"1:APP/Settings.bin"};
char* app_name[APP_NUM] = { "Picture",
							"Camera",
							"Settings"};
extern USBD_HandleTypeDef hUsbDeviceFS;



void task1_cb(lv_task_t* task)
{
	
	if(task == task1)
	{
		if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED)
			HAL_GPIO_WritePin(E3_GPIO_Port,E3_Pin,GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(E3_GPIO_Port,E3_Pin,GPIO_PIN_SET);
	}
}

void lv_demo_benchmark(void)
{
    lv_theme_t * theme = lv_theme_material_init(LV_COLOR_MAKE(0xff,0,0),LV_COLOR_MAKE(0,0,0xff),0,
		&lv_font_montserrat_14,&lv_font_montserrat_14,&lv_font_montserrat_14,&lv_font_montserrat_14);//创建主题
    lv_theme_set_act(theme);

    lv_obj_t *scr = lv_scr_act();//获取当前活跃的屏幕对象
	lv_obj_t* img_bg = lv_img_create(scr,NULL);
	lv_img_set_src(img_bg,"1:image/img_bg1.bin");
	
	
	
	img = lv_img_create(scr,NULL);
	lv_img_set_src(img,app_img_path[0]);
	lv_anim_Start(img,50,LEFT_TO_RIGHT);
	lv_obj_align(img,scr,LV_ALIGN_IN_BOTTOM_MID,-20,-5);
	
	img_label = lv_label_create(scr,NULL);
	lv_label_set_text(img_label,app_name[0]);
	lv_obj_align(img_label,img,LV_ALIGN_OUT_RIGHT_BOTTOM,0,5);
	lv_anim_Start(img_label,50,UP_TO_BOTTON);
	
    lv_obj_t * btn = lv_btn_create(scr,NULL);
    lv_obj_set_size(btn, 20,20);
    lv_obj_align(btn,img_label,LV_ALIGN_OUT_TOP_MID,0,-10);
    lv_obj_t *label_btn = lv_label_create(btn, NULL);
    lv_label_set_text(label_btn,LV_SYMBOL_RIGHT);
    lv_obj_set_event_cb(btn,btn_cb);
	
    lv_group_t *group = lv_group_create();
    lv_indev_set_group(indev_keypad, group);
	lv_group_add_obj(group ,btn);
	lv_group_set_editing(group,false);
	
	lv_obj_t* label = lv_label_create(lv_scr_act(),NULL);
	lv_label_set_text(label,LV_SYMBOL_BATTERY_2 LV_SYMBOL_WIFI LV_SYMBOL_SD_CARD LV_SYMBOL_USB);
	lv_label_set_align(label,LV_ALIGN_IN_TOP_LEFT);
    task1 = lv_task_create(task1_cb,1000,LV_TASK_PRIO_MID,NULL);
    lv_task_ready(task1);


}




static void btn_cb(lv_obj_t * obj,lv_event_t event)
{
	static char img_path[20];
	static int i = 0;
    if(event == LV_EVENT_CLICKED)
    {
		if(i == APP_NUM-1) i=-1;
		lv_img_set_src(img,app_img_path[++i]);
		lv_label_set_text(img_label,app_name[i]);
		lv_anim_Start(img,50,LEFT_TO_RIGHT);
		lv_anim_Start(img_label,50,UP_TO_BOTTON);
    }

}
static void lv_anim_Start(lv_obj_t * obj, uint32_t delay,uint32_t way)
{
	lv_obj_fade_out(obj,500,50);
    if (obj != lv_scr_act()) {
		
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_time(&a, 150);
        lv_anim_set_delay(&a, delay);
		if(way == UP_TO_BOTTON)
		{
			lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
			lv_anim_set_values(&a, lv_obj_get_y(obj)-25,lv_obj_get_y(obj));		
		}
		else if(way == LEFT_TO_RIGHT)
		{
			lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_x);
			lv_anim_set_values(&a, 0,lv_obj_get_x(obj));
		}
        lv_anim_start(&a);

        lv_obj_fade_in(obj,300, delay);
    }
}

