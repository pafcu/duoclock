#pragma once
#include <pebble.h>
#include "languages.h"

static char* get_word(char separator, char* c);
static void update_word(time_t time);
static void update_layout();
static void inbox_received_callback(DictionaryIterator *iterator, void *context);
static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed);
static void handle_init(void);
static void handle_deinit(void);
static void set_language(lang_t lang, bool get_words);
static void click_config_provider(void *context);
static void request_words();