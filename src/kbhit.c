#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)

#include "kbhit.h"

#include <poll.h>
#include <termios.h>
#include <sys/ioctl.h>

static struct termios oldtio;
static struct termios curtio;

void term_setup(void (*sighandler)(int))
{
    struct sigaction sa;
    
    /* Save stdin terminal attributes */
    tcgetattr(0, &oldtio);

    if(sighandler){
    /* Make sure we exit cleanly */
        memset(&sa, 0, sizeof(struct sigaction));
        sa.sa_handler = sighandler;
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGQUIT, &sa, NULL);
        sigaction(SIGTERM, &sa, NULL);
    }

    /* This is needed to be able to tcsetattr() after a hangup (Ctrl-C)
     * see tcsetattr() on POSIX
     */
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGTTOU, &sa, NULL);

    /* Set non-canonical no-echo for stdin */
    tcgetattr(0, &curtio);
    curtio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &curtio);
}

void term_restore()
{
    /* restore terminal attributes */
    tcsetattr(0, TCSANOW, &oldtio);
}

static char get[4]= {0};

bool kbhit()
{
    struct pollfd pfds[1];
    int ret;
    memset(get, 0, sizeof(char) * 4);

    /* See if there is data available */
    pfds[0].fd = 0;
    pfds[0].events = POLLIN;
    ret = poll(pfds, 1, 0);

    /* Consume data */
    if (ret > 0) 
    {
        read(0, get, 3);
        return strlen(get) > 0;
    }
    return false;
}

bool keydown(const char* key)
{
    return !strcmp(get, key);
}

#endif