#include <pebble.h>
#include "duo_clock.h"

#define KEY_KNOWN_WORDS 0
#define KEY_NUM_KNOWN_WORDS 1
#define KEY_LANGUAGE 2

#define PERSIST_LANGUAGE 0

static Window *window;
static Layer* time_layer;
static TextLayer *text_layers[TEXT_LAYER_COUNT];

static lang_t current_language = 0;

static char numbers_str[60][MAX_NUMBER_LENGTH];
static int num_words = 0;
static char** words = 0;
static char* words_buffer = 0;

static bool demo_mode = false;


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


static void make_numbers(language_t* lang) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Making numbers for language %s %d",lang->lang_code,lang->code);

  memcpy(numbers_str,lang->first,sizeof(char)*20*MAX_NUMBER_LENGTH);
  
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
          
          case LANG_RU:
            snprintf(numbers_str[(tens)*10+digit],MAX_NUMBER_LENGTH,"%s %s",lang->tens[tens-2],lang->first[digit]);
            break;
        }  
      }
    }
  }
   APP_LOG(APP_LOG_LEVEL_DEBUG, "Made numbers");
}

static void update_layout() {
  const language_t *cur_lang = &languages[current_language];
  
  // Pick a good layout for the texts
  const int16_t *layout = cur_lang->layout;
  
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
  text_layer_set_text(text_layers[0], cur_lang->intro);
  text_layer_set_text(text_layers[2], cur_lang->separator);
  
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
  Tuple *word_tuple = dict_find(iterator, KEY_KNOWN_WORDS);
  Tuple *num_words_tuple = dict_find(iterator, KEY_NUM_KNOWN_WORDS);
  Tuple *language_tuple = dict_find(iterator, KEY_LANGUAGE);

  int len = 0;
  if(!num_words_tuple || !word_tuple || !language_tuple) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error in inbox recv");
    return;
  }
  
  set_language(language_tuple->value->int32, false);
  
  num_words = num_words_tuple->value->int32;
  APP_LOG(APP_LOG_LEVEL_ERROR, "Language %d num words %d",current_language,num_words);
  if(num_words < 1) {
    return;
  }
  
  if(words) free(words);
  words = (char**)malloc(num_words*sizeof(char*));
  
  len = strlen(word_tuple->value->cstring);

  if(words_buffer) free(words_buffer);
  words_buffer = (char*)malloc(len+1);

  strncpy(words_buffer,word_tuple->value->cstring,len);
  words_buffer[len] = '\0';
    
  words[0] = words_buffer;
  for(int i=1;i<num_words; i++) {
    words[i] = get_word('|', words[i-1]);
  }
  
  update_word(time(0));
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
  
  update_word(mktime(tick_time));
}

static void handle_init(void) {
  for(int i=0;i<LANG_COUNT;i++) {
    languages[i].code = i;
  }

  if(persist_exists(PERSIST_LANGUAGE)) {
    current_language = persist_read_int(PERSIST_LANGUAGE);
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

void set_language(lang_t lang, bool get_words) {
  current_language = lang;
  make_numbers(&languages[lang]);
  update_layout();
  persist_write_int(PERSIST_LANGUAGE, current_language);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(current_language == 0) {
    set_language(LANG_COUNT-1, true);
  }
  else {
    set_language(current_language-1, true);
  }
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(current_language >= LANG_COUNT) {
    set_language(0, true);
  }
  else {
    set_language(current_language+1, true);
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

void handle_deinit(void) {
	/*for(int i=0;i<4;i++) {
    text_layer_destroy(text_layers[i]);
  }
  layer_destroy(time_layer);

  if(words_buffer) free(words_buffer);
  if(words) free(words);
  */
  window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
