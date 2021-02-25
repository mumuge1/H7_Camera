/**
 * @file lv_demo_benchmark.h
 *
 */

#ifdef __cplusplus
extern "C" {
#endif
#include "lvgl.h"
void lv_demo_benchmark(void);


struct {
	lv_obj_t* current;//指向
	lv_obj_t* previous;
	lv_obj_t* next;
}APP_t;


#ifdef __cplusplus
} /* extern "C" */
#endif
