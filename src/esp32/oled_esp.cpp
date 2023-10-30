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
	ssd1306_init(&dev_, 128, 64);
	ssd1306_clear_screen(&dev_, false);
	ssd1306_contrast(&dev_, 0xff);
    return true;
}
//---
bool OLED::write(int vpos, 
    const string& s, int font_sz)
{
    if(font_sz <3)
        ssd1306_display_text(&dev_, vpos, s.c_str(), s.length(), false);
    else
    	ssd1306_display_text_x3(&dev_, vpos, s.c_str(), s.length(), false);

    return true;
}
//---
void OLED::clear()
{
    ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
}
