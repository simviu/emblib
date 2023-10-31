#include "emb/esp32/embLib_esp.h"
#include "ssd1306.h"
#include "font8x8_basic.h"

using namespace emb;

namespace{
    SSD1306_t dev_;

}
//----
bool OLED::init()
{
    // TODO: sys level singleton
    i2c_master_init(&dev_, 
        cfg_.pin_SDA, 
        cfg_.pin_SCL, cfg_.pin_RST);
    //--- 
	ssd1306_init(&dev_, 128, 64);
	ssd1306_clear_screen(&dev_, false);
	ssd1306_contrast(&dev_, 0xff);
    return true;
}
//---
bool OLED::write(int ln, 
    const string& s, int font_sz)
{
    /// for only 128x64, each page is a line, with 8bits x 128
    if(font_sz <3)
        ssd1306_display_text(&dev_, ln, (char*)s.c_str(), s.length(), false);
    else
    	ssd1306_display_text_x3(&dev_, ln, (char*)s.c_str(), s.length(), false);

    return true;
}
//---
bool OLED::clear(int ln)
{
    ssd1306_clear_line(&dev_, ln, false);
    return true;
}
//---
void OLED::clear()
{
    ssd1306_clear_screen(&dev_, false);
	ssd1306_contrast(&dev_, 0xff);
}
