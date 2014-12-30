#include <allegro5/allegro.h>
#include <OGRE/Ogre.h>

#ifndef __asteroids3d_common_h
#define __asteroids3d_common_h

#ifdef __cplusplus
extern "C" {
#endif

void abort_example(char const *format, ...);
void open_log(void);
void open_log_monospace(void);
void close_log(bool wait_for_user);
void log_printf(char const *format, ...);

#ifdef __cplusplus
}
#endif

#endif
