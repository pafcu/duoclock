#pragma once
#include <pebble.h>

#define LANG_COUNT 6
#define MAX_NUMBER_LENGTH 50
#define TEXT_LAYER_COUNT 5

#define LAYOUT_SEPARATOR {10,28,60,78,138}
#define LAYOUT_NO_SEPARATOR {28,46,-1,78,138}
#define LAYOUT_NO_SEPARATOR_LONG_HOUR {5,23,-1,83,140}

#define DEFAULT_BIG_FONT FONT_KEY_GOTHIC_28_BOLD
#define DEFAULT_SMALL_FONT FONT_KEY_GOTHIC_18

typedef enum {LANG_FR, LANG_EN, LANG_FI, LANG_SV, LANG_DE, LANG_RU} lang_t;

typedef struct {
  lang_t code;
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

language_t languages[] = {
  {
    .lang_code = "fr",
    .intro = "Il est",
    .separator = "heures",
    .tens = {"vingt","trente","quarante","cinquante","soixante"},
    .first = {"zéro","un","deux","trois","quatre","cinq","six","sept","huit","neuf","dix","onze","douze","trieze","quatorze","quinze","seixe","dix-sept","dix-huit","dix-neuf"},
    .bigfont = DEFAULT_BIG_FONT,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_SEPARATOR
  },
  {
    .lang_code = "en",
    .intro = "It is",
    .separator = "",
    .tens = {"twenty","thirty","fourty","fifty","sixty"},
    .first = {"zero","one","two","three","four","five","six","seven","eight","nine","ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"},
    .bigfont = DEFAULT_BIG_FONT,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_NO_SEPARATOR
  },
  {
    .lang_code = "fi",
    .intro = "Kello on",
    .separator = "",
    .tens = {"kaksikymmentä","kolmekymmentä","neljäkymmentä","viisikymmentä","kuusikymmentä"},
    .first = {"nolla","yksi","kaksi","kolme","neljä","viisi","kuusi","seitsemän","kahdeksan","yhdeksän","kymmenen","yksitoista","kaksitoista","kolmetoista","neljätoista","viisitoista","kuusitoista","seitsemäntoista","kahdeksantoista","yhdeksäntoista"},
    .bigfont = FONT_KEY_GOTHIC_24_BOLD,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_NO_SEPARATOR_LONG_HOUR
  },
  {
    .lang_code = "sv",
    .intro = "Klockan är",
    .separator = "",
    .tens = {"tjugo","trettio","fyrttio","femtio","sextio"},
    .first = {"noll","ett","två","tre","fyra","fem","sex","sju","åtta","nio","tio","elva","tolv","tretton","fjorton","femton","sexton","sjutton","aderton","nitton"},
    .bigfont = DEFAULT_BIG_FONT,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_NO_SEPARATOR
  },
  {
    .lang_code = "de",
    .intro = "Es ist",
    .separator = "Uhr",
    .tens = {"zwanzig","dreißig","vierzig","fünfzig","sechzig"},
    .first = {"null","ein","zwei","drei","vier","fünf","sechs","sieben","acht","neun","zehn","elf","zwölf","dreizehn","vierzehn","fünfzehn","sechzehn","siebzehn","achtzehn","neunzehn"},
    .bigfont = FONT_KEY_GOTHIC_24_BOLD,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_SEPARATOR
  },
  {
    .lang_code = "ru",
    .intro = "",
    .separator = "",
    .tens = {"двадцать","тридцать","сорок","пятьдесят","шестьдесят"},
    .first = {"ноль","один","два","три","четыре","пять","шесть","семь","восемь","девять","десять","одиннадцать","двенадцать","тринадцать","четырнадцать","пятнадцать","шестнадцать","семнадцать","восемнадцать","девятнадцать"},
    .bigfont = (char*)RESOURCE_ID_FONT_DIDACT_CYRILLIC_22,
    .smallfont = (char*)RESOURCE_ID_FONT_DIDACT_CYRILLIC_22,
    .customfont = true,
    .layout = LAYOUT_NO_SEPARATOR
  },
};