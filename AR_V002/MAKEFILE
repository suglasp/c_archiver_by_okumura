########################################
#  'ar' -- compression archiver        #
#  makefile for Turbo C compact model  #
########################################

MDL  = c  ##### compact model #####
OBJS = ar.obj io.obj encode.obj decode.obj maketree.obj maketbl.obj huf.obj

ar.exe: $(OBJS)
	tlink /c /d /x a:\tc\lib\c0$(MDL) \
	ar io encode decode maketree maketbl huf, ar, , a:\tc\lib\c$(MDL)

.c.obj:
	tcc -c -m$(MDL) -N -w -w-stv -Ia:\tc\include $<

$(OBJS): ar.h
