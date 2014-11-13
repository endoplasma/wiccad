#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>

caddr_t _sbrk(int increment)
{
    extern char end asm("end");
    register char *pStack asm("sp");

    static char *s_pHeapEnd;

    if (!s_pHeapEnd)
        s_pHeapEnd = &end;

    if (s_pHeapEnd + increment > pStack)
        return (caddr_t)-1;

    char *pOldHeapEnd = s_pHeapEnd;
    s_pHeapEnd += increment;
    return (caddr_t)pOldHeapEnd;
}
