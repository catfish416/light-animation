#include "tsp92662_defines.h"


#define spc(level) PrintSpace(level)

void PrintSpace(int level);

int send_led_uart_data(FrameInit command, uint8_t lmm, uint8_t led_index, uint16_t* data_ptr);

