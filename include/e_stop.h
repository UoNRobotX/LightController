#ifndef E_STOP_H
#define E_STOP_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void e_stop_init(void);
bool e_stop_activated(void);

#ifdef __cplusplus
}
#endif

#endif
