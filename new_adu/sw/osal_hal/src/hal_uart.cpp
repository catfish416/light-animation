#ifdef __cplusplus
extern "C" {
#endif

#include "hal_uart.h"


#define DEVICE "/dev/ttyLF0" 

#define S_TIMEOUT 1

#define RAUD_115200 B115200
#define RAUD_500000 B500000

int serial_fd = -1;

//打开串口并初始化设置

/** 
*初始化串口函数
* return: fd 
*/
int init_uart(void) 
{

    if (serial_fd > 0) { // 串口已经打开，避免重复初始化
        tcflush(serial_fd, TCIOFLUSH);//溢出数据可以接收，但不读 
        return serial_fd;
    }

    
    serial_fd = open(DEVICE, O_RDWR | O_NOCTTY | O_NDELAY); 
    if (serial_fd < 0) { 
        perror("open"); 
        return -1; 
    } 
      
    //串口主要设置结构体termios <termios.h> 
    struct termios options; 
      
    /**1. tcgetattr函数用于获取与终端相关的参数。 
    *参数fd为终端的文件描述符，返回的结果保存在termios结构体中 
    */ 
    tcgetattr(serial_fd, &options); 
    /**2. 修改所获得的参数*/ 
    options.c_cflag |= (CLOCAL | CREAD);//设置控制模式状态，本地连接，接收使能 
    options.c_cflag &= ~CSIZE;//字符长度，设置数据位之前一定要屏掉这个位 
    options.c_cflag &= ~CRTSCTS;//无硬件流控 
    options.c_cflag |= CS8;//8位数据长度 
    options.c_cflag &= ~CSTOPB;//1位停止位 
    options.c_iflag |= IGNPAR;//无奇偶检验位 
    options.c_oflag = 0; //输出模式 
    options.c_lflag = 0; //不激活终端模式 

    tcflush(serial_fd, TCIOFLUSH);
    cfsetispeed(&options, RAUD_500000);//设置波特率 
    cfsetospeed(&options, RAUD_500000);
    
    /**3. 设置新属性，TCSANOW：所有改变立即生效*/ 
    
    tcsetattr(serial_fd, TCSANOW, &options);
    tcflush(serial_fd, TCIOFLUSH);//溢出数据可以接收，但不读 

    return serial_fd; 
} 
  
/** 
*串口发送数据 
*@fd:串口描述符 
*@data:待发送数据 
*@datalen:数据长度 
*/ 
unsigned int total_send = 0 ;
int uart_send(int fd, uint8_t *data, int datalen) 
{
    if (fd < 0)
    {
        printf("fd is %d\n", fd);
        return -1;
    }
/*
    for (int i = 0; i < datalen; i++)
    {
        printf("sent data[%d]: 0x%x\n", i, data[i]);
    }
*/

//    tcflush(fd, TCIOFLUSH);

    int len = 0;
    len = write(fd, data, datalen);//实际写入的长度 
    if(len == datalen) { 
        total_send += len ;
        //printf("total_send is %d\n",total_send); 
        return len; 
    } else { 
        //tcflush(fd, TCOFLUSH);//TCOFLUSH刷新写入的数据但不传送 
        return -1; 
    }
    tcflush(fd, TCIOFLUSH);  //数据发送后，返回消息不处理，丢弃

    return 0; 
} 
  
/** 
*串口接收数据 
*要求启动后，在pc端发送ascii文件 
*/
unsigned int total_length = 0 ; 
int uart_recv(int fd, uint8_t *data, int datalen) 
{ 
    int len=0, ret = 0; 
    fd_set fs_read; 
    struct timeval tv_timeout; 
      
    FD_ZERO(&fs_read); 
    FD_SET(fd, &fs_read); 

#ifdef S_TIMEOUT    
    tv_timeout.tv_sec = (10*20/115200+2); 
    tv_timeout.tv_usec = 0; 
    ret = select(fd+1, &fs_read, NULL, NULL, NULL);
#else
    ret = select(fd+1, &fs_read, NULL, NULL, tv_timeout);
#endif

    printf("ret = %d\n", ret); 
    //如果返回0，代表在描述符状态改变前已超过timeout时间,错误返回-1 
     
    if (FD_ISSET(fd, &fs_read)) { 
        len = read(fd, data, datalen); 
        total_length += len ;
        printf("total len = %d\n", total_length); 
        return len; 
    } else { 
        perror("select"); 
        return -1; 
    } 
      
    return 0; 
} 


#ifdef __cplusplus
}
#endif /* end of __cplusplus */
