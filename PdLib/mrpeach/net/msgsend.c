/* msgsend.c 20060424. Martin Peach did it based on x_net.c. x_net.c header follows: */
/* Copyright (c) 1997-1999 Miller Puckette.
* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

/* network */

#include "m_pd.h"
#include "s_main.h"
#include "s_stuff.h"
#include "queue.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

static t_class *msgsend_class;

typedef struct _msgsend
{
    t_object x_obj;
    int      x_fd;
} t_msgsend;

void msgsend_setup(void);
static void msgsend_free(t_msgsend *x);
static void msgsend_send(t_msgsend *x, t_symbol *s, int argc, t_atom *argv);
static void *msgsend_new(void);

static void *msgsend_new(void)
{
    t_msgsend *x = (t_msgsend *)pd_new(msgsend_class);
    outlet_new(&x->x_obj, &s_float);
    x->x_fd = -1;
    return (x);
}

static void msgsend_send(t_msgsend *x, t_symbol *s, int argc, t_atom *argv)
{
#define BYTE_BUF_LEN 65536 // arbitrary maximum similar to max IP packet size
    static char    byte_buf[BYTE_BUF_LEN];
    int            d;
    int            i, j;
    unsigned char  c;
    float          f, e;
    int            length;
    char           fpath[FILENAME_MAX];
    FILE           *fptr;

#ifdef OSC_DEBUG
    post("s: %s", s->s_name);
    post("argc: %d", argc);
#endif
    for (i = j = 0; i < argc; ++i)
    {
        if (argv[i].a_type == A_FLOAT)
        {
            f = argv[i].a_w.w_float;
            d = (int)f;
            e = f - d;
            if (e != 0)
            {
                error("msgsend_send: item %d (%f) is not an integer", i, f);
                return;
            }
	        c = (unsigned char)d;
	        if (c != d)
            {
                error("msgsend_send: item %d (%f) is not between 0 and 255", i, f);
                return;
            }
#ifdef OSC_DEBUG
	        post("msgsend_send: argv[%d]: %d", i, c);
#endif
	        byte_buf[j++] = c;
        }
        else if (argv[i].a_type == A_SYMBOL)
        {
            atom_string(&argv[i], fpath, FILENAME_MAX);
#ifdef OSC_DEBUG
            post ("msgsend fname: %s", fpath);
#endif
            fptr = fopen(fpath, "rb");
            if (fptr == NULL)
            {
                post("msgsend: unable to open \"%s\"", fpath);
                return;
            }
            rewind(fptr);
#ifdef OSC_DEBUG
            post("msgsend: d is %d", d);
#endif
            while ((d = fgetc(fptr)) != EOF)
            {
                byte_buf[j++] = (char)(d & 0x0FF);
#ifdef OSC_DEBUG
                post("msgsend: byte_buf[%d] = %d", j-1, byte_buf[j-1]);
#endif
                if (j >= BYTE_BUF_LEN)
                {
                    post ("msgsend: file too long, truncating at %lu", BYTE_BUF_LEN);
                    break;
                }
            }
            fclose(fptr);
            fptr = NULL;
            post("msgsend: read \"%s\" length %d byte%s", fpath, j, ((d==1)?"":"s"));
        }
        else
        {
            error("msgsend_send: item %d is not a float or a file name", i);
            return;
        }
    }

    length = j;
    if ((x->x_fd == -1) && (length > 0))
    {
        sys_msgadd_recv(byte_buf, length);
    }
    else error("msgsend: not connected");
}

static void msgsend_free(t_msgsend *x)
{
}

void msgsend_setup(void)
{
    msgsend_class = class_new(gensym("msgsend"), (t_newmethod)msgsend_new,
        (t_method)msgsend_free,
        sizeof(t_msgsend), 0, 0);
    class_addmethod(msgsend_class, (t_method)msgsend_send, gensym("send"),
        A_GIMME, 0);
    class_addlist(msgsend_class, (t_method)msgsend_send);
}

/* end msgsend.c*/

