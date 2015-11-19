#pragma once
#include <pebble.h>
#define MAX_NUMBER_LENGTH 50
#define TEXT_LAYER_COUNT 5

#define LAYOUT_SEPARATOR {10,28,60,78,134}
#define LAYOUT_NO_SEPARATOR {28,46,-1,78,134}
#define LAYOUT_NO_SEPARATOR_LONG_HOUR {5,23,-1,83,140}

#define DEFAULT_BIG_FONT FONT_KEY_GOTHIC_28_BOLD
#define DEFAULT_SMALL_FONT FONT_KEY_GOTHIC_18

typedef struct {
  uint8_t code; // Should actually be lang_t
  int16_t layout[TEXT_LAYER_COUNT];
  char lang_code[3];
  char intro[12];
  char separator[12];
  char tens[5][32];
  char first[20][MAX_NUMBER_LENGTH];
  char* bigfont;
  char* smallfont;
  bool customfont;
} language_t;