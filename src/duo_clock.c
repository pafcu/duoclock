#include <pebble.h>

#define KEY_KNOWN_WORDS 0
#define KEY_NUM_KNOWN_WORDS 1

Window *window;

Layer* time_layer;
TextLayer *text_layers[5];

char french_tens[5][32] = {"vingt","trente","quarante","cinquante","soixante"};
char french_numbers[60][32] = {"zéro","un","deux","trois","quatre","cinq","six","sept","huit","neuf","dix","onze","douze","trieze","quatorze","quinze","seixe","dix-sept","dix-huit","dix-neuf"};

int num_words = 0;
char** words = 0; // Max 1000 words
char* words_buffer = 0;


char* get_word(char separator, char* c) {
  while(*c != '\0' && *c != separator) c++;
  *c = '\0';
  return c+1;
}

void update_word(time_t time) {
    text_layer_set_text(text_layers[4],words[time%num_words]);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Got data!");
  Tuple *word_tuple = dict_find(iterator, KEY_KNOWN_WORDS);
  Tuple *num_words_tuple = dict_find(iterator, KEY_NUM_KNOWN_WORDS);
  
  int len = 0;
  
  if(!num_words_tuple || !word_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Error in inbox recv");
    return;
  }
  
  num_words = num_words_tuple->value->int32;
  
  if(words) free(words);
  words = (char**)malloc(num_words*sizeof(char*));
  
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Number of known words: %d",num_words);
  len = strlen(word_tuple->value->cstring);
  APP_LOG(APP_LOG_LEVEL_DEBUG,"length of words buffer: %d",len);

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



void make_numbers() {
  for(int tens=2;tens<6; tens++) {
    for(int digit=0;digit<10;digit++) {
      if(digit == 0) {
        snprintf(french_numbers[(tens)*10+digit],16,"%s",french_tens[tens-2]);
      }
      else if(digit == 1) {
        snprintf(french_numbers[(tens)*10+digit],16,"%s et %s",french_tens[tens-2],french_numbers[digit]);
      }
      else {
        snprintf(french_numbers[(tens)*10+digit],16,"%s-%s",french_tens[tens-2],french_numbers[digit]);
      }
    }
  }
}


static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed) {
  text_layer_set_text(text_layers[1],french_numbers[tick_time->tm_hour]);
  text_layer_set_text(text_layers[3],french_numbers[tick_time->tm_min]);

  update_word(mktime(tick_time));
}

void handle_init(void) {
  make_numbers();

	// Create a window and text layer
	window = window_create();
	//text_layer = text_layer_create(GRect(0, 50, 144, 154));
  
  time_layer = layer_create(GRect(0, 10, 144, 154));

  text_layers[0] = text_layer_create(GRect(0, 0, 144, 154));
  text_layer_set_text(text_layers[0], "Il est");
  text_layer_set_font(text_layers[0], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

  text_layers[1] = text_layer_create(GRect(0, 18, 144, 154));
  text_layer_set_font(text_layers[1], fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  
  text_layers[2] = text_layer_create(GRect(0, 50, 144, 154));
  text_layer_set_font(text_layers[2], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(text_layers[2], "heurs");
  
  text_layers[3] = text_layer_create(GRect(0, 70, 144, 154));
  text_layer_set_font(text_layers[3], fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  
  text_layers[4] = text_layer_create(GRect(0, 120, 144, 154));
  text_layer_set_font(text_layers[4], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  
  for(int i=0;i<5; i++) {
	  text_layer_set_text_alignment(text_layers[i], GTextAlignmentCenter);
    layer_add_child(time_layer, text_layer_get_layer(text_layers[i]));
  }
  layer_add_child(window_get_root_layer(window), time_layer);

	// Push the window
	window_stack_push(window, true);
	
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  
  
   // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}


void handle_deinit(void) {
	// Destroy the text layer
	//text_layer_destroy(text_layer);
	for(int i=0;i<4;i++) {
    text_layer_destroy(text_layers[i]);
  }
  
  if(words_buffer) free(words_buffer);
  if(words) free(words);
  
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
