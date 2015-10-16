# Edmanuel Torres 09/07/2006
# Edmanuel Torres 05/04/2008

include ../makeinclude

all: 
	@for file in $(MPRG); do\
	    echo "==== Checking $$file ====";\
	    if [ -f $$file ] ; then\
		echo ==========================================;\
		echo "**** $$file was susessful Compiled ****";\
		echo ==========================================;\
	    else\
		echo "!!!! $$file was NOT Compiled !!!!";\
		echo "!!!! Check the makeinclude file !!!!";\
	    fi;\
	done

clean:
	@echo "==== cleaning binary files ===="
	rm -rf *o $(MPRG)



