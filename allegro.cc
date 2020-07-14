/* This example displays a picture on the screen, with support for
 * command-line parameters, multi-screen, screen-orientation and
 * zooming.
 */
#include <stdio.h>
#include <stdlib.h>

#define ALLEGRO_NO_MAGIC_MAIN
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_image.h"
#include "allegro5/events.h"

#include "v8.h"
#include <libplatform/libplatform.h>

#include <v8pp/class.hpp>
#include <v8pp/module.hpp>
#include <v8pp/context.hpp>
#include <v8pp/version.hpp>
#include <v8pp/factory.hpp>

using namespace v8;

static bool w_al_init() {
    return al_init();
}

#ifdef __APPLE__
extern "C" {
void _al_pump_events(); 
int _al_osx_run_main(int argc, char **argv, int (*real_main)(int, char **), bool main_loop);
};

static int dead_loop(int argc, char *argv[]) {
    for(;;) {}
}

static int w_al_exec() {

    return _al_osx_run_main(0, NULL, dead_loop, false);
}
#endif

#define ALLEGRO_STRUCT(T)\
    template<>\
    struct v8pp::factory<T, v8pp::raw_ptr_traits>\
    {\
        static T* create(v8::Isolate*, int) { return nullptr; }\
        static void destroy(v8::Isolate*, T*) {}\
    };

class Event {
public:
    Event() {}

    ALLEGRO_EVENT_TYPE type() const { return _event.type; }

    const ALLEGRO_ANY_EVENT *any() const { return &_event.any; }
    const ALLEGRO_DISPLAY_EVENT *display() const { return &_event.display; }
    const ALLEGRO_JOYSTICK_EVENT *joystick() const { return &_event.joystick; }
    const ALLEGRO_KEYBOARD_EVENT *keyboard() const { return &_event.keyboard; }
    const ALLEGRO_MOUSE_EVENT *mouse() const { return &_event.mouse; }
    const ALLEGRO_TIMER_EVENT *timer() const { return &_event.timer; }
    const ALLEGRO_TOUCH_EVENT *touch() const { return &_event.touch; }
    const ALLEGRO_USER_EVENT *user() const { return &_event.user; }

    ALLEGRO_EVENT *event() { return &_event; }

private:
    ALLEGRO_EVENT _event;
};

ALLEGRO_STRUCT(ALLEGRO_TIMER);
ALLEGRO_STRUCT(ALLEGRO_DISPLAY);
ALLEGRO_STRUCT(ALLEGRO_BITMAP);
ALLEGRO_STRUCT(ALLEGRO_COLOR);
ALLEGRO_STRUCT(ALLEGRO_EVENT_QUEUE);
ALLEGRO_STRUCT(ALLEGRO_EVENT_SOURCE);
//ALLEGRO_STRUCT(ALLEGRO_EVENT);

static void w_al_wait_for_event(ALLEGRO_EVENT_QUEUE *queue, Event *event) {

    al_wait_for_event(queue, event->event());
}

