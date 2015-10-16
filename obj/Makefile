# Edmanuel Torres 05/04/2008

include ../makeinclude

all: 
	@for file in $(MPRG); do\
	    echo "==== Compiling $$file ====";\
	    $(CXX) -o $(XDIR)$$file $$file.o $(FOBJ) $(COBJ) $(LFLAG);\
	done

clean:
	@echo "==== cleaning object files ===="
	rm -rf *o $(FOBJ) $(COBJ) $(MOBJ)