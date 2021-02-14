#include "lv_port_disp_template.h"
#include "lcd.h"
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void lv_port_disp_init(void)
{
    LCD_Init();
    static lv_disp_buf_t draw_buf_dsc_1;
    static lv_color_t draw_buf_1[LV_HOR_RES_MAX * 160];                          /*A buffer for 10 rows*/
	static lv_color_t draw_buf_2[LV_HOR_RES_MAX * 160];
    lv_disp_buf_init(&draw_buf_dsc_1, draw_buf_1, draw_buf_2, LV_HOR_RES_MAX * 320);   /*Initialize the display buffer*/
    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/
    disp_drv.hor_res = 160;
    disp_drv.ver_res = 80;
    disp_drv.flush_cb = disp_flush;
    disp_drv.buffer = &draw_buf_dsc_1;
    lv_disp_drv_register(&disp_drv);
}
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
	static uint8_t pdata[320];
	uint16_t *rgb_data = (uint16_t*)color_p;
	uint32_t i, j;
    for(j = 0; j < (area->y2-area->y1+1); j++)
    {
        ST7735_SetCursor(&st7735_pObj, area->x1, area->y1+j);
        for(i = 0; i < (area->x2-area->x1+1); i++)
        {
          pdata[2U*i + 1U] = (uint8_t)((*rgb_data)&0x00ff);
          pdata[(2U*i)] = (uint8_t)(((*(rgb_data) & 0xff00))>>8);
          rgb_data +=1;
        }
        st7735_send_data(&(st7735_pObj.Ctx), (uint8_t*)&pdata[0], 2*(area->x2-area->x1+1));
    }
    lv_disp_flush_ready(disp_drv);
}
