#include "LED_Display.h"

#define DATA_PIN 27

LED_Display::LED_Display(uint8_t LEDNumbre)
{
    FastLED.addLeds<WS2812, DATA_PIN>(_ledbuff, LEDNumbre);
    _xSemaphore = xSemaphoreCreateMutex();
    _numberled = LEDNumbre;
}

LED_Display::~LED_Display()
{

}

void LED_Display::run()
{
    //data = nullptr;
    
    for (int num = 0; num < NUM_LEDS; num++)
    {
        _ledbuff[num] = 0;
        _ledtransient[num] = NOT_TRANSIENT;
    }
    FastLED.show();
    FastLED.setBrightness(20);
    
    while (1)
    {
        xSemaphoreTake(_xSemaphore, portMAX_DELAY);
        if (_mode == kAnmiation_run)
        {
            ESP_LOGI("LED_Display", "Animating");
            if ((_am_mode & kMoveRight) || (_am_mode & kMoveLeft))
            {
                if (_am_mode & kMoveRight)
                {
                    _count_x++;
                }
                else
                {
                    _count_x--;
                }
            }
            if ((_am_mode & kMoveTop) || (_am_mode & kMoveButtom))
            {
                if (_am_mode & kMoveTop)
                {
                    _count_y--;
                }
                else
                {
                    _count_y++;
                }
            }
            if ((_am_count != -1) && (_am_count != 0))
            {
                _am_count--;
                if (_am_count == 0)
                {
                    _mode = kAnmiation_stop;
                }
            }
            _displaybuff(_am_buffptr, _count_x, _count_y);
            delay(_am_speed);
            //delay(10);
            //xSemaphoreGive(_xSemaphore);
        }
        else
        {
            //delay(10);
            // Check transient
            for (int num = 0; num < NUM_LEDS; num++)
            {
                if (_ledtransient[num] < NOT_TRANSIENT)
                {
                    if (_ledtransient[num] == 0 || --_ledtransient[num] == 0)
                    {
                        _ledbuff[num] = 0;
                        _ledtransient[num] = NOT_TRANSIENT;
                    }
                }
            }
            //xSemaphoreGive(_xSemaphore);
        }
        
        FastLED.show();
        xSemaphoreGive(_xSemaphore);
        delay(20);
    }

    ESP_LOGI("LED_DISPLAY", "END LOOP");
}

void LED_Display::_displaybuff(uint8_t *buffptr, int8_t offsetx, int8_t offsety)
{
    uint16_t xsize = 0, ysize = 0;
    xsize = buffptr[0];
    ysize = buffptr[1];

    offsetx = offsetx % xsize;
    offsety = offsety % ysize;

    int8_t setdatax = (offsetx < 0) ? (-offsetx) : (xsize - offsetx);
    int8_t setdatay = (offsety < 0) ? (-offsety) : (ysize - offsety);
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            _ledbuff[x + y * 5].raw[1] = buffptr[2 + ((setdatax + x) % xsize + ((setdatay + y) % ysize) * xsize) * 3 + 0];
            _ledbuff[x + y * 5].raw[0] = buffptr[2 + ((setdatax + x) % xsize + ((setdatay + y) % ysize) * xsize) * 3 + 1];
            _ledbuff[x + y * 5].raw[2] = buffptr[2 + ((setdatax + x) % xsize + ((setdatay + y) % ysize) * xsize) * 3 + 2];
        }
    }
    FastLED.setBrightness(20);
}

void LED_Display::animation(uint8_t *buffptr, uint8_t amspeed, uint8_t ammode, int64_t amcount)
{
    xSemaphoreTake(_xSemaphore, portMAX_DELAY);
    if (_mode == kAnmiation_run)
    {
        _mode = kAnmiation_stop;
    }
    _am_buffptr = buffptr;
    _am_speed = amspeed;
    _am_mode = ammode;
    _am_count = amcount;
    _count_x = _count_y = 0;
    _mode = kAnmiation_run;
    xSemaphoreGive(_xSemaphore);
}

void LED_Display::displaybuff(uint8_t *buffptr, int8_t offsetx, int8_t offsety)
{
    uint16_t xsize = 0, ysize = 0;
    xsize = buffptr[0];
    ysize = buffptr[1];

    offsetx = offsetx % xsize;
    offsety = offsety % ysize;

    int8_t setdatax = (offsetx < 0) ? (-offsetx) : (xsize - offsetx);
    int8_t setdatay = (offsety < 0) ? (-offsety) : (ysize - offsety);
    xSemaphoreTake(_xSemaphore, portMAX_DELAY);
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            _ledbuff[x + y * 5].raw[1] = buffptr[2 + ((setdatax + x) % xsize + ((setdatay + y) % ysize) * xsize) * 3 + 0];
            _ledbuff[x + y * 5].raw[0] = buffptr[2 + ((setdatax + x) % xsize + ((setdatay + y) % ysize) * xsize) * 3 + 1];
            _ledbuff[x + y * 5].raw[2] = buffptr[2 + ((setdatax + x) % xsize + ((setdatay + y) % ysize) * xsize) * 3 + 2];
        }
    }
    xSemaphoreGive(_xSemaphore);
    FastLED.setBrightness(20);
}

void LED_Display::setBrightness(uint8_t brightness)
{
    xSemaphoreTake(_xSemaphore, portMAX_DELAY);
    brightness = ( brightness > 40 ) ? 40 : brightness;
    FastLED.setBrightness(brightness);
    xSemaphoreGive(_xSemaphore);
}

void LED_Display::drawpixA(uint8_t xpos, uint8_t ypos, CRGB Color, uint8_t transient)
{
    if ((xpos >= 5) || (ypos >= 5))
    {
        return;
    }
    xSemaphoreTake(_xSemaphore, portMAX_DELAY);
    _ledbuff[xpos + ypos * 5] = Color;
    _ledtransient[xpos + ypos * 5] = transient;
    xSemaphoreGive(_xSemaphore);
}

void LED_Display::drawpixB(uint8_t Number, CRGB Color, uint8_t transient)
{
    if (Number >= NUM_LEDS)
    {
        return;
    }
    xSemaphoreTake(_xSemaphore, portMAX_DELAY);
    _ledbuff[Number] = Color;
    _ledtransient[Number] = transient;
    xSemaphoreGive(_xSemaphore);
}

void LED_Display::fillpix(CRGB Color)
{
    xSemaphoreTake(_xSemaphore, portMAX_DELAY);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        _ledbuff[i] = Color;
        _ledtransient[i] = NOT_TRANSIENT;
    }
    xSemaphoreGive(_xSemaphore);
}

void LED_Display::clear()
{
    xSemaphoreTake(_xSemaphore, portMAX_DELAY);
    for (int8_t i = 0; i < NUM_LEDS; i++)
    {
        _ledbuff[i] = 0;
        _ledtransient[i] = NOT_TRANSIENT;
    }
    xSemaphoreGive(_xSemaphore);
}
