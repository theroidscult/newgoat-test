#ifndef __CEREAL_H__
#define __CEREAL_H__

#include <stdbool.h>
#include <stdint.h>

bool cereal_init_port(int port_num);

void cereal_write(int port_num, uint8_t data);
uint8_t cereal_read(int port_num);

#endif // __CEREAL_H__