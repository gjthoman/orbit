#include "simple_analog.h"

#include "pebble.h"

static Window *s_window;
static Layer *s_simple_bg_layer, *s_hands_layer, *s_date_layer;
static TextLayer *s_day_label, *s_num_label;
static int s_digits[12];
static char s_num_buffer[4], s_day_buffer[6];

static GColor background_color;
static GColor hour_1_color;
static GColor hour_2_color;
static GColor min_1_color;
static GColor min_2_color;
static GColor date_color;
static GColor batt_back_color;
static GColor batt_fore_color;

static int battery_visibility;
static int date_visibility;
/*
0: Always
1: Never
2: Shake
*/

static bool show_battery;

typedef enum {
  AppKeyBackgroundColor = 0,
  AppKeyHour1Color = 1,
  AppKeyHour2Color = 2,
  AppKeyMin1Color = 3,
  AppKeyMin2Color = 4,
  AppKeyBattBackColor = 5,
  AppKeyBattForeColor = 6,
  AppKeyShowBatt = 7,
  AppKeyShowDate = 8,
  AppKeyDateColor = 9
} AppKey;

static void bg_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, background_color);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

static GPoint getPoint(int32_t angle) {
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  uint16_t radius = PBL_IF_ROUND_ELSE(90, 70);
  
  return GPoint((int16_t)(sin_lookup(angle) * (int32_t)radius / TRIG_MAX_RATIO) + center.x, (int16_t)(-cos_lookup(angle) * (int32_t)radius / TRIG_MAX_RATIO) + center.y);
}

static void drawBatt(Layer *layer, GContext *ctx) {
  
    GRect bounds = layer_get_bounds(layer);
    BatteryChargeState battState = battery_state_service_peek();
    
    int batt_perc = battState.charge_percent;
    
    float height = bounds.size.h * batt_perc/100;
    float width = bounds.size.w * batt_perc/100;
    
    float h_diff = (bounds.size.h - height)/2;
    float w_diff = (bounds.size.w - width)/2;
  
    int offset = (bounds.size.h - bounds.size.w)/2;
    
    GPoint y_a = GPoint(bounds.size.w/2, h_diff+offset);
    GPoint y_b = GPoint(bounds.size.w/2, bounds.size.h - h_diff-offset);
    GPoint x_a = GPoint(w_diff, bounds.size.h/2);
    GPoint x_b = GPoint(bounds.size.w - w_diff, bounds.size.h/2);
    
    graphics_context_set_stroke_width(ctx, 4);
    graphics_context_set_stroke_color(ctx, batt_back_color);
    graphics_context_set_fill_color(ctx, batt_back_color);
    
    graphics_draw_line(ctx, GPoint(bounds.size.w/2, 0+offset), GPoint(bounds.size.w/2, bounds.size.h-offset));
    graphics_draw_line(ctx, GPoint(0, bounds.size.h/2), GPoint(bounds.size.w, bounds.size.h/2));
    
    graphics_context_set_stroke_width(ctx, 4);
    graphics_context_set_stroke_color(ctx, batt_fore_color);
    graphics_context_set_fill_color(ctx, batt_fore_color);
    
    graphics_draw_line(ctx, y_a, y_b);
    graphics_draw_line(ctx, x_a, x_b);
  
}

