
-- STEP 1: Create the Database to store the data --
CREATE DATABASE HairSalonBookingManager;


-- STEP 2: Create your login information... --
-- username: choose a username --
-- password: choose a password --
-- url: url your will use to connect to DB (localhost if using same machine) --

CREATE USER username@url IDENTIFIED BY 'password';

-- Step 3: Grant permission (use same information as in step 2) --

GRANT ALL PRIVILEGES ON HairSalonBookingManager.* TO username@url IDENTIFIED BY 'password';
FLUSH PRIVILEGES;

-- Step 4: Create Tables -- 

CREATE TABLE Appointments (
    apptDate DATE,
    timeOfDay DECIMAL(3,1),
    customerID varchar(26), 
    employeeID VARCHAR(26),
    PRIMARY KEY(customerID),
    UNIQUE(employeeID, apptDate, timeOfDay)
);


CREATE TABLE EmployeeShifts (
  employeeID VARCHAR(26), 
  mon_start DECIMAL(3,1),
  mon_end DECIMAL(3,1),
  tues_start DECIMAL(3,1),
  tues_end DECIMAL(3,1),
  wed_start DECIMAL(3,1),
  wed_end DECIMAL(3,1),
  thurs_start DECIMAL(3,1),
  thurs_end DECIMAL(3,1),
  fri_start DECIMAL(3,1),
  fri_end DECIMAL(3,1),
  sat_start DECIMAL(3,1),
  sat_end DECIMAL(3,1),
  sun_start DECIMAL(3,1),
  sun_end DECIMAL(3,1),
  PRIMARY KEY(employeeID)

);