/*
Copyright Stefan Parviainen <pafcu@iki.fi> 2015

This file is part of DuoClock.

DuoClock is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DuoClock is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DuoClock.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include <pebble.h>

#define LANG_COUNT 7
#define MAX_NUMBER_LENGTH 50
#define TEXT_LAYER_COUNT 5

#define LAYOUT_SEPARATOR {4,23,57,78,138}
#define LAYOUT_NO_SEPARATOR {28,46,-1,78,138}
#define LAYOUT_NO_SEPARATOR_LONG_HOUR {5,23,-1,83,140}

#define DEFAULT_BIG_FONT FONT_KEY_GOTHIC_28_BOLD
#define DEFAULT_SMALL_FONT FONT_KEY_GOTHIC_18

typedef enum {LANG_FR, LANG_EN, LANG_FI, LANG_SV, LANG_DE, LANG_ES, LANG_RU} lang_t;

typedef const char* (*time_cb)(int h, int m);

typedef struct {
  lang_t code;
  int16_t layout[TEXT_LAYER_COUNT];
  char lang_code[3];
  char intro[12];
  time_cb intro_callback;
  char separator[12];
  time_cb separator_callback;
  char tens[5][32];
  char first[20][MAX_NUMBER_LENGTH];
  char* bigfont;
  char* smallfont;
  bool customfont;
} language_t;

const char* separator_fr(int h, int m);
const char* separator_ru(int h, int m);

const char* intro_es(int h, int m);

language_t languages[] = {
  {
    .lang_code = "fr",
    .intro = "Il est",
    .intro_callback = NULL,
    .separator = "",
    .tens = {"vingt","trente","quarante","cinquante","soixante"},
    .first = {"zéro","un","deux","trois","quatre","cinq","six","sept","huit","neuf","dix","onze","douze","trieze","quatorze","quinze","seixe","dix-sept","dix-huit","dix-neuf"},
    .bigfont = DEFAULT_BIG_FONT,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_SEPARATOR,
    .separator_callback = separator_fr
  },
  {
    .lang_code = "en",
    .intro = "It is",
    .intro_callback = NULL,
    .separator = "",
    .tens = {"twenty","thirty","fourty","fifty","sixty"},
    .first = {"zero","one","two","three","four","five","six","seven","eight","nine","ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"},
    .bigfont = DEFAULT_BIG_FONT,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_NO_SEPARATOR,
    .separator_callback = NULL
  },
  {
    .lang_code = "fi",
    .intro = "Kello on",
    .intro_callback = NULL,
    .separator = "",
    .tens = {"kaksikymmentä","kolmekymmentä","neljäkymmentä","viisikymmentä","kuusikymmentä"},
    .first = {"nolla","yksi","kaksi","kolme","neljä","viisi","kuusi","seitsemän","kahdeksan","yhdeksän","kymmenen","yksitoista","kaksitoista","kolmetoista","neljätoista","viisitoista","kuusitoista","seitsemäntoista","kahdeksantoista","yhdeksäntoista"},
    .bigfont = FONT_KEY_GOTHIC_24_BOLD,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_NO_SEPARATOR_LONG_HOUR,
    .separator_callback = NULL
  },
  {
    .lang_code = "sv",
    .intro = "Klockan är",
    .intro_callback = NULL,
    .separator = "",
    .tens = {"tjugo","trettio","fyrttio","femtio","sextio"},
    .first = {"noll","ett","två","tre","fyra","fem","sex","sju","åtta","nio","tio","elva","tolv","tretton","fjorton","femton","sexton","sjutton","aderton","nitton"},
    .bigfont = DEFAULT_BIG_FONT,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_NO_SEPARATOR,
    .separator_callback = NULL
  },
  {
    .lang_code = "de",
    .intro = "Es ist",
    .intro_callback = NULL,
    .separator = "Uhr",
    .tens = {"zwanzig","dreißig","vierzig","fünfzig","sechzig"},
    .first = {"null","ein","zwei","drei","vier","fünf","sechs","sieben","acht","neun","zehn","elf","zwölf","dreizehn","vierzehn","fünfzehn","sechzehn","siebzehn","achtzehn","neunzehn"},
    .bigfont = FONT_KEY_GOTHIC_24_BOLD,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_SEPARATOR,
    .separator_callback = NULL
  },
  {
    .lang_code = "es",
    .intro = "",
    .intro_callback = intro_es,
    .separator = "y",
    .tens = {"veinte","treinta","cuarenta","cincuenta","sesenta"},
    .first = {"cero","uno","dos","tres","cuatro","cinco","seis","siete","ocho","nueve","diez","once","doce","trece","catorce","quince","dieciséis","diecisiete","dieciocho","diecinueve"},
    .bigfont = DEFAULT_BIG_FONT,
    .smallfont = DEFAULT_SMALL_FONT,
    .customfont = false,
    .layout = LAYOUT_SEPARATOR,
    .separator_callback = NULL
  },
  {
    .lang_code = "ru",
    .intro = "",
    .intro_callback = NULL,
    .separator = "",
    .tens = {"двадцать","тридцать","сорок","пятьдесят","шестьдесят"},
    .first = {"ноль","один","два","три","четыре","пять","шесть","семь","восемь","девять","десять","одиннадцать","двенадцать","тринадцать","четырнадцать","пятнадцать","шестнадцать","семнадцать","восемнадцать","девятнадцать"},
    .bigfont = (char*)RESOURCE_ID_FONT_DIDACT_CYRILLIC_22,
    .smallfont = (char*)RESOURCE_ID_FONT_DIDACT_CYRILLIC_18,
    .customfont = true,
    .layout = {10,22,55,78,138}, // Slightly modified layout due to different font
    .separator_callback = separator_ru
  },
};

const char* separator_fr(int h, int m) {
  if(h < 2) {
    return "heure";
  }
  else {
    return "heures";
  }
}

const char* separator_ru(int h, int m) {
  if(h <= 1) {
    return "Час";
  }
  else if (h<=4) {
    return "часа";
  }
  else {
    return "часов";
  }
}

const char* intro_es(int h, int m) {
  if(h <= 1) {
    return "Es la";
  }
  else {
    return "Son las";
  }
}

static void make_numbers(language_t* lang, char numbers_str[60][MAX_NUMBER_LENGTH]) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Making numbers for language %s %d",lang->lang_code,lang->code);

  memcpy(numbers_str,lang->first,sizeof(char)*20*MAX_NUMBER_LENGTH);
  if(lang->code == LANG_ES) {
    strcpy(numbers_str[1],"una"); // Just 1 is slightly different  
  }
  
  for(int tens=2;tens<6; tens++) {
    for(int digit=0;digit<10;digit++) {
      if(digit == 0) {
        snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"%s",lang->tens[tens-2]);
      }
      else {
        switch(lang->code) {
          case LANG_FR:
            if(digit == 1) {
              snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"%s et %s",lang->tens[tens-2],lang->first[digit]);
            }
            else {
              snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"%s-%s",lang->tens[tens-2],lang->first[digit]);
            }
            break;
          
          case LANG_EN:
            snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"%s-%s",lang->tens[tens-2],lang->first[digit]);
            break;
          
          case LANG_FI:
            snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"%s-\n%s",lang->tens[tens-2],lang->first[digit]);
            break;
          
          case LANG_SV:
            snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"%s%s",lang->tens[tens-2],lang->first[digit]);
            break;
          
          case LANG_DE:
            snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"%s und %s",lang->first[digit],lang->tens[tens-2]);
            break;
          
          case LANG_ES:
            if(tens==2) {
              snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"veinti%s",lang->first[digit]);
            }
            else {
              snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"%s y %s",lang->tens[tens-2], lang->first[digit]);
            }
          
          case LANG_RU:
            snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"%s %s",lang->tens[tens-2],lang->first[digit]);
            break;
        }  
      }
    }
  }
   APP_LOG(APP_LOG_LEVEL_DEBUG, "Made numbers");
}

/* vim: set expandtab tabstop=2 shiftwidth=2: */
