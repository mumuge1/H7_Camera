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
LV_IMG_DECLARE(image_scen);
LV_IMG_DECLARE(CSGO);
LV_IMG_DECLARE(color);
static void lv_demo_printer_anim_in(lv_obj_t * obj, uint32_t delay);
static void btn_cb(lv_obj_t * obj,lv_event_t event);
lv_task_t* task1 = NULL;
lv_obj_t * img;
lv_img_dsc_t img_list[] = {0};
extern USBD_HandleTypeDef hUsbDeviceFS;
void task1_cb(lv_task_t* task)
{
	HAL_GPIO_TogglePin(E3_GPIO_Port,E3_Pin);
//	if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED)
//		printf("USB Unconnect...%d\r\n",hUsbDeviceFS.dev_state);
//	else
//		printf("USB connect!!!\r\n");
}



//extern uint8_t glyph_bitmap_1[];
//extern uint16_t unicode_list_1_1[];
//extern lv_font_fmt_txt_glyph_dsc_t glyph_dsc_1[];
//static const lv_font_fmt_txt_cmap_t cmaps_1[] = {
//    {
//        .range_start = 0x000a,
//        .range_length = 0x9fa5,
//        .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY,
//        .glyph_id_start = 0,
//        .unicode_list = unicode_list_1_1,
//        .glyph_id_ofs_list = NULL,
//        .list_length = 20999,
//    }
//};
//static lv_font_fmt_txt_dsc_t font_dsc = {
//    .glyph_bitmap = glyph_bitmap_1,
//    .glyph_dsc = glyph_dsc_1,
//    .cmaps = cmaps_1,
//    .cmap_num = 1,
//    .bpp = 4,

//    .kern_scale = 0,
//    .kern_dsc = NULL,
//    .kern_classes = 0,

//    .last_letter = 0x9fa5,
//    .last_glyph_id = 20998,
//};
//static int binsearch(const uint16_t *sortedSeq, int seqLength, uint16_t keyData) {
//    int low = 0, mid, high = seqLength - 1;
//    while (low <= high) {
//        mid = (low + high)>>1;//右移1位等于是/2，奇数，无论奇偶，有个值就行
//        if (keyData < sortedSeq[mid]) {
//            high = mid - 1;//是mid-1，因为mid已经比较过了
//        }
//        else if (keyData > sortedSeq[mid]) {
//            low = mid + 1;
//        }
//        else {
//            return mid;
//        }
//    }
//    return -1;
//}
///* Get the bitmap of `unicode_letter` from `font`. */
//const uint8_t * my_get_glyph_bitmap_cb(const lv_font_t * font, uint32_t unicode_letter)
//{
//    lv_font_fmt_txt_dsc_t * fdsc = (lv_font_fmt_txt_dsc_t *) font->dsc;

//    if( unicode_letter<fdsc->cmaps[0].range_start || unicode_letter>fdsc->cmaps[0].range_length ) return false;

//    int i;
//    if( unicode_letter==fdsc->last_letter ){
//        i = fdsc->last_glyph_id;
//    }
//    else{
//        i = binsearch(fdsc->cmaps[0].unicode_list, fdsc->cmaps[0].list_length, unicode_letter);
//    }
//    if( i != -1 ) {
//        const lv_font_fmt_txt_glyph_dsc_t * gdsc = &fdsc->glyph_dsc[i];
//        fdsc->last_glyph_id = i;
//        fdsc->last_letter = unicode_letter;
//        return &fdsc->glyph_bitmap[gdsc->bitmap_index];
//    }
//    return NULL;
//}
//bool my_get_glyph_dsc_cb(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next)
//{
//    lv_font_fmt_txt_dsc_t * fdsc = (lv_font_fmt_txt_dsc_t *) font->dsc;

//    if( unicode_letter<fdsc->cmaps[0].range_start || unicode_letter>fdsc->cmaps[0].range_length ) return false;

//    int i;
//    if( unicode_letter==fdsc->last_letter ){
//        i = fdsc->last_glyph_id;
//    }
//    else{
//        i = binsearch(fdsc->cmaps[0].unicode_list, fdsc->cmaps[0].list_length, unicode_letter);
//    }
//    if( i != -1 ) {
//        const lv_font_fmt_txt_glyph_dsc_t * gdsc = &fdsc->glyph_dsc[i];
//        fdsc->last_glyph_id = i;
//        fdsc->last_letter = unicode_letter;
//        dsc_out->adv_w = gdsc->adv_w;
//        dsc_out->box_h = gdsc->box_h;
//        dsc_out->box_w = gdsc->box_w;
//        dsc_out->ofs_x = gdsc->ofs_x;
//        dsc_out->ofs_y = gdsc->ofs_y;
//        dsc_out->bpp   = fdsc->bpp;
//        return true;
//    }
//    return false;
//}
extern lv_font_t myFont1;
void lv_demo_benchmark(void)
{
//	lv_font_t my_font;
//	my_font.get_glyph_dsc = my_get_glyph_dsc_cb;        /*Set a callback to get info about gylphs*/
//	my_font.get_glyph_bitmap = my_get_glyph_bitmap_cb;  /*Set a callback to get bitmap of a glyp*/
//	my_font.line_height = 16;                       /*The real line height where any text fits*/
//	my_font.base_line = 0;                      /*Base line measured from the top of line_height*/
//	my_font.dsc = &font_dsc;                   /*Store any implementation specific data here*/	

//    lv_theme_t * theme = lv_theme_material_init(LV_COLOR_MAKE(0xff,0,0),LV_COLOR_MAKE(0,0,0xff),0,
//		&lv_font_montserrat_14,&lv_font_montserrat_14,&lv_font_montserrat_14,&lv_font_montserrat_14);//创建主题
    lv_theme_t * theme = lv_theme_material_init(LV_COLOR_MAKE(0xff,0,0),LV_COLOR_MAKE(0,0,0xff),0,
		&myFont1,&myFont1,&myFont1,&myFont1);//创建主题
    lv_theme_set_act(theme);

    lv_obj_t *scr = lv_scr_act();//获取当前活跃的屏幕对象

	img = lv_img_create(scr,NULL);
	lv_img_set_src(img,"1:color.bin");
	lv_demo_printer_anim_in(img,400);
	
    lv_obj_t * btn = lv_btn_create(scr,NULL);
    lv_obj_set_size(btn, 50,20);

    lv_obj_align(btn,scr,LV_ALIGN_IN_BOTTOM_MID,0,0);
    lv_obj_t *label_btn = lv_label_create(btn, NULL);
    lv_label_set_text(label_btn,"!!" LV_SYMBOL_HOME );
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
	//static int i = 0;
    if(event == LV_EVENT_CLICKED)
    {
		lv_img_set_src(img,((lv_img_get_src(img) == color.data)?(&image_scen):(&color)));
		lv_demo_printer_anim_in(img,400);
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