static void draw_segment(Layer *layer, GContext *ctx,  int position, int segment) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_stroke_width(ctx, 5);

  uint16_t inset_thickness = PBL_IF_ROUND_ELSE(7, 6); 
  
  int32_t a_arc;
  int32_t b_arc;
  int32_t c_arc;
  int32_t seg_arc;
  int32_t seg_end_arc;
  int32_t a_angle;  
  int32_t b_angle;
  int32_t c_angle;

  GPoint b;
  GPoint a;
  GPoint c;
  
  switch(position) {
    case 1:
      a_arc = DEG_TO_TRIGANGLE(-5);
      b_arc = DEG_TO_TRIGANGLE(-60);
      c_arc = DEG_TO_TRIGANGLE(-85);
      seg_arc = DEG_TO_TRIGANGLE(-31);
      seg_end_arc = DEG_TO_TRIGANGLE(-30);
      a_angle = TRIG_MAX_ANGLE * 59/ 60;  
      b_angle = TRIG_MAX_ANGLE * 50/ 60;
      c_angle = TRIG_MAX_ANGLE * 46/ 60;
  
      b = getPoint(b_angle);
      a = getPoint(a_angle);
      c = getPoint(c_angle);
    
      switch(segment) {
        case 1:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, b_arc, seg_end_arc);
        break; 
        case 2:
          graphics_draw_line(ctx, a, GPoint(a.x, b.y));
        break; 
        case 3:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, c_arc, b_arc);
        break;
        case 4:
          graphics_draw_line(ctx, GPoint(a.x, b.y), GPoint(a.x, c.y));
        break;
        case 5:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, seg_arc, a_arc);
        break;
        case 6:
          graphics_draw_line(ctx, GPoint(a.x, b.y), b);
        break;
        case 7:
          graphics_draw_line(ctx, GPoint(a.x, c.y), c);
        break;
    
      }
    break;
    
    case 2:
      a_arc = DEG_TO_TRIGANGLE(5);
      b_arc = DEG_TO_TRIGANGLE(60);
      c_arc = DEG_TO_TRIGANGLE(85);
      seg_arc = DEG_TO_TRIGANGLE(31);
      seg_end_arc = DEG_TO_TRIGANGLE(30);
      a_angle = TRIG_MAX_ANGLE * 1/ 60;  
      b_angle = TRIG_MAX_ANGLE * 10/ 60;
      c_angle = TRIG_MAX_ANGLE * 14/ 60;

      b = getPoint(b_angle);
      a = getPoint(a_angle);
      c = getPoint(c_angle);
    
      switch(segment) {
        case 1:
          graphics_draw_line(ctx, a, GPoint(a.x, b.y));
        break; 
        case 2:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, seg_end_arc, b_arc);
        break; 
        case 3:
          graphics_draw_line(ctx, GPoint(a.x, b.y), GPoint(a.x, c.y));
        break;
        case 4:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, b_arc, c_arc);
        break;
        case 5:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, a_arc, seg_arc);
        break;
        case 6:
          graphics_draw_line(ctx, GPoint(a.x, b.y), b);
        break;
        case 7:
          graphics_draw_line(ctx, GPoint(a.x, c.y), c);
        break;
    
      }
    break;
     case 3:
      a_arc = DEG_TO_TRIGANGLE(265);
      b_arc = DEG_TO_TRIGANGLE(240);
      c_arc = DEG_TO_TRIGANGLE(185);
      seg_arc = DEG_TO_TRIGANGLE(210);
      seg_end_arc = DEG_TO_TRIGANGLE(209);
      a_angle = TRIG_MAX_ANGLE * 44/ 60;  
      b_angle = TRIG_MAX_ANGLE * 40/ 60;
      c_angle = TRIG_MAX_ANGLE * 31/ 60;

      b = getPoint(b_angle);
      a = getPoint(a_angle);
      c = getPoint(c_angle);
    
      switch(segment) {
        case 1:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, b_arc, a_arc);
        break; 
        case 2:
          graphics_draw_line(ctx, GPoint(c.x, a.y), GPoint(c.x, b.y));
        break; 
        case 3:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, seg_end_arc, b_arc);
        break;
        case 4:
          graphics_draw_line(ctx, GPoint(c.x, b.y), c);
        break;
        case 5:
          graphics_draw_line(ctx, GPoint(c.x, a.y), a); 
        break;
        case 6:
          graphics_draw_line(ctx, GPoint(c.x, b.y), b);
        break;
        case 7:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, c_arc, seg_arc);
        break;
    
      }
    break;
    case 4:
      a_arc = DEG_TO_TRIGANGLE(95);
      b_arc = DEG_TO_TRIGANGLE(120);
      c_arc = DEG_TO_TRIGANGLE(175);
      seg_arc = DEG_TO_TRIGANGLE(150);
      seg_end_arc = DEG_TO_TRIGANGLE(151);
      a_angle = TRIG_MAX_ANGLE * 16/ 60;  
      b_angle = TRIG_MAX_ANGLE * 20/ 60;
      c_angle = TRIG_MAX_ANGLE * 29/ 60;

      b = getPoint(b_angle);
      a = getPoint(a_angle);
      c = getPoint(c_angle);
    
      switch(segment) {
        case 1:
          graphics_draw_line(ctx, GPoint(c.x, a.y), GPoint(c.x, b.y));
        break; 
        case 2:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, a_arc, b_arc);
        break; 
        case 3:
          graphics_draw_line(ctx, GPoint(c.x, b.y), c);
        break;
        case 4:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, b_arc, seg_end_arc);
        break;
        case 5:
          graphics_draw_line(ctx, GPoint(c.x, a.y), a); 
        break;
        case 6:
          graphics_draw_line(ctx, GPoint(c.x, b.y), b);
        break;
        case 7:
          graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, inset_thickness, seg_arc, c_arc);
        break;
    
      }
    break;
  }
}

char *upcase(char *str) {
  for (int i = 0; str[i] != 0; i++) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      str[i] -= 0x20;
    }
  }
  
  return str;
}

