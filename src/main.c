#include <pebble.h>

Window *window;
TextLayer *first_layer, *second_layer, *third_layer, *title_layer;
char buffer[320];
const char *phase[] = {"New Moon",
                 "Waxing Crescent",
                 "First Quarter",
                 "Waxing Gibbous",
                 "Full Moon",
                 "Waning Gibbous",
                 "Third Quarter",
                 "Waning Crescent"};

// Returns a percentage from 0 - 99000 for the current phase of the moon (0 = New Moon)
int moonphase_x1000() {
  return ((((time(NULL) - 592542) % 2551443))/25.51443);
}

// Returns a percentage from 0 - 99 for the current phase of the moon (0 = New Moon)
int moonphase_percent() {
  return ((((time(NULL) - 592542) % 2551443) * 8)/2551443); // replace 100 to return 0-whatever
}

// Returns a number from 0 - 7 for the current phase of the moon
int moonphase() {
  //592542 seconds (Jan 8th 1970 8:35:42pm) = New Moon
  //2551443 seconds = 1 Synodic Month (rounding 2551442.861088 seconds)
  //159465 = 1/16 of 1 orbit (so a phase is +/- 1/16 from the midpoint of the phase)
  //return ((((time(NULL) + (2551443/16) - 592542) % 2551443)) / (2551443/8);
  
  //1388574900 seconds (Jan 1st 2014 11:15:00) = Recent New Moon
  return (((time(NULL) + (2551443/16) - 1388574900) % 2551443)) / (2551443/8);
}

static TextLayer* init_text_layer(GRect location, GColor colour, GColor background, const char *res_id, GTextAlignment alignment)
{
	TextLayer *layer = text_layer_create(location);
	text_layer_set_text_color(layer, colour);
	text_layer_set_background_color(layer, background);
	text_layer_set_font(layer, fonts_get_system_font(res_id));
	text_layer_set_text_alignment(layer, alignment);
  text_layer_set_overflow_mode(layer, GTextOverflowModeFill);
	return layer;
}

void window_load(Window *window)
{
  ResHandle font_handle = resource_get_handle(RESOURCE_ID_PHASE_16);
  Layer *window_layer = window_get_root_layer(window);
  
	first_layer = init_text_layer(GRect(0, 0, 144, 168), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
	text_layer_set_text(first_layer, "abcdefghijklabcdefghijkl");
  text_layer_set_font(first_layer, fonts_load_custom_font(font_handle));
  text_layer_set_text(first_layer, "0 D G I N R T W");
  layer_add_child(window_layer, text_layer_get_layer(first_layer));

  second_layer = init_text_layer(GRect(5, 30, 190, 42), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(second_layer));
	//text_layer_set_text(second_layer, "12345678901234567890123456789012345678901234567890");
  text_layer_set_text(second_layer, phase[moonphase()]);

  third_layer = init_text_layer(GRect(5, 60, 134, 21), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
	//snprintf(buffer, sizeof("Phase: XX.XXX"), "Phase: %d%%", moonphase());
	//int value = moonphase_x1000(); snprintf(buffer, sizeof("Phase: XX.xxx"), "Phase: %d.%d", value/1000, value - (((int) (value/1000)) * 1000));
	//int value = moonphase();
  snprintf(buffer, sizeof(buffer), "Phase: %d %d", moonphase(), moonphase_percent());
  text_layer_set_text(third_layer, (char*) &buffer);
  
  //text_layer_set_text(third_layer, "12345 12345 12345 12345 12345 12345 12345");
	layer_add_child(window_layer, text_layer_get_layer(third_layer));
}

void window_unload(Window *window)
{	
	text_layer_destroy(first_layer);
	text_layer_destroy(second_layer);
	text_layer_destroy(third_layer);
}

void handle_init(void) {
	window = window_create();
	WindowHandlers handlers = {
		.load = window_load,
		.unload = window_unload
	};
  window_set_fullscreen(window, true);
	window_set_window_handlers(window, handlers);
  window_stack_push(window, true);
}

void handle_deinit(void) {
  window_destroy(window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
