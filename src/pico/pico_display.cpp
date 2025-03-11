//
// Created by cpasjuste on 29/03/23.
//

#include <hardware/i2c.h>
#include <hardware/gpio.h>

#include "main.h"
#include "pico_display.h"

using namespace uGamePad;

struct render_area {
    uint8_t start_col;
    uint8_t end_col;
    uint8_t start_page;
    uint8_t end_page;
    int buflen;
};

static render_area frame_area;

static uint8_t framebuffer[SSD1306_BUF_LEN];

static void SSD1306_init();

static void SSD1306_SetPixel(uint8_t *buf, int x, int y, bool on);

static void SSD1306_Render(uint8_t *buf, struct render_area *area);

static void calc_render_area_buflen(struct render_area *area);

PicoDisplay::PicoDisplay() : Adafruit_GFX(SSD1306_WIDTH, SSD1306_HEIGHT) {
    // I2C is "open drain", pull-ups to keep signal high when no data is being sent
    const auto rate = i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
    printf("PicoDisplay: i2c rate: %i\r\n", rate);
    gpio_set_function(GPIO_OLED_SDA, GPIO_FUNC_I2C);
    gpio_set_function(GPIO_OLED_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(GPIO_OLED_SDA);
    gpio_pull_up(GPIO_OLED_SCL);

    // run through the complete initialization process
    SSD1306_init();

    frame_area = {
        .start_col = 0,
        .end_col = SSD1306_WIDTH - 1,
        .start_page = 0,
        .end_page = SSD1306_NUM_PAGES - 1
    };

    calc_render_area_buflen(&frame_area);

    // zero the entire display
    memset(framebuffer, 0, SSD1306_BUF_LEN);
    SSD1306_Render(framebuffer, &frame_area);
}

void PicoDisplay::drawPixel(const int16_t x, const int16_t y, const uint16_t color) {
    SSD1306_SetPixel(framebuffer, x, y, color);
}

void PicoDisplay::clear() {
    memset(framebuffer, 0, SSD1306_BUF_LEN);
}

void PicoDisplay::flip() {
    SSD1306_Render(framebuffer, &frame_area);
}

///
///
///

static void calc_render_area_buflen(struct render_area *area) {
    // calculate how long the flattened buffer will be for a render area
    area->buflen = (area->end_col - area->start_col + 1) * (area->end_page - area->start_page + 1);
}

static void SSD1306_send_cmd(uint8_t cmd) {
    // I2C write process expects a control byte followed by data
    // this "data" can be a command or data to follow up a command
    // Co = 1, D/C = 0 => the driver expects a command
    uint8_t buf[2] = {0x80, cmd};
    i2c_write_blocking(i2c_default, SSD1306_I2C_ADDR, buf, 2, false);
}

static void SSD1306_send_cmd_list(uint8_t *buf, int num) {
    for (int i = 0; i < num; i++)
        SSD1306_send_cmd(buf[i]);
}

static void SSD1306_send_buf(uint8_t buf[], int buflen) {
    // in horizontal addressing mode, the column address pointer auto-increments
    // and then wraps around to the next page, so we can send the entire frame
    // buffer in one gooooooo!

    // copy our frame buffer into a new buffer because we need to add the control byte
    // to the beginning

    uint8_t *temp_buf = static_cast<uint8_t *>(malloc(buflen + 1));

    temp_buf[0] = 0x40;
    memcpy(temp_buf + 1, buf, buflen);

    i2c_write_blocking(i2c_default, SSD1306_I2C_ADDR, temp_buf, buflen + 1, false);

    free(temp_buf);
}

static void SSD1306_init() {
    // Some of these commands are not strictly necessary as the reset
    // process defaults to some of these, but they are shown here
    // to demonstrate what the initialization sequence looks like
    // Some configuration values are recommended by the board manufacturer

    uint8_t cmds[] = {
        SSD1306_SET_DISP, // set display off
        /* memory mapping */
        SSD1306_SET_MEM_MODE, // set memory address mode 0 = horizontal, 1 = vertical, 2 = page
        0x00, // horizontal addressing mode
        /* resolution and layout */
        SSD1306_SET_DISP_START_LINE, // set display start line to 0
        SSD1306_SET_SEG_REMAP | 0x01, // set segment re-map, column address 127 is mapped to SEG0
        SSD1306_SET_MUX_RATIO, // set multiplex ratio
        SSD1306_HEIGHT - 1, // Display height - 1
        SSD1306_SET_COM_OUT_DIR |
        0x08, // set COM (common) output scan direction. Scan from bottom up, COM[N-1] to COM0
        SSD1306_SET_DISP_OFFSET, // set display offset
        0x00, // no offset
        SSD1306_SET_COM_PIN_CFG, // set COM (common) pins hardware configuration. Board specific magic number.
        // 0x02 Works for 128x32, 0x12 Possibly works for 128x64. Other options 0x22, 0x32
#if ((SSD1306_WIDTH == 128) && (SSD1306_HEIGHT == 32))
            0x02,
#elif ((SSD1306_WIDTH == 128) && (SSD1306_HEIGHT == 64))
        0x12,
#else
            0x02,
#endif
        /* timing and driving scheme */
        SSD1306_SET_DISP_CLK_DIV, // set display clock divide ratio
        0x80, // div ratio of 1, standard freq
        SSD1306_SET_PRECHARGE, // set pre-charge period
        0xF1, // Vcc internally generated on our board
        SSD1306_SET_VCOM_DESEL, // set VCOMH deselect level
        0x30, // 0.83xVcc
        /* display */
        SSD1306_SET_CONTRAST, // set contrast control
        0xFF,
        SSD1306_SET_ENTIRE_ON, // set entire display on to follow RAM content
        SSD1306_SET_NORM_DISP, // set normal (not inverted) display
        SSD1306_SET_CHARGE_PUMP, // set charge pump
        0x14, // Vcc internally generated on our board
        SSD1306_SET_SCROLL |
        0x00,
        // deactivate horizontal scrolling if set. This is necessary as memory writes will corrupt if scrolling was enabled
        SSD1306_SET_DISP | 0x01, // turn display on
    };

    SSD1306_send_cmd_list(cmds, std::size(cmds));
}

static void SSD1306_Render(uint8_t *buf, struct render_area *area) {
    // update a portion of the display with a render area
    uint8_t cmds[] = {
        SSD1306_SET_COL_ADDR,
        area->start_col,
        area->end_col,
        SSD1306_SET_PAGE_ADDR,
        area->start_page,
        area->end_page
    };

    SSD1306_send_cmd_list(cmds, std::size(cmds));
    SSD1306_send_buf(buf, area->buflen);
}

static void SSD1306_SetPixel(uint8_t *buf, int x, int y, bool on) {
    if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) return;

    // The calculation to determine the correct bit to set depends on which address
    // mode we are in. This code assumes horizontal

    // The video ram on the SSD1306 is split up in to 8 rows, one bit per pixel.
    // Each row is 128 long by 8 pixels high, each byte vertically arranged, so byte 0 is x=0, y=0->7,
    // byte 1 is x = 1, y=0->7 etc

    // This code could be optimised, but is like this for clarity. The compiler
    // should do a half decent job optimising it anyway.

    constexpr int BytesPerRow = SSD1306_WIDTH; // x pixels, 1bpp, but each row is 8 pixel high, so (x / 8) * 8
    const int byte_idx = (y / 8) * BytesPerRow + x;
    uint8_t byte = buf[byte_idx];

    if (on)
        byte |= 1 << (y % 8);
    else
        byte &= ~(1 << (y % 8));

    buf[byte_idx] = byte;
}
