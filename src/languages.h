#pragma once
#include <pebble.h>
#include "duo_clock.h"

#define LANG_COUNT 6
typedef enum {LANG_FR, LANG_EN, LANG_FI, LANG_SV, LANG_DE, LANG_RU} lang_t;

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
    .first = {"null","ein","zwei","drei","vier","fünf","sechs","sieben","acht","neun","zehn","elf","zwölf","dreizehn","vierzehn","fünfzehn","sechzehn","achtzehn","neunzehn"},
    .bigfont = DEFAULT_BIG_FONT,
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