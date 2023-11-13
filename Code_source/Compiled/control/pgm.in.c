// porres 2018

#include "m_pd.h"
#include <string.h>

typedef struct _pgmin{
    t_object       x_obj;
    t_int          x_omni;
    t_float        x_ch;
    t_float        x_ch_in;
    t_int          x_ext;
    unsigned char  x_pgm;
    unsigned char  x_channel;
    t_outlet      *x_chanout;
}t_pgmin;

static t_class *pgmin_class;

static void pgmin_float(t_pgmin *x, t_float f){
    if(f < 0 || f > 256){
        x->x_pgm = 0;
        return;
    }
    else{
        t_int ch = (t_int)x->x_ch_in;
        if(ch != x->x_ch && ch >= 0 && ch <= 16)
            x->x_omni = ((x->x_ch = (t_int)ch) == 0);
        unsigned char val = (int)f;
        if(val & 0x80){ // message type > 128)
            if((x->x_pgm = ((val & 0xF0) == 0xC0))) // if pgm change
                x->x_channel = (val & 0x0F) + 1; // get channel
        }
        else if(x->x_pgm && val < 128){ // output value
            if(x->x_omni){
                outlet_float(x->x_chanout, x->x_channel);
                outlet_float(((t_object *)x)->ob_outlet, val);
            }
            else if(x->x_ch == x->x_channel)
                outlet_float(((t_object *)x)->ob_outlet, val);
            x->x_pgm = 0;
        }
        else
            x->x_pgm = 0;
    }
}

static void pgmin_list(t_pgmin *x, t_symbol *s, int ac, t_atom *av){
    s = NULL;
    if(!ac)
        return;
    if(!x->x_ext)
        pgmin_float(x, atom_getfloat(av));
}

static void pgmin_ext(t_pgmin *x, t_floatarg f){
    x->x_ext = f != 0;
}

static void pgmin_free(t_pgmin *x){
    pd_unbind(&x->x_obj.ob_pd, gensym("#midiin"));
}

static void *pgmin_new(t_symbol *s, int ac, t_atom *av){
    s = NULL;
    t_pgmin *x = (t_pgmin *)pd_new(pgmin_class);
    x->x_pgm = 0;
    int channel = 0;
    if(ac){
        if(atom_getsymbolarg(0, ac, av) == gensym("-ext")){
            x->x_ext = 1;
            ac--, av++;
        }
        channel = (t_int)atom_getfloatarg(0, ac, av);
    }
    if(channel < 0)
        channel = 0;
    if(channel > 16)
        channel = 16;
    x->x_omni = (channel == 0);
    x->x_ch = x->x_ch_in = channel;
    floatinlet_new((t_object *)x, &x->x_ch_in);
    outlet_new((t_object *)x, &s_float);
    x->x_chanout = outlet_new((t_object *)x, &s_float);
    pd_bind(&x->x_obj.ob_pd, gensym("#midiin"));
    return(x);
}

void setup_pgm0x2ein(void){
    pgmin_class = class_new(gensym("pgm.in"), (t_newmethod)pgmin_new,
        (t_method)pgmin_free, sizeof(t_pgmin), 0, A_GIMME, 0);
    class_addfloat(pgmin_class, pgmin_float);
    class_addlist(pgmin_class, pgmin_list);
    class_addmethod(pgmin_class, (t_method)pgmin_ext, gensym("ext"), A_DEFFLOAT, 0);
}
