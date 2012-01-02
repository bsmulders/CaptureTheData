PRAGMA foreign_keys = false;

-- ----------------------------
--  Table structure for "GpsData"
-- ----------------------------
DROP TABLE IF EXISTS "GpsData";
CREATE TABLE "GpsData" (
	 "Trip_ID" integer NOT NULL,
	 "Gps_ID" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
	 "TimeStamp" real NOT NULL,
	 "RawData" text NOT NULL,
	 "UTC" text,
	 "Fix" integer,
	 "Latitude" real,
	 "Longitude" real,
	 "Speed" real,
	 "Direction" real,
	 "Declination" real,
	CONSTRAINT "Trip_ID" FOREIGN KEY ("Trip_ID") REFERENCES "Trip" ("Trip_ID")
);

-- ----------------------------
--  Table structure for "GpsReport"
-- ----------------------------
DROP TABLE IF EXISTS "GpsReport";
CREATE TABLE "GpsReport" (
	 "Trip_ID" integer NOT NULL,
	 "Gps_ID" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
	 "TimeStamp" integer NOT NULL,
	 "TimeStampSub" integer NOT NULL,
	 "UTC" text,
	 "Fix" integer,
	 "Latitude" real,
	 "Longitude" real,
	 "Speed" real,
	 "Direction" real,
	 "Declination" real,
	CONSTRAINT "Trip_ID" FOREIGN KEY ("Trip_ID") REFERENCES "Trip" ("Trip_ID")
);

-- ----------------------------
--  Table structure for "GsmData"
-- ----------------------------
DROP TABLE IF EXISTS "GsmData";
CREATE TABLE "GsmData" (
	 "Trip_ID" integer NOT NULL,
	 "Gsm_ID" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
	 "TimeStamp" real NOT NULL,
	 "RawData" text NOT NULL,
	 "Command" text,
	 "Value" text,
	CONSTRAINT "Trip_ID" FOREIGN KEY ("Trip_ID") REFERENCES "Trip" ("Trip_ID")
);

-- ----------------------------
--  Table structure for "GsmReport"
-- ----------------------------
DROP TABLE IF EXISTS "GsmReport";
CREATE TABLE "GsmReport" (
	 "Trip_ID" integer NOT NULL,
	 "Gsm_ID" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
	 "TimeStamp" integer NOT NULL,
	 "TimeStampSub" integer NOT NULL,
	 "SignalStrength" integer,
	CONSTRAINT "Trip_ID" FOREIGN KEY ("Trip_ID") REFERENCES "Trip" ("Trip_ID")
);

-- ----------------------------
--  Table structure for "ObdData"
-- ----------------------------
DROP TABLE IF EXISTS "ObdData";
CREATE TABLE "ObdData" (
	 "Trip_ID" integer NOT NULL,
	 "Obd_ID" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
	 "TimeStamp" real NOT NULL,
	 "RawData" text NOT NULL,
	 "Mode" integer,
	 "PID" integer,
	 "Value" real,
	CONSTRAINT "Trip_ID" FOREIGN KEY ("Trip_ID") REFERENCES "Trip" ("Trip_ID")
);

-- ----------------------------
--  Table structure for "ObdReport"
-- ----------------------------
DROP TABLE IF EXISTS "ObdReport";
CREATE TABLE "ObdReport" (
	 "Trip_ID" integer NOT NULL,
	 "Obd_ID" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
	 "TimeStamp" integer NOT NULL,
	 "TimeStampSub" integer NOT NULL,
	 "CalculatedEngineLoad" integer,
	 "EngineCoolantTemperature" integer,
	 "EngineRPM" integer,
	 "VehicleSpeed" integer,
	 "ThrottlePosition" integer,
	CONSTRAINT "Trip_ID" FOREIGN KEY ("Trip_ID") REFERENCES "Trip" ("Trip_ID")
);

