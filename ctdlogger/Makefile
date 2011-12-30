INCDIR= include
SRCDIR= src
OBJDIR= obj
LIBDIR= lib
SQLDIR = database
BLDDIR = build

# Link command:
$(BLDDIR)ctd : $(OBJDIR)/common.o $(OBJDIR)/ctd.o $(OBJDIR)/gps.o $(OBJDIR)/gsm.o $(OBJDIR)/obd.o $(OBJDIR)/trip.o $(OBJDIR)/wii.o
	gcc -l sqlite3 $(OBJDIR)/common.o $(OBJDIR)/ctd.o $(OBJDIR)/gps.o $(OBJDIR)/gsm.o $(OBJDIR)/obd.o $(OBJDIR)/trip.o $(OBJDIR)/wii.o $(LIBDIR)/nmealib/lib/libnmea.a $(LIBDIR)/wiiuse/build/src/libwiiuse.dylib -o $(BLDDIR)/ctd

# Compilation commands:
$(OBJDIR)/common.o : $(SRCDIR)/common.c $(INCDIR)/common.h
	gcc -I $(INCDIR) -c $(SRCDIR)/common.c -o $(OBJDIR)/common.o

$(OBJDIR)/ctd.o : $(SRCDIR)/ctd.c $(INCDIR)/trip.h $(INCDIR)/gps.h $(INCDIR)/gsm.h $(INCDIR)/obd.h
	gcc -I $(INCDIR) -c $(SRCDIR)/ctd.c -o $(OBJDIR)/ctd.o

$(OBJDIR)/gps.o : $(SRCDIR)/gps.c $(INCDIR)/gps.h $(INCDIR)/common.h $(LIBDIR)/nmealib/lib/libnmea.a
	gcc -I $(INCDIR) -I $(LIBDIR)/nmealib/include -std=c99 -c $(SRCDIR)/gps.c -o $(OBJDIR)/gps.o

$(OBJDIR)/gsm.o : $(SRCDIR)/gsm.c $(INCDIR)/gsm.h $(INCDIR)/common.h
	gcc -I $(INCDIR) -std=c99 -c $(SRCDIR)/gsm.c -o $(OBJDIR)/gsm.o

$(OBJDIR)/obd.o : $(SRCDIR)/obd.c $(INCDIR)/obd.h
	gcc -I $(INCDIR) -std=c99 -c $(SRCDIR)/obd.c -o $(OBJDIR)/obd.o

$(OBJDIR)/wii.o : $(SRCDIR)/wii.c $(INCDIR)/wii.h $(LIBDIR)/wiiuse/build/src/libwiiuse.dylib
	gcc -I $(INCDIR) -I $(LIBDIR)/wiiuse/src -c $(SRCDIR)/wii.c -o $(OBJDIR)/wii.o

$(OBJDIR)/trip.o : $(SRCDIR)/trip.c $(INCDIR)/trip.h $(INCDIR)/gsm.h $(INCDIR)/gps.h
	gcc -I $(INCDIR) -c $(SRCDIR)/trip.c -o $(OBJDIR)/trip.o

$(LIBDIR)/nmealib/lib/libnmea.a : $(LIBDIR)/nmealib/include/nmea/nmea.h
	@cd $(LIBDIR)/nmealib ; make ; cd ..

$(LIBDIR)/wiiuse/build/src/libwiiuse.dylib : $(LIBDIR)/wiiuse/src/wiiuse.h
	@cd $(LIBDIR)/wiiuse ; mkdir build ; cd build ; cmake .. ; make wiiuse ; cd .. ; cd ..

# Make commands
clean: 
	rm -f $(OBJDIR)/*.o
	rm -f $(BLDDIR)/ctd