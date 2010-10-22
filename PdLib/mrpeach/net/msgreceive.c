/* x_net_msgreceive.c 20060424. Martin Peach did it based on x_net.c. x_net.c header follows: */
/* Copyright (c) 1997-1999 Miller Puckette.
* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

#include "m_pd.h"
#include "s_stuff.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ----------------------------- msgreceive ------------------------- */

static t_class *msgreceive_class;

#define MAX_MSG_RECEIVE 65536L // longer than data in maximum UDP packet

typedef struct _msgreceive
{
    t_object  x_obj;
    t_outlet  *x_msgout;
    t_outlet  *x_addrout;
    int       x_connectsocket;
    t_atom    x_addrbytes[5];
    t_atom    x_msgoutbuf[MAX_MSG_RECEIVE];
    char      x_msginbuf[MAX_MSG_RECEIVE];
} t_msgreceive;

void msgreceive_setup(void);
static void msgreceive_free(t_msgreceive *x);
static void *msgreceive_new();
static void msgreceive_read(t_msgreceive *x, void* msg);

static void msgreceive_read(t_msgreceive *x, void* msg)
{
    int                 i, read = 0;

    struct msg_struct* msg_data = msg;
    if (msg_data)
    {
        memcpy(x->x_msginbuf, msg_data->data, msg_data->len);
        read = msg_data->len;

        x->x_addrbytes[0].a_w.w_float = 0;
        x->x_addrbytes[1].a_w.w_float = 0;
        x->x_addrbytes[2].a_w.w_float = 0;
        x->x_addrbytes[3].a_w.w_float = 0;
        x->x_addrbytes[4].a_w.w_float = 0;
        outlet_list(x->x_addrout, &s_list, 5L, x->x_addrbytes);

        if (read < 0)
        {
            return;
        }
        if (read > 0)
        {
            for (i = 0; i < read; ++i)
            {
                /* convert the bytes in the buffer to floats in a list */
                x->x_msgoutbuf[i].a_w.w_float = (float)(unsigned char)x->x_msginbuf[i];
            }
            /* send the list out the outlet */
            if (read > 1) outlet_list(x->x_msgout, &s_list, read, x->x_msgoutbuf);
            else outlet_float(x->x_msgout, x->x_msgoutbuf[0].a_w.w_float);
        }
    }
}

static void *msgreceive_new()
{
    t_msgreceive       *x;
    int                i;

    x = (t_msgreceive *)pd_new(msgreceive_class);
    x->x_msgout = outlet_new(&x->x_obj, &s_anything);
    x->x_addrout = outlet_new(&x->x_obj, &s_list);
    x->x_connectsocket = -1;

    /* convert the bytes in the buffer to floats in a list */
    for (i = 0; i < MAX_MSG_RECEIVE; ++i)
    {
        x->x_msgoutbuf[i].a_type = A_FLOAT;
        x->x_msgoutbuf[i].a_w.w_float = 0;
    }
    for (i = 0; i < 5; ++i)
    {
        x->x_addrbytes[i].a_type = A_FLOAT;
        x->x_addrbytes[i].a_w.w_float = 0;
    }
    sys_addpollfn(x->x_connectsocket, (t_fdpollfn)msgreceive_read, x);
    return (x);
}

static void msgreceive_free(t_msgreceive *x)
{
    if (x->x_connectsocket >= 0)
    {
        sys_rmpollfn(x->x_connectsocket);
    }
}

void msgreceive_setup(void)
{
    msgreceive_class = class_new(gensym("msgreceive"),
        (t_newmethod)msgreceive_new, (t_method)msgreceive_free,
        sizeof(t_msgreceive), CLASS_NOINLET, A_DEFFLOAT, 0);
}

/* end msgreceive.c */
