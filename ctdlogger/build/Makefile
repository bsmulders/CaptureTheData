INCDIR=../include
SRCDIR=../src
OBJDIR=../obj
LIBDIR=../lib
SQLDIR =../database

# Link command:
ctd : $(OBJDIR)/common.o $(OBJDIR)/ctd.o $(OBJDIR)/gps.o $(OBJDIR)/gsm.o $(OBJDIR)/obd.o $(OBJDIR)/trip.o
	gcc -l sqlite3 $(OBJDIR)/common.o $(OBJDIR)/ctd.o $(OBJDIR)/gps.o $(OBJDIR)/gsm.o $(OBJDIR)/obd.o $(OBJDIR)/trip.o $(LIBDIR)/nmealib/lib/libnmea.a -o ctd

# Compilation commands:
$(OBJDIR)/common.o : $(SRCDIR)/common.c $(INCDIR)/common.h
	gcc -I $(INCDIR) -c $(SRCDIR)/common.c -o $(OBJDIR)/common.o

$(OBJDIR)/ctd.o : $(SRCDIR)/ctd.c $(INCDIR)/ctd.h $(INCDIR)/trip.h $(INCDIR)/gps.h $(INCDIR)/gsm.h $(INCDIR)/obd.h
	gcc -I $(INCDIR) -c $(SRCDIR)/ctd.c -o $(OBJDIR)/ctd.o

$(OBJDIR)/gps.o : $(SRCDIR)/gps.c $(INCDIR)/gps.h $(INCDIR)/common.h $(INCDIR)/ctd.h $(LIBDIR)/nmealib/lib/libnmea.a
	gcc -I $(INCDIR) -I $(LIBDIR)/nmealib/include -std=c99 -c $(SRCDIR)/gps.c -o $(OBJDIR)/gps.o

$(OBJDIR)/gsm.o : $(SRCDIR)/gsm.c $(INCDIR)/gsm.h $(INCDIR)/ctd.h $(INCDIR)/common.h
	gcc -I $(INCDIR) -std=c99 -c $(SRCDIR)/gsm.c -o $(OBJDIR)/gsm.o

$(OBJDIR)/obd.o : $(SRCDIR)/obd.c $(INCDIR)/obd.h
	gcc -I $(INCDIR) -c $(SRCDIR)/obd.c -o $(OBJDIR)/obd.o

$(OBJDIR)/trip.o : $(SRCDIR)/trip.c $(INCDIR)/trip.h $(INCDIR)/gsm.h $(INCDIR)/gps.h $(INCDIR)/ctd.h
	gcc -I $(INCDIR) -c $(SRCDIR)/trip.c -o $(OBJDIR)/trip.o

$(LIBDIR)/nmealib/lib/libnmea.a : $(LIBDIR)/nmealib/include/nmea/nmea.h
	@cd $(LIBDIR)/nmealib ; make

# Make commands
clean: 
	rm -f $(OBJDIR)/*.o
	rm -f ctd