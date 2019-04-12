#ifndef USART_CTRL_API_H
#define USART_CTRL_API_H

#ifdef __cplusplus

#define UART1_485_LOW 410
#define UART1_485_HIGH 411

#define HOLDER_SERIALPORT_NUMBER "COM9"
#define LASER_SERIALPORT_NUMBER "COM6"
#define GYROSCOPE_SERIALPORT_NUMBER "COM9"

#define IPADDRESSPATH "C:/Users/14017/Desktop/Substation_Project/Holder_Laser_20190411/ipadress.txt"

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
