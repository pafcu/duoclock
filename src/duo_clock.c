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

/*
Main file 
*/

#include <pebble.h>
#include "duo_clock.h"

#define KEY_KNOWN_WORDS 0
#define KEY_NUM_KNOWN_WORDS 1
#define KEY_LANGUAGE 2
#define KEY_SEND_WORDS 3

#define PERSIST_LANGUAGE 0

static Window* window; // Main window
static Layer* time_layer; // Layer containing all the other layers
static TextLayer *text_layers[TEXT_LAYER_COUNT]; // Individual text layers (hour, minutes, word, etc.)

static lang_t current_language = 0; // Index of currently selected language

static char numbers_str[60][MAX_NUMBER_LENGTH]; // Array of numbers as text

static int num_words = 0; // Number of words from Duolingo
static char** words = 0; // Array of pointers to the words from Duolingo
static char* words_buffer = 0; // Buffer containing all the words from Duolingo

static bool demo_mode = false; // Cycle numbers more quickly to check that all the numbers look OK


// A bit like strtok
static char* get_word(char separator, char* c) {
  while(*c != '\0' && *c != separator) c++;
  *c = '\0';
  return c+1;
}

static void update_word(time_t time) {
  if(num_words > 0) {
    text_layer_set_text(text_layers[4], words[time%num_words]);
  }
}

// Set fonts, positions, etc. of text layers
static void update_layout() {
  const language_t *cur_lang = &languages[current_language];
  
  // Pick a good layout for the texts
  const int16_t *layout = cur_lang->layout;
  
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
  GFont bigfont;
  GFont smallfont;
  if(cur_lang->customfont) {
    bigfont = fonts_load_custom_font(resource_get_handle((uint32_t)cur_lang->bigfont));
    smallfont = fonts_load_custom_font(resource_get_handle((uint32_t)cur_lang->smallfont));
  }
  else {
    bigfont = fonts_get_system_font(cur_lang->bigfont);
    smallfont = fonts_get_system_font(cur_lang->smallfont);
  }
  
  text_layer_set_font(text_layers[0], smallfont);
  text_layer_set_font(text_layers[1], bigfont);
  text_layer_set_font(text_layers[2], smallfont);
  text_layer_set_font(text_layers[3], bigfont);
  text_layer_set_font(text_layers[4], smallfont);
  
  for(int i=0;i<TEXT_LAYER_COUNT;i++) {
    Layer *text_layer = text_layer_get_layer(text_layers[i]);
    
    layer_set_hidden(text_layer,layout[i]<0);
    
    if(layout[i] >= 0) {
      GRect old_bounds = layer_get_frame(text_layer);
      GRect new_bounds;
      new_bounds.origin.x = old_bounds.origin.x;
      new_bounds.size.w = old_bounds.size.w;
      
      // Extend this layer until the next one start
      if(i < TEXT_LAYER_COUNT - 1) {
        if(layout[i+1] > 0) { // BUG: This will only work as long as there are not two hidden fields or the last field is not hidden
          new_bounds.size.h = layout[i+1] - layout[i];  
        }
        else {
          new_bounds.size.h = layout[i+2] - layout[i];  
        }
        
      }
      else {
        new_bounds.size.h = (bounds.origin.x+bounds.size.h) - layout[i];
      }
      APP_LOG(APP_LOG_LEVEL_DEBUG, "layer %d has height %d",i,new_bounds.size.h);

      new_bounds.origin.y = layout[i];
      layer_set_frame(text_layer,new_bounds);
    }
  }
}


