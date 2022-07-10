
#ifndef __low_lcd_h__
#define __low_lcd_h__

#include "plat_os.h"

/*
 * 屏幕旋转方向枚举,由硬件决定是否需要
 * 目前只有lcd才需要，墨水屏不需要
 */
enum
{
    LOW_LCD_ROTATE_0 = 0,
    LOW_LCD_ROTATE_90,
    LOW_LCD_ROTATE_180,
    LOW_LCD_ROTATE_270,
}low_lcd_rotate;

/*
 * 初始化显示硬件
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_lcd_init(void);

/*
 * 显示模块休眠前如果需要做特殊操作的话在这里完成
 */
void low_lcd_term(void);

/*
 * 获取lcd水平方面的像素点数
 * retval:
 *  
 */
uint32_t low_lcd_get_xsize(void);

/*
 * 获取lcd垂直方向的像素点数
 * retval:
 *  
 */
uint32_t low_lcd_get_ysize(void);

/*
 * 刷新一屏数据
 * buf:
 *  把这里的数据写到整个屏幕上
 *  应用层会保证buf里的数据正好是一屏的数据
 */
void low_lcd_flush(uint8_t *buf);


/*
 * 设置指定位置到颜色,目前不用实现
 * pixel: 4字节分别为:blue,green,red,reserved;
 */
void low_lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t pixel);

/*
 * 关闭显示模块的电源，进入低功耗状态,墨水屏暂时不用实现
 */
void low_lcd_display_off(void);

/*
 * 显示模块恢复工作,与display_off()配对使用,墨水屏暂时不用实现
 */
void low_lcd_display_on(void);

/*
 * 设置lcd的亮度，没有背光的显示模块不用实现
 * level:0-100
 */
void low_lcd_set_brightness(uint8_t level);

/*
 * 配置屏幕显示方向旋转,lcd屏需要，墨水屏暂时不需要
 * rotate[in]:
 *  low_lcd_rotate枚举类型
 * retval:
 *  0: success
 *  -1: fail
 */
int32_t low_lcd_set_rotate(int32_t rotate);

/*
*获取屏幕型号,暂时不用实现
*/
int32_t low_lcd_id_get(void);

#endif