static void hands_update_proc(Layer *layer, GContext *ctx) {  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  if( date_visibility != 1) {
    if (date_visibility == 0 || show_battery) {
      text_layer_set_text_color(s_day_label, date_color);
      text_layer_set_text_color(s_num_label, date_color);
      
      strftime(s_day_buffer, sizeof(s_day_buffer), "%b", t);
      text_layer_set_text(s_day_label, upcase(s_day_buffer));
    
      strftime(s_num_buffer, sizeof(s_num_buffer), "%d", t);
      text_layer_set_text(s_num_label, s_num_buffer);
    
    } else {
      text_layer_set_text_color(s_day_label, GColorClear);
      text_layer_set_text_color(s_num_label, GColorClear);
    }
  } else {
    text_layer_set_text_color(s_day_label, GColorClear);
    text_layer_set_text_color(s_num_label, GColorClear);
  }

  int mins = t->tm_min;
  int hours = t->tm_hour;
  
  if (!clock_is_24h_style() ){
    if (hours > 12){
      hours = hours - 12;
    }
  } 
  
  if(mins < 10) {
    s_digits[2] = 0;
  } else {
    s_digits[2] = mins / 10;
  }
  s_digits[3] = mins % 10;

  if(hours < 10) {
    s_digits[0] = 0;
  } else {
    s_digits[0] = hours / 10;
  }
  s_digits[1] = hours % 10;
  
  if (battery_visibility != 1) {
    if (battery_visibility == 0 || show_battery) {
      drawBatt(layer, ctx);
    }  
  }
  
  graphics_context_set_stroke_color(ctx, hour_1_color);
  graphics_context_set_fill_color(ctx, hour_1_color);
  for (int i = 0; i < NUM_POINTS[s_digits[0]]; i ++ ) {
    draw_segment(layer, ctx, 1, NUM_PATHS[s_digits[0]][i]);
  }
  
  graphics_context_set_stroke_color(ctx, hour_2_color);
  graphics_context_set_fill_color(ctx, hour_2_color);
  for (int i = 0; i < NUM_POINTS[s_digits[1]]; i ++ ) {
    draw_segment(layer, ctx, 2, NUM_PATHS[s_digits[1]][i]);
  }
  
  graphics_context_set_stroke_color(ctx, min_1_color);
  graphics_context_set_fill_color(ctx, min_1_color);
  for (int i = 0; i < NUM_POINTS[s_digits[2]]; i ++ ) {
    draw_segment(layer, ctx, 3, NUM_PATHS[s_digits[2]][i]);
  }
  
  graphics_context_set_stroke_color(ctx, min_2_color);
  graphics_context_set_fill_color(ctx, min_2_color);
  for (int i = 0; i < NUM_POINTS[s_digits[3]]; i ++ ) {
    draw_segment(layer, ctx, 4, NUM_PATHS[s_digits[3]][i]);
  }
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  
  Tuple *bg_color_t = dict_find(iter, AppKeyBackgroundColor);
  if(bg_color_t) {
    background_color = GColorFromHEX(bg_color_t->value->int32);
    persist_write_int(AppKeyBackgroundColor, bg_color_t->value->int32);
  }
  
  Tuple *hour_1_color_t = dict_find(iter, AppKeyHour1Color);
  if(hour_1_color_t) {
    hour_1_color = GColorFromHEX(hour_1_color_t->value->int32);
    persist_write_int(AppKeyHour1Color, hour_1_color_t->value->int32);
  }
  Tuple *hour_2_color_t = dict_find(iter, AppKeyHour2Color);
  if(hour_2_color_t) {
    hour_2_color = GColorFromHEX(hour_2_color_t->value->int32);
    persist_write_int(AppKeyHour2Color, hour_2_color_t->value->int32);
  }
  Tuple *min_1_color_t = dict_find(iter, AppKeyMin1Color);
  if(min_1_color_t) {
    min_1_color = GColorFromHEX(min_1_color_t->value->int32);
    persist_write_int(AppKeyMin1Color, min_1_color_t->value->int32);
  }
  Tuple *min_2_color_t = dict_find(iter, AppKeyMin2Color);
  if(min_2_color_t) {
    min_2_color = GColorFromHEX(min_2_color_t->value->int32);
    persist_write_int(AppKeyMin2Color, min_2_color_t->value->int32);
  }

  Tuple *date_color_t = dict_find(iter, AppKeyDateColor);
  if(date_color_t) {
    date_color = GColorFromHEX(date_color_t->value->int32);
    persist_write_int(AppKeyDateColor, date_color_t->value->int32);
  }
  
  Tuple *batt_fore_color_t = dict_find(iter, AppKeyBattForeColor);
  if(batt_fore_color_t) {
    batt_fore_color = GColorFromHEX(batt_fore_color_t->value->int32);
    persist_write_int(AppKeyBattForeColor, batt_fore_color_t->value->int32);
  }
  
  Tuple *batt_back_color_t = dict_find(iter, AppKeyBattBackColor);
  if(batt_back_color_t) {
    batt_back_color = GColorFromHEX(batt_back_color_t->value->int32);
    persist_write_int(AppKeyBattBackColor, batt_back_color_t->value->int32);
  }
  Tuple *show_batt_t = dict_find(iter, AppKeyShowBatt);
  if(show_batt_t) {
    battery_visibility = show_batt_t->value->int32;
    persist_write_int(AppKeyShowBatt, show_batt_t->value->int32);
  }
  
  Tuple *show_date_t = dict_find(iter, AppKeyShowDate);
  if(show_date_t) {
    date_visibility = show_date_t->value->int32;
    persist_write_int(AppKeyShowDate, show_date_t->value->int32);
  }
  
  layer_mark_dirty(window_get_root_layer(s_window));
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(s_window));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_simple_bg_layer = layer_create(bounds);
  layer_set_update_proc(s_simple_bg_layer, bg_update_proc);
  layer_add_child(window_layer, s_simple_bg_layer);

  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);
  
  s_date_layer = layer_create(bounds);

  layer_add_child(window_layer, s_date_layer);

  s_day_label = text_layer_create(PBL_IF_ROUND_ELSE(
    GRect(112, 105, 27, 24),
    GRect(85, 93, 27, 24)));

  text_layer_set_text(s_day_label, s_day_buffer);
  text_layer_set_background_color(s_day_label, GColorClear);
  text_layer_set_text_color(s_day_label, GColorClear);
  text_layer_set_font(s_day_label, fonts_get_system_font(FONT_KEY_GOTHIC_18));

  layer_add_child(s_date_layer, text_layer_get_layer(s_day_label));

  s_num_label = text_layer_create(PBL_IF_ROUND_ELSE(
    GRect(139, 105, 18, 24),
    GRect(112, 93, 18, 24)));
  text_layer_set_text(s_num_label, s_num_buffer);
  text_layer_set_background_color(s_num_label, GColorClear);
  text_layer_set_text_color(s_num_label, GColorClear);
  text_layer_set_font(s_num_label, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

  layer_add_child(s_date_layer, text_layer_get_layer(s_num_label));
}

