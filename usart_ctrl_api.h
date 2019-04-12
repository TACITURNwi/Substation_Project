#ifndef USART_CTRL_API_H
#define USART_CTRL_API_H

#ifdef __cplusplus

#define UART1_485_LOW 410
#define UART1_485_HIGH 411

#define UART2_485_LOW 420
#define UART2_485_HIGH 421

//#define HOLDER_SERIALPORT_NUMBER_1 "COM9"
#define HOLDER_SERIALPORT_NUMBER_1 "/dev/ttySAC1"

//#define LASER_SERIALPORT_NUMBER_2 "COM6"
#define LASER_SERIALPORT_NUMBER_2 "/dev/ttySAC2"

//#define GYROSCOPE_SERIALPORT_NUMBER_4 "COM9"
#define GYROSCOPE_SERIALPORT_NUMBER_4 "/dev/ttySAC4"


//#define IPADDRESSPATH "C:/Users/14017/Desktop/Substation_Project/Holder_Laser_20190411/ipadress.txt"
#define IPADDRESSPATH "/home/charlie/file_from_pc/ip_address"


extern "C" {
#endif
    extern int gpio_open();
    extern int gpio_ctrl(unsigned int cmd);
    extern int gpio_close();
    extern int fd;
#ifdef __cplusplus
}
#endif

#endif // USART_CTRL_API_H
