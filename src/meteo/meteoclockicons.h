

/*
 *
 * meteoclockicons
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : meteoclockicons6!.h
 * Date                : 17.01.2021
 * Font size in bytes  : 2631
 * Font width          : 14
 * Font height         : 15
 * Font first char     : 32
 * Font last char      : 47
 * Font used chars     : 15
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#ifndef METEOCLOCKICONS_H
#define METEOCLOCKICONS_H

#define METEOCLOCKICONS_WIDTH 14
#define METEOCLOCKICONS_HEIGHT 15

GLCDFONTDECL(meteoclockicons) = {
    0x0A, 0x47, // size
    0x0E, // width
    0x0F, // height
    0x20, // first char
    0x0F, // char count
    
    // char widths
    0x07, 0x0E, 0x0F, 0x08, 0x0D, 0x0D, 0x0D, 0x08, 
    0x08, 0x06, 0x0E, 0x0C, 0x10, 0x0E, 0x0D, 
    
    // font data
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 32
    0x00, 0x00, 0x80, 0xC0, 0x70, 0x18, 0xC4, 0xE4, 0x0C, 0x38, 0xE0, 0x80, 0x00, 0x00, 0x38, 0x7C, 0x46, 0x40, 0x40, 0x40, 0x58, 0x5A, 0x40, 0x40, 0x40, 0x42, 0x4E, 0x78, // 33
    0x20, 0x70, 0xB8, 0xD8, 0xD8, 0xEC, 0x6C, 0x6C, 0x6C, 0x6C, 0xEC, 0xD8, 0xD8, 0xB8, 0x70, 0x00, 0x00, 0x00, 0x02, 0x00, 0x0C, 0x06, 0x36, 0x36, 0x06, 0x0C, 0x00, 0x02, 0x00, 0x00, // 34
    0x00, 0x80, 0xC0, 0xFE, 0xF0, 0xC0, 0x80, 0x00, 0x3C, 0x66, 0xE0, 0xFE, 0xFE, 0xFE, 0x7E, 0x3C, // 35
    0x20, 0xB0, 0x90, 0x98, 0x98, 0x90, 0xB0, 0x20, 0x20, 0x20, 0x20, 0x90, 0x90, 0x32, 0x12, 0x18, 0x08, 0x18, 0x18, 0x12, 0x32, 0x26, 0x26, 0x32, 0x1A, 0x10, // 36
    0xE0, 0xF8, 0xFE, 0xF0, 0x40, 0x00, 0x80, 0x00, 0x00, 0xF0, 0xFE, 0xF8, 0xE0, 0x00, 0x02, 0x02, 0x02, 0x60, 0xF8, 0xFE, 0xF8, 0x70, 0x02, 0x02, 0x02, 0x00, // 37
    0x50, 0x50, 0x78, 0x60, 0x44, 0x8C, 0xFF, 0x8C, 0x44, 0x70, 0x78, 0x50, 0x50, 0x0A, 0x0C, 0x3C, 0x0C, 0x42, 0x22, 0xFE, 0x22, 0x42, 0x0C, 0x3C, 0x0C, 0x0A, // 38
    0x00, 0xFC, 0x02, 0xFA, 0x02, 0xFC, 0xA8, 0xA8, 0x38, 0x46, 0xB8, 0xBE, 0xB8, 0x46, 0x38, 0x00, // 39
    0x00, 0xFC, 0x02, 0xE2, 0x02, 0xFC, 0xA8, 0xA8, 0x38, 0x46, 0xB8, 0xBE, 0xB8, 0x46, 0x38, 0x00, // 40
    0xC0, 0xC0, 0x00, 0xC0, 0x40, 0x40, 0x00, 0x00, 0x00, 0x0E, 0x08, 0x08, // 41
    0x80, 0x80, 0x40, 0x40, 0xE0, 0x60, 0x70, 0x9C, 0x84, 0x08, 0x90, 0xA0, 0xD6, 0x06, 0x02, 0x04, 0x08, 0x12, 0x24, 0x52, 0x86, 0xC6, 0x30, 0x0E, 0x00, 0x02, 0x00, 0x00, // 42
    0x0C, 0xCC, 0xCC, 0xCC, 0xCE, 0xCE, 0xCE, 0xCC, 0xCC, 0xCC, 0xCC, 0x0C, 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, // 43
    0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x06, 0x0E, 0x1E, 0x3C, 0x78, 0xFC, 0x7E, 0x1E, 0x0E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, // 44
    0x00, 0x00, 0x00, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80, 0x80, 0x00, 0x1C, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x1E, 0x0E, // 45
    0x40, 0xC0, 0xF8, 0xF8, 0xF0, 0xF8, 0x1C, 0xF0, 0xF0, 0xF8, 0xCC, 0xC0, 0x40, 0x08, 0x0C, 0x6E, 0x7E, 0x3E, 0x7E, 0xF4, 0x3E, 0x3E, 0x7E, 0x1E, 0x0C, 0x08 // 46
    
};

#endif
