
#########################################################################

# Makefile

#########################################################################

# library name

lib.name = Porres-ELS


#########################################################################

# sources

# control:
cents2ratio.class.sources := classes/cents2ratio.c
ratio2cents.class.sources := classes/ratio2cents.c
hz2rad.class.sources := classes/hz2rad.c

# rescale.class.sources := classes/rescale.c

# signal:
cents2ratio~.class.sources := classes/cents2ratio~.c
ratio2cents~.class.sources := classes/ratio2cents~.c
sh~.class.sources := classes/sh~.c

imp~.class.sources := classes/imp~.c


# median~.class.sources := classes/median_tilde.c


#########################################################################

# extra files

datafiles = Porres-ELS-meta.pd README.md


#########################################################################

# include Makefile.pdlibbuilder

include pd-lib-builder/Makefile.pdlibbuilder