-- ----------------------------
--  Table structure for "Trip"
-- ----------------------------
DROP TABLE IF EXISTS "Trip";
CREATE TABLE "Trip" (
	 "Trip_ID" integer PRIMARY KEY AUTOINCREMENT,
	 "Name" text,
	 "FirstData" real,
	 "LastData" real,
	 "CalculatedKilometers" real,
	 "CalculatedFuel" real,
	 "FirstReport" integer,
	 "FirstReportSub" integer,
	 "LastReport" integer,
	 "LastReportSub" integer
);

-- ----------------------------
--  Table structure for "WiiData"
-- ----------------------------
DROP TABLE IF EXISTS "WiiData";
CREATE TABLE "WiiData" (
	 "Trip_ID" integer NOT NULL,
	 "Wii_ID" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
	 "TimeStamp" real NOT NULL,
	 "GforceX" real NOT NULL,
	 "GforceY" real NOT NULL,
	 "GforceZ" real NOT NULL,
	 "Roll" real NOT NULL,
	 "Pitch" real NOT NULL,
	 "Yaw" real NOT NULL,
	 "Speed" real,
	CONSTRAINT "Trip_ID" FOREIGN KEY ("Trip_ID") REFERENCES "Trip" ("Trip_ID")
);

-- ----------------------------
--  Table structure for "WiiReport"
-- ----------------------------
DROP TABLE IF EXISTS "WiiReport";
CREATE TABLE "WiiReport" (
	 "Trip_ID" integer NOT NULL,
	 "Wii_ID" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
	 "TimeStamp" integer NOT NULL,
	 "TimeStampSub" integer NOT NULL,
	 "GforceX" real,
	 "GforceY" real,
	 "GforceZ" real,
	 "Roll" real,
	 "Pitch" real,
	 "Yaw" real,
	 "Speed" real,
	CONSTRAINT "Trip_ID" FOREIGN KEY ("Trip_ID") REFERENCES "Trip" ("Trip_ID")
);

-- ----------------------------
--  Indexes structure for table "GpsData"
-- ----------------------------
CREATE UNIQUE INDEX "DataGps" ON GpsData (TimeStamp DESC);

-- ----------------------------
--  Indexes structure for table "GpsReport"
-- ----------------------------
CREATE UNIQUE INDEX LookupGps ON GpsReport (Trip_ID ASC, TimeStamp ASC, TimeStampSub ASC);

-- ----------------------------
--  Indexes structure for table "GsmData"
-- ----------------------------
CREATE UNIQUE INDEX "DataGsm" ON GsmData (TimeStamp DESC);

-- ----------------------------
--  Indexes structure for table "GsmReport"
-- ----------------------------
CREATE UNIQUE INDEX LookupGsm ON GsmReport (Trip_ID ASC, TimeStamp ASC, TimeStampSub ASC);

-- ----------------------------
--  Indexes structure for table "ObdData"
-- ----------------------------
CREATE UNIQUE INDEX "DataObd" ON ObdData (TimeStamp DESC);

-- ----------------------------
--  Indexes structure for table "ObdReport"
-- ----------------------------
CREATE UNIQUE INDEX LookupObd ON ObdReport (Trip_ID ASC, TimeStamp ASC, TimeStampSub ASC);

-- ----------------------------
--  Indexes structure for table "Trip"
-- ----------------------------
CREATE UNIQUE INDEX "Trip_ID" ON Trip (Trip_ID ASC);

-- ----------------------------
--  Indexes structure for table "WiiData"
-- ----------------------------
CREATE UNIQUE INDEX "DataWii" ON WiiData (TimeStamp DESC);

-- ----------------------------
--  Indexes structure for table "WiiReport"
-- ----------------------------
CREATE UNIQUE INDEX "LookupWii" ON WiiReport (Trip_ID ASC, TimeStamp ASC, TimeStampSub ASC);

PRAGMA foreign_keys = true;