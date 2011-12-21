# CaptureTheData!

## License
Copyright (c) 2011, Bobbie Smulders

Contact: <mail@bsmulders.com>

License: GPLv3

## Project goal
The goal of this project is to create a cheap and modular vehicle data logger using cheap and readily available sensors. These sensors can be simple cellphones, Wiimotes, GPS receivers and more. They have in common that they are all Bluetooth enabled, readily available, don't need to be soldered / modified and have a price of less than €15,00.

The logging application can run on any PC or embedded device. After the data is logged, it can be viewed using a web-application. This application allows the user to select a trip and watch the recorded data in real-time using configurable gauges and information panels.

Currently supported devices and features:

* Bluetooth/Serial GPS receiver *(NMEA based)*
   * UTC
   * Speed
   * Latitude
   * Longitude
   * Direction
   * Declination
* Bluetooth/Serial Cellphone *(Hayes command set based)*
   * Cell-tower signal strength
* Bluetooth/Serial OBD-II device *(ELM327 based)*
   * Battery voltage
   * Speed
   * RPM
   * Engine load
   * Coolant temperature
   * Throttle position

Planned devices for the future:

* Nintendo Wiimote
   * Three-axis accelerometer

## Project components
### ctdlogger
	
Data logger, parser and report generator (written in C).
	
This part logs data from several (bluetooth) sensors. It stores the raw data in a SQLite3 database. It can also parse the information into human readable data and generate measurement reports for a trip.
	
### ctdservice
	
REST(ish) Web-service (written in PHP).
	
The web-service publishes the data from the SQLite3 database. You can get a summary of all trips, get detailed trip information, get information about the sensors used in the trip and get the sensor measurements for every 1/10th of a second in the trip.
	
### ctdapp
	
Web-application (written in JavaScript, using Ext JS 4).
	
The web-application access the web-service and displays all recorded trips. After selecting a trip, it displays the trip in realtime. It is possible to enable and disable various gauges and information panels. A trip can be played in realtime, 10x speed or manually controlled.
	
## Usage
### ctdlogger
	
Call the make-file (/ctdlogger/build/Makefile) to build the project.

Commands to operate the application:

<table>
	<tr>
		<th>Description</th>
		<th>Command</th>
		<th>Example</th>
	</tr>
	<tr>
		<td>Create a new trip</td>
		<td>./ctd create</td>
		<td>./ctd create</td>
	</tr>
	<tr>
		<td>Start logging</td>
		<td>./ctd log {gps|gsm|obd} {tripid}</td>
		<td>./ctd log gsm 1</td>
	</tr>
	<tr>
		<td>Parse raw serial data</td>
		<td>./ctd parse {tripid}</td>
		<td>./ctd parse 1</td>
	</tr>
	<tr>
		<td>Generate reports</td>
		<td>./ctd reports {tripid}</td>
		<td>./ctd reports 1</td>
	</tr>
</table>


In a normal application flow you would:

1. Create a trip, save the returned ID somewhere
2. Start the GSM, GPS and OBD logging in separate processes
3. Cancel the logging when you are finished (CTRL+C)
4. Parse the raw serial data to human understandable data
5. Generate trip reports with all logging values for every 1/10th of a second in this trip

Bash scripts to automate this process may be available in the future.  
Please note that generating the reports can take a while. Generating reports for 30 minutes of recorded data can take up to a few minutes.

### ctdservice

Put the ctdservice directory on a PHP and SQLite3 enabled Apache server.

Don't forget the .htaccess file that might be hidden by your OS.

Access the web-service by browsing to http://yourserver/ctdservice/trips

Other entry-points are:

* /trips/{tripid}
* /trips/{tripid}/measurement/{timestamp}.{milliseconds}

### ctdapp

Put the ctdapp directory on a web-server or open the index.html file in your browser. Don't forget to enable [CORS](http://www.w3.org/TR/cors) on your web-server if you want to run the application locally or on a different server that the web-service is running on. Access the web-application by browsing to http://yourserver/ctdapp/

Select the trip you want to analyze and enable widgets by clicking the buttons on the widgets field. You can move, resize, maximize (most) widgets to your liking.