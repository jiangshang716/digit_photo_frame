#ifndef __KEY_BOARD__
#define __KEY_BOARD__

pthread_t start_key_controll_pthread();

void * key_controll_pthread_fun();

int change_global_keycode( uint16_t *dest , uint16_t source);
#endif
