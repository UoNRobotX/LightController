#ifndef LIGHT_H
#define LIGHT_H

#ifdef __cplusplus
extern "C" {
#endif

void light_init(void);
void cmd_light(int argc, char *argv[]);
void light_red();
void light_yellow();
void light_green();

#ifdef __cplusplus
}
#endif

#endif
