#include <stdio.h>
#include <stdarg.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

ALLEGRO_TEXTLOG *textlog = NULL;

void abort_example(char const *format, ...) {
    char str[1024];
    va_list args;
    ALLEGRO_DISPLAY *display;

    va_start(args, format);
    vsnprintf(str, sizeof str, format, args);
    va_end(args);

//   if (al_init_native_dialog_addon()) {
    display = al_is_system_installed() ? al_get_current_display() : NULL;
    al_show_native_message_box(display, "Error", "Cannot run example", str, NULL, 0);
/*   }
   else {
      fprintf(stderr, "%s", str);
   } */
    exit(1);
}

void open_log(void) {
//   if (al_init_native_dialog_addon()) {
    textlog = al_open_native_text_log("Log", 0);
//   }
}

void open_log_monospace(void) {
//   if (al_init_native_dialog_addon()) {
    textlog = al_open_native_text_log("Log", ALLEGRO_TEXTLOG_MONOSPACE);
//   }
}

void close_log(bool wait_for_user) {
    if (textlog && wait_for_user) {
        ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
        al_register_event_source(queue, al_get_native_text_log_event_source(
                textlog));
        al_wait_for_event(queue, NULL);
        al_destroy_event_queue(queue);
    }

    al_close_native_text_log(textlog);
    textlog = NULL;
}

void log_printf(char const *format, ...) {
    char str[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(str, sizeof str, format, args);
    va_end(args);
    al_append_native_text_log(textlog, "%s", str);
}
