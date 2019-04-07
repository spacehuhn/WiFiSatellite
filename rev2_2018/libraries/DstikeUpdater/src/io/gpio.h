#ifndef gpio_h
#define gpio_h

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

void init_buttons(int up, int down, int select);
bool pressed(int pin);
bool pressed_any(int up, int down, int select);

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef gpio_h */