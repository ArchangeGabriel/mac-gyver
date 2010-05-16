#ifndef BITMAPH
#define BITMAPH

void save_bitmap_header(FILE *F, int width, int height);
void save_buff_to_bitmap(const char *file, unsigned int w, unsigned int h, uint16_t *data);

#endif
