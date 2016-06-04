#include "simple_analog.h"

#include "pebble.h"

static Window *s_window;
static Layer *s_simple_bg_layer, *s_hands_layer;
static int s_digits[12];

static void bg_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, GColorWhite);
}

static GPoint getPoint(int32_t angle) {
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  uint16_t radius = PBL_IF_ROUND_ELSE(90, 70);
  
  return GPoint((int16_t)(sin_lookup(angle) * (int32_t)radius / TRIG_MAX_RATIO) + center.x, (int16_t)(-cos_lookup(angle) * (int32_t)radius / TRIG_MAX_RATIO) + center.y);
}

static void draw_segment(Layer *layer, GContext *ctx,  int position, int segment) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_stroke_width(ctx, 5);
  graphics_context_set_stroke_color(ctx, GColorGreen);
  graphics_context_set_fill_color(ctx, GColorGreen);

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

static void hands_update_proc(Layer *layer, GContext *ctx) {  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

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
  
  for (int i = 0; i < NUM_POINTS[s_digits[0]]; i ++ ) {
    draw_segment(layer, ctx, 1, NUM_PATHS[s_digits[0]][i]);
  }
  for (int i = 0; i < NUM_POINTS[s_digits[1]]; i ++ ) {
    draw_segment(layer, ctx, 2, NUM_PATHS[s_digits[1]][i]);
  }
  for (int i = 0; i < NUM_POINTS[s_digits[2]]; i ++ ) {
    draw_segment(layer, ctx, 3, NUM_PATHS[s_digits[2]][i]);
  }
  for (int i = 0; i < NUM_POINTS[s_digits[3]]; i ++ ) {
    draw_segment(layer, ctx, 4, NUM_PATHS[s_digits[3]][i]);
  }
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
}

static void window_unload(Window *window) {
  layer_destroy(s_simple_bg_layer);
  layer_destroy(s_hands_layer);
}

static void init() {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
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
