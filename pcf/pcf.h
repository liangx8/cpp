#pragma once

struct PCF_FONT;

struct PCF_FONT* pcf_load(const char *);
void pcf_release(struct PCF_FONT *);

int glyph_name(struct PCF_FONT *toc,unsigned index,unsigned char *gname,int size);
unsigned encoding_index(struct PCF_FONT *font,unsigned short ucs);
int pcf_bitmaps(struct PCF_FONT *font,unsigned font_index,unsigned char *buf);
int pcf_metrics(struct PCF_FONT *font,unsigned font_idx,unsigned char *buf);
int pcf_swidths(struct PCF_FONT *toc,unsigned font_idx);