#pragma once
#define MAX_NUMBER_LENGTH 32

typedef struct {
  int code;
  char lang_code[3];
  char intro[12];
  char separator[12];
  char tens[5][16];
  char first[20][MAX_NUMBER_LENGTH];
} language_t;

#define LANG_COUNT 5

typedef enum {LANG_FR, LANG_EN, LANG_FI, LANG_SV, LANG_DE} lang_t;

language_t languages[] = {
  {
    .lang_code = "fr",
    .intro = "Il est",
    .separator = "heures",
    .tens = {"vingt","trente","quarante","cinquante","soixante"},
    .first = {"zéro","un","deux","trois","quatre","cinq","six","sept","huit","neuf","dix","onze","douze","trieze","quatorze","quinze","seixe","dix-sept","dix-huit","dix-neuf"}
  },
  {
    .lang_code = "en",
    .intro = "It is",
    .separator = "",
    .tens = {"twenty","thirty","fourty","fifty","sixty"},
    .first = {"zero","one","two","three","four","five","six","seven","eight","nine","ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"},
  },
  {
    .lang_code = "fi",
    .intro = "Kello on",
    .separator = "",
    .tens = {"kaksikymmentä","kolmekymmentä","neljäkymmentä","viisikymmentä","kuusikymmentä"},
    .first = {"nolla","yksi","kaksi","kolme","neljä","viisi","kuusi","seitsemän","kahdeksan","yhdeksän","kymmenen","yksitoista","kaksitoista","kolmetoista","neljätoista","viisitoista","kuusitoista","seitsemäntoista","kahdeksantoista","yhdeksäntoista"},
  },
  {
    .lang_code = "sv",
    .intro = "Klockan är",
    .separator = "",
    .tens = {"tjugo","trettio","fyrttio","femtio","sextio"},
    .first = {"noll","ett","två","tre","fyra","fem","sex","sju","åtta","nio","tio","elva","tolv","tretton","fjorton","femton","sexton","sjutton","aderton","nitton"},
  },
  {
    .lang_code = "de",
    .intro = "Es ist",
    .separator = "Uhr",
    .tens = {"zwanzig","dreißig","vierzig","fünfzig","sechzig"},
    .first = {"null","ein","zwei","drei","vier","fünf","sechs","sieben","acht","neun","zehn","elf","zwölf","dreizehn","vierzehn","fünfzehn","sechzehn","achtzehn","neunzehn"},
  },
};