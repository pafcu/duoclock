/*
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