static void window_unload(Window *window) {
  layer_destroy(s_simple_bg_layer);
  layer_destroy(s_hands_layer);
}
static void reset_shake() {
  show_battery = false;
  layer_mark_dirty(window_get_root_layer(s_window));
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  if (battery_visibility == 2){
    show_battery = true;
    
    layer_mark_dirty(window_get_root_layer(s_window));
    app_timer_register(8000, reset_shake, NULL);  
  }
}

static void handle_battery(BatteryChargeState charge_state) {
  layer_mark_dirty(window_get_root_layer(s_window));
}

static GColor readColor(AppKey app_key, GColor defaultColor) {
  if (persist_exists(app_key)) {
    return GColorFromHEX(persist_read_int(app_key));    
  } else {
    return defaultColor;
  }
}

static bool readBool(AppKey app_key, bool _bool) {
  if (persist_exists(app_key)) {
    return persist_read_bool(app_key);    
  } else {
    return _bool;
  }
}

static int readInt(AppKey app_key, int _int) {
  if (persist_exists(app_key)) {
    return persist_read_int(app_key);
  } else {
    return _int;
  }
}

static void init() {
  background_color = readColor(AppKeyBackgroundColor, GColorBlack);
  hour_1_color = readColor(AppKeyHour1Color, GColorGreen);  
  hour_2_color = readColor(AppKeyHour2Color, GColorGreen);  
  min_1_color = readColor(AppKeyMin1Color, GColorGreen);  
  min_2_color = readColor(AppKeyMin2Color, GColorGreen);  
  date_color = readColor(AppKeyDateColor, GColorWhite);  
  batt_back_color = readColor(AppKeyBattBackColor, GColorDarkGray);
  batt_fore_color = readColor(AppKeyBattForeColor, GColorLightGray);
  
  battery_visibility = readInt(AppKeyShowBatt, 2);
  date_visibility = readInt(AppKeyShowDate, 2);
  
  show_battery = false;
  
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  accel_tap_service_subscribe(tap_handler);
  battery_state_service_subscribe(handle_battery);
  
  app_message_register_inbox_received(inbox_received_handler);

  app_message_open(128, 128);
}

static void deinit() {
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