void register_allegro(Local<Object> exports)
{
    Isolate* isolate = exports->GetIsolate();

    v8pp::module allegro(isolate);

    v8pp::class_<ALLEGRO_TIMER> w_ALLEGRO_TIMER(isolate);
    v8pp::class_<ALLEGRO_DISPLAY> w_ALLEGRO_DISPLAY(isolate);
    v8pp::class_<ALLEGRO_COLOR> w_ALLEGRO_COLOR(isolate);
    v8pp::class_<ALLEGRO_BITMAP> w_ALLEGRO_BITMAP(isolate);
    v8pp::class_<ALLEGRO_EVENT_QUEUE> w_ALLEGRO_EVENT_QUEUE(isolate);
    v8pp::class_<ALLEGRO_EVENT_SOURCE> w_ALLEGRO_EVENT_SOURCE(isolate);
    v8pp::class_<Event> w_EVENT(isolate);

    v8pp::class_<ALLEGRO_ANY_EVENT> w_ALLEGRO_ANY_EVENT(isolate);
    w_ALLEGRO_ANY_EVENT
        .set("timestamp", &ALLEGRO_ANY_EVENT::timestamp)
    ;

    v8pp::class_<ALLEGRO_DISPLAY_EVENT> w_ALLEGRO_DISPLAY_EVENT(isolate);
    w_ALLEGRO_DISPLAY_EVENT
        .set("timestamp", &ALLEGRO_DISPLAY_EVENT::timestamp)
        .set("x", &ALLEGRO_DISPLAY_EVENT::x)
        .set("y", &ALLEGRO_DISPLAY_EVENT::y)
        .set("width", &ALLEGRO_DISPLAY_EVENT::width)
        .set("height", &ALLEGRO_DISPLAY_EVENT::height)
        .set("orientation", &ALLEGRO_DISPLAY_EVENT::orientation)
    ;

    v8pp::class_<ALLEGRO_JOYSTICK_EVENT> w_ALLEGRO_JOYSTICK_EVENT(isolate);
    w_ALLEGRO_JOYSTICK_EVENT
        .set("timestamp", &ALLEGRO_JOYSTICK_EVENT::timestamp)
        .set("id", &ALLEGRO_JOYSTICK_EVENT::id)
        .set("stick", &ALLEGRO_JOYSTICK_EVENT::stick)
        .set("axis", &ALLEGRO_JOYSTICK_EVENT::axis)
        .set("pos", &ALLEGRO_JOYSTICK_EVENT::pos)
        .set("button", &ALLEGRO_JOYSTICK_EVENT::button)
    ;

    v8pp::class_<ALLEGRO_KEYBOARD_EVENT> w_ALLEGRO_KEYBOARD_EVENT(isolate);
    w_ALLEGRO_KEYBOARD_EVENT
        .set("timestamp", &ALLEGRO_KEYBOARD_EVENT::timestamp)
        .set("display", &ALLEGRO_KEYBOARD_EVENT::display)
        .set("keycode", &ALLEGRO_KEYBOARD_EVENT::keycode)
        .set("unichar", &ALLEGRO_KEYBOARD_EVENT::unichar)
        .set("modifiers", &ALLEGRO_KEYBOARD_EVENT::modifiers)
        .set("repeat", &ALLEGRO_KEYBOARD_EVENT::repeat)
    ;

    v8pp::class_<ALLEGRO_MOUSE_EVENT> w_ALLEGRO_MOUSE_EVENT(isolate);
    w_ALLEGRO_MOUSE_EVENT
        .set("timestamp", &ALLEGRO_MOUSE_EVENT::timestamp)
        .set("x", &ALLEGRO_MOUSE_EVENT::x)
        .set("y", &ALLEGRO_MOUSE_EVENT::y)
        .set("z", &ALLEGRO_MOUSE_EVENT::z)
        .set("w", &ALLEGRO_MOUSE_EVENT::w)
        .set("dx", &ALLEGRO_MOUSE_EVENT::dx)
        .set("dy", &ALLEGRO_MOUSE_EVENT::dy)
        .set("dz", &ALLEGRO_MOUSE_EVENT::dz)
        .set("dw", &ALLEGRO_MOUSE_EVENT::dw)
        .set("button", &ALLEGRO_MOUSE_EVENT::button)
        .set("pressure", &ALLEGRO_MOUSE_EVENT::pressure)
    ;

    v8pp::class_<ALLEGRO_TIMER_EVENT> w_ALLEGRO_TIMER_EVENT(isolate);
    w_ALLEGRO_TIMER_EVENT
        .set("timestamp", &ALLEGRO_TIMER_EVENT::timestamp)
        .set("count", &ALLEGRO_TIMER_EVENT::count)
        .set("error", &ALLEGRO_TIMER_EVENT::error)
    ;

    v8pp::class_<ALLEGRO_TOUCH_EVENT> w_ALLEGRO_TOUCH_EVENT(isolate);
    w_ALLEGRO_TOUCH_EVENT
        .set("timestamp", &ALLEGRO_TOUCH_EVENT::timestamp)
        .set("display", &ALLEGRO_TOUCH_EVENT::display)
        .set("id", &ALLEGRO_TOUCH_EVENT::id)
        .set("x", &ALLEGRO_TOUCH_EVENT::x)
        .set("y", &ALLEGRO_TOUCH_EVENT::y)
        .set("dx", &ALLEGRO_TOUCH_EVENT::dx)
        .set("dy", &ALLEGRO_TOUCH_EVENT::dy)
        .set("primary", &ALLEGRO_TOUCH_EVENT::primary)
    ;

    v8pp::class_<ALLEGRO_USER_EVENT> w_ALLEGRO_USER_EVENT(isolate);
    w_ALLEGRO_USER_EVENT
        .set("timestamp", &ALLEGRO_USER_EVENT::timestamp)
    ;

    w_EVENT
        .ctor<>()
        .set("type", &Event::type)
        .set("any", &Event::any)
        .set("display", &Event::display)
        .set("joystick", &Event::joystick)
        .set("keyboard", &Event::keyboard)
        .set("mouse", &Event::mouse)
        .set("timer", &Event::timer)
        .set("touch", &Event::touch)
        .set("user", &Event::user)
    ;

	allegro
        .set("ALLEGRO_TIMER", w_ALLEGRO_TIMER)
        .set("ALLEGRO_DISPLAY", w_ALLEGRO_DISPLAY)
        .set("ALLEGRO_COLOR", w_ALLEGRO_COLOR)
        .set("ALLEGRO_BITMAP", w_ALLEGRO_BITMAP)
        .set("ALLEGRO_EVENT_QUEUE", w_ALLEGRO_EVENT_QUEUE)
        .set("ALLEGRO_EVENT", w_EVENT)
        .set("ALLEGRO_EVENT_SOURCE", w_ALLEGRO_EVENT_SOURCE)

        .set("ALLEGRO_ANY_EVENT", w_ALLEGRO_ANY_EVENT)
        .set("ALLEGRO_DISPLAY_EVENT", w_ALLEGRO_DISPLAY_EVENT)
        .set("ALLEGRO_JOYSTICK_EVENT", w_ALLEGRO_JOYSTICK_EVENT)
        .set("ALLEGRO_KEYBOARD_EVENT", w_ALLEGRO_KEYBOARD_EVENT)
        .set("ALLEGRO_MOUSE_EVENT", w_ALLEGRO_MOUSE_EVENT)
        .set("ALLEGRO_TIMER_EVENT", w_ALLEGRO_TIMER_EVENT)
        .set("ALLEGRO_TOUCH_EVENT", w_ALLEGRO_TOUCH_EVENT)
        .set("ALLEGRO_USER_EVENT", w_ALLEGRO_USER_EVENT)

        .set("init", w_al_init)
#ifdef __APPLE__
        .set("exec", w_al_exec)
        .set("pump", _al_pump_events)
#endif
        .set("uninstall_system", al_uninstall_system)

        .set("install_mouse", al_install_mouse)
        .set("install_keyboard", al_install_keyboard)

        .set("init_font_addon", al_init_font_addon)

        .set("init_image_addon", al_init_image_addon)
        .set("load_bitmap", al_load_bitmap)
        .set("destroy_bitmap", al_destroy_bitmap)
        .set("get_bitmap_width", al_get_bitmap_width)

        .set("create_display", al_create_display)
        .set("set_window_title", al_set_window_title)
        .set("get_display_width", al_get_display_width)
        .set("clear_to_color", al_clear_to_color)
        .set("map_rgb_f", al_map_rgb_f)
        .set("draw_bitmap", al_draw_bitmap)
        .set("draw_scaled_rotated_bitmap", al_draw_scaled_rotated_bitmap)
        .set("flip_display", al_flip_display)

        .set("create_timer", al_create_timer)
        .set("start_timer", al_start_timer)
        .set("get_time", al_get_time)

        .set("create_event_queue", al_create_event_queue)
        .set("register_event_source", al_register_event_source)
        .set("get_keyboard_event_source", al_get_keyboard_event_source)
        .set("get_display_event_source", al_get_display_event_source)
        .set("get_timer_event_source", al_get_timer_event_source)
        .set("wait_for_event", w_al_wait_for_event)
        .set("is_event_queue_empty", al_is_event_queue_empty)
	;

    v8::Maybe<bool> ret = exports->SetPrototype(isolate->GetCurrentContext(), allegro.new_instance());
    ret.FromMaybe(false);
}