static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Pebble received appmessage");
  
  Tuple *word_tuple = dict_find(iterator, KEY_KNOWN_WORDS);
  Tuple *num_words_tuple = dict_find(iterator, KEY_NUM_KNOWN_WORDS);
  Tuple *language_tuple = dict_find(iterator, KEY_LANGUAGE);

  int len = 0;
  if(!num_words_tuple || !word_tuple || !language_tuple) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error in inbox recv %p %p %p",num_words_tuple, word_tuple, language_tuple);
    return;
  }
  
  num_words = num_words_tuple->value->int32;
  
  if(words) {
    free(words);
    words = 0;
  }
  
  if(num_words < 1) {  
    text_layer_set_text(text_layers[4], "");
    set_language(language_tuple->value->int32, false);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Got no words, returning");
    return;
  }
  
  words = (char**)malloc(num_words*sizeof(char*));

  len = strlen(word_tuple->value->cstring);

  if(words_buffer) {
    free(words_buffer);
  }
  words_buffer = (char*)malloc(len+1);

  strncpy(words_buffer,word_tuple->value->cstring,len);
  words_buffer[len] = '\0';
    
  words[0] = words_buffer;
  for(int i=1;i<num_words; i++) {
    words[i] = get_word('|', words[i-1]);
  }
  
  set_language(language_tuple->value->int32, false);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Language %d num words %d",current_language,num_words);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed) {
  int h, m;
  if(demo_mode) {
    h = tick_time->tm_sec%24;
    m = tick_time->tm_sec;
  }
  else {
    h = tick_time->tm_hour;
    m = tick_time->tm_min;
  }
  
  text_layer_set_text(text_layers[1],numbers_str[h]);
  text_layer_set_text(text_layers[3],numbers_str[m]);
  
  if(languages[current_language].intro_callback) {
    text_layer_set_text(text_layers[0],languages[current_language].intro_callback(h,m));
  }
  
  if(languages[current_language].separator_callback) {
    text_layer_set_text(text_layers[2],languages[current_language].separator_callback(h,m));
  }
  update_word(mktime(tick_time));
}

static void handle_init(void) {
  for(int i=0;i<LANG_COUNT;i++) {
    languages[i].code = i;
  }

  if(persist_exists(PERSIST_LANGUAGE)) {
    int tmp_lang = persist_read_int(PERSIST_LANGUAGE); // Use tmp int so we can compare against negative values
    if(tmp_lang < 0 || tmp_lang > LANG_COUNT -1) {
      current_language = 0;
    }
    else {
      current_language = tmp_lang;
    }
  }
  else {
    current_language = 0;
  }
  
  window = window_create();
  window_set_background_color(window, GColorBlack);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  time_layer = layer_create(bounds);

  for(int i=0;i<TEXT_LAYER_COUNT; i++) {
    text_layers[i] = text_layer_create(GRect(0, 0, bounds.size.w, 0));
	  text_layer_set_text_alignment(text_layers[i], GTextAlignmentCenter);
    text_layer_set_text_color(text_layers[i], GColorWhite);
    text_layer_set_background_color(text_layers[i], GColorBlack);
    layer_add_child(time_layer, text_layer_get_layer(text_layers[i]));
  }
  
  layer_add_child(window_layer, time_layer);
	window_stack_push(window, true);
	
  set_language(current_language, false);
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  window_set_click_config_provider(window, click_config_provider);
}

static void set_language(lang_t lang, bool get_words) {
  current_language = lang;
  make_numbers(&languages[lang], numbers_str);
  text_layer_set_text(text_layers[0], languages[lang].intro);
  
  if(!languages[lang].separator_callback) {
    text_layer_set_text(text_layers[2], languages[lang].separator);  
  }
  
  text_layer_set_text(text_layers[4], "");

  update_layout();
  persist_write_int(PERSIST_LANGUAGE, current_language);
  time_t curtime = time(NULL);

  handle_minute_tick(localtime(&curtime), 0);
  
  if(get_words) {
    request_words();
  }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(current_language == 0) {
    set_language(LANG_COUNT - 1, true);
  }
  else {
    set_language(current_language - 1, true);
  }
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(current_language >= LANG_COUNT - 1) {
    set_language(0, true);
  }
  else {
    set_language(current_language + 1, true);
  }
}
static void mid_click_handler(ClickRecognizerRef recognizer, void *context) {
  demo_mode = !demo_mode;
   
  if(demo_mode) {
    tick_timer_service_subscribe(SECOND_UNIT, handle_minute_tick);
  }
  else {
      tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, mid_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void request_words() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Message requesting words");

  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_uint8(iter, KEY_SEND_WORDS, current_language);
  app_message_outbox_send();
}

static void handle_deinit(void) {
	for(int i=0;i<4;i++) {
    text_layer_destroy(text_layers[i]);
  }
  layer_destroy(time_layer);

  if(words_buffer) free(words_buffer);
  if(words) free(words);
  
  window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
