#include "Global.h"

#include "AdcEval.h"

#include <iostream>

#include <termio.h>
#include <fcntl.h>

Global *Global::instance = NULL;

Global::Global()
{
}

Global &Global::Instance()
{
    if (instance == NULL)
    {
        instance = new Global();
    }

    return *instance;
}

int Global::OpenSerial(const char *device, int speed)
{
    struct termios options;    
    int fd;

    if ((fd = open(device, O_RDWR | O_NOCTTY)) == -1)
        return -1;

    fcntl(fd, F_SETFL, 0);
    tcgetattr(fd, &options);
    cfsetispeed(&options, speed);
    cfsetospeed(&options, speed);
    cfmakeraw(&options);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CRTSCTS;
    if (tcsetattr(fd, TCSANOW, &options) != 0)
        return -1;

    return fd;
}