CTD - CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact: mail@bsmulders.com

License: GPLv3

ctdlogger:	Data logger, parser and report generator (written in C)

	This part logs data from several (bluetooth) sensors. It stores the raw
	data in a SQLite3 database. It can also parse the information into human
	readable data, and generate measurement reports for a trip.

	Call the make-file (/ctdlogger/build/Makefile) to build the project.

	Commands to operate the application:
	-Description            -Command                -Example
	Create a new trip:      ./ctd create            ./ctd create
	Start GSM logging:      ./ctd gsm {tripid}      ./ctd gsm 1
	Start GPS logging:      ./ctd gps {tripid}      ./ctd gps 1
	Parse raw serial data   ./ctd parse {tripid}    ./ctd parse 1
	Generate reports        ./ctd report {tripid}   ./ctd report 1

	In a normal application flow you would:
	- Create a trip, save the returned ID somewhere
	- Start both the GSM and GPS logging in two separate processes
	- Cancel the logging when you are finished (CTRL+C)
	- Parse the raw serial data to human understandable data 
	- Generate trip reports with all logging values for every 1/10th of a
	  second in this trip

	Bash scripts to automate the process may be available in the future.

ctdservice:	REST(ish) Web-service (written in PHP)

	The web-service publishes the data from the SQLite3 database. You can 
	get a summary of all trips, get detailed trip information, get 
	information about the sensors used in the trip and get the sensor 
	measurements for every 1/10th of a second in the trip.

	Put the ctdservice directory on a PHP and SQLite3 enabled Apache server. 
	Don't forget the .htaccess file that might be hidden by your OS.

	Access the web-service by browsing to http://yourserver/ctdservice/trips
	 
	Other entry-points are:
	/trips/{tripid}
	/trips/{tripid}/{sensorname[gsm|gps]}
	/trips/{tripid}/{sensorname[gsm|gps]}/{timestamp}.{milliseconds}

ctdapp:		Web-application (written in JavaScript, using ExtJS 4) 

	Put the ctdapp directory on a web-server, or open the index.html file in
	your browser. Don't forget to enable CORS[*] on your web-server if
	you want to run the application locally or on a different server that 
	the web-service is running on.
	
	Access the web-application by browsing to http://yourserver/ctdapp/
	
	Select the trip you want to evaluate and enable widgets by clicking the
	buttons on the widgets field. You can move, resize, maximize (most) 
	widgets to your liking.

	[*] CORS: Cross-Origin Resource Sharing. 
	    http://www.w3.org/TR/cors
	    http://enable-cors.org