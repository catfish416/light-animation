#ifdef __cplusplus
extern "C" {
#endif

#include "hal_uart.h"


#define RD_LEN  11

int main(int argc, char **argv) 
{
    int serial_fd = init_uart();
    if (serial_fd < 0)
    {
        printf("init serial failed!\n");
        return -1;
    }
  
    char buf_w[] = "hello world"; 
    char buf_r[RD_LEN] ; 
    memset(buf_r, 0, sizeof(char)*RD_LEN); 

    uart_send(serial_fd, buf_w, strlen(buf_w));
    //printf("\n");
/*
    while(1)
    {
        uart_send(serial_fd, buf, strlen(buf)); 
        printf("\n"); 
        sleep(1);
        //if(uart_recv(serial_fd, buf1, 11) > 0)
        uart_recv(serial_fd, buf_r, RD_LEN);
        printf("receive: %s\n", buf_r);
        memset(buf1, 0, sizeof(char)*RD_LEN); 
    }
*/
    close(serial_fd); 
    return 0; 
}


#ifdef __cplusplus
}
#endif /* end of __cplusplus */
