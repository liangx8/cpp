#pragma once
#include <stdint.h>

#include <stdio.h>
struct OFFSET_DATA{
    uint32_t format;
    int32_t count;
    long seek_offsets;
    long data_size;
    long seek_data;
};
typedef struct OFFSET_DATA TABLE_BITMAPS ;
typedef struct OFFSET_DATA TABLE_GLYPH_NAMES;
struct TABLE_BDF_ENCODINGS{
    uint32_t format;
    int16_t default_char;
    int16_t _unused;
    long seek_offset;
};
struct TABLE_METRICS{
    uint32_t format;
    int32_t count;
    long seek_offsets;
};
struct FONT_TOC{
    FILE *pcf;
    long properties;
    long accelerators;
    struct TABLE_METRICS metrics;
    TABLE_BITMAPS bitmaps;
    long ink_metrics;
    struct TABLE_BDF_ENCODINGS bdf_encodings;
    long swidths;
    TABLE_GLYPH_NAMES glyph_names;
    long bdf_accelerators;
};

// type field
#define PCF_PROPERTIES              (1<<0)
#define PCF_ACCELERATORS            (1<<1)
#define PCF_METRICS                 (1<<2)
#define PCF_BITMAPS                 (1<<3)
#define PCF_INK_METRICS             (1<<4)
#define PCF_BDF_ENCODINGS           (1<<5)
#define PCF_SWIDTHS                 (1<<6)
#define PCF_GLYPH_NAMES             (1<<7)
#define PCF_BDF_ACCELERATORS        (1<<8)

// format field
#define PCF_DEFAULT_FORMAT      0x00000000
#define PCF_INKBOUNDS           0x00000200
#define PCF_ACCEL_W_INKBOUNDS   0x00000100
#define PCF_COMPRESSED_METRICS  0x00000100

// format field may be modified by
#define PCF_GLYPH_PAD_MASK      (3<<0)            /* See the bitmap table for explanation */
#define PCF_BYTE_MASK           (1<<2)            /* If set then Most Sig Byte First */
#define PCF_BIT_MASK            (1<<3)            /* If set then Most Sig Bit First */
#define PCF_SCAN_UNIT_MASK      (3<<4)            /* See the bitmap table for explanation */


uint32_t msbyte4(unsigned char b[]);
void toc_comm(uint8_t *pcf,long size,long offset,uint32_t format,int seq);