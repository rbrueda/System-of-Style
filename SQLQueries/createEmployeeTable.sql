DROP TABLE Appointments;

CREATE TABLE Appointments (
    apptDate DATE,
    timeOfDay DECIMAL(3,1),
    customerID varchar(26), 
    employeeID VARCHAR(26),
    PRIMARY KEY(customerID),
    UNIQUE(employeeID, apptDate, timeOfDay)
);




INSERT INTO Appointments (apptDate, timeOfDay, customerID, employeeID)
VALUES (
    '2023-08-01',
    '10',
    '65489jg54',SELECT * FROM Appointments 
WHERE timeOfDay=DATE(2023-08-01) and employeeID='fj34f3443';
    'fj34f3443'
  );


INSERT INTO Appointments (apptDate, timeOfDay, customerID, employeeID)
VALUES (
    '2023-08-01',
    '13',
    '8f4f434f',
    'fj34f3443'
  );
  

INSERT INTO Appointments (apptDate, timeOfDay, customerID, employeeID)
VALUES (
    '2023-08-01',
    '9',
    'ivu88944jf',
    'fj34f3443'
  );SELECT * FROM Appointments 
WHERE timeOfDay=DATE(2023-08-01) and employeeID='fj34f3443';


INSERT INTO Appointments (apptDate, timeOfDay, customerID, employeeID)
VALUES (
    '2023-08-01',
    '16',
    'f843f43f',
    'fj34f3443'
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
INSERT INTO EmployeeShifts (
    employeeID,
    mon_start,
    mon_end,
    tues_start,
    tues_end,
    wed_start,
    wed_end,
    thurs_start,
    thurs_end,
    fri_start,
    fri_end,
    sat_start,
    sat_end,
    sun_start,
    sun_end
  )
VALUES (
    'employeeID:varchar',
    'mon_start:decimal',
    'mon_end:decimal',
    'tues_start:decimal',
    'tues_end:decimal',
    'wed_start:decimal',
    'wed_end:decimal',
    'thurs_start:decimal',
    'thurs_end:decimal',
    'fri_start:decimal',
    'fri_end:decimal',
    'sat_start:decimal',
    'sat_end:decimal',
    'sun_start:decimal',
    'sun_end:decimal'
  );

INSERT INTO EmployeeShifts (
    employeeID,
    mon_start,
    mon_end,
    tues_start,
    tues_end,
    wed_start,
    wed_end,
    thurs_start,
    thurs_end,
    fri_start,
    fri_end,
    sat_start,
    sat_end,
    sun_start,
    sun_end
  )
VALUES (
    'gdrgrdgred',
    9.0,
    17.0,
    9.0,
    17.0,
    9.0,
    17.0,
    9.0,
    17.0,
    9.0,
    17.0,
    9.0,
    17.0,
    NULL,
    NULL
  );


SELECT mon_start, mon_end, tues_start, tues_end, wed_start, wed_end, thurs_start, thurs_end, fri_start, fri_end, sat_start, sat_end, sun_start, sun_end FROM EmployeeShifts WHERE employeeID = 'TMMtIFID2dUZg3Gv';



SELECT mon_start, mon_end FROM EmployeeShifts
WHERE employeeID="fj34f3443";


-- Cancel Appointment -- 
DELETE FROM Appointments WHERE customerID="43f43rf";

DELETE FROM Appointments;

SELECT timeOfDay FROM Appointments WHERE apptDate=DATE("2023-05-24") and employeeID='TM3JlK0wjfYm3YX6' ORDER BY timeOfDay ASC;

DELETE FROM EmployeeShifts WHERE employeeID="fj34f3443";

-- YYYY-MM-DD

-- GET AVAILABLE DATES --


SELECT timeOfDay FROM Appointments 
WHERE timeOfDay=DATE(2023-08-01) and employeeID='fj34f3443'
ORDER BY timeOfDay ASC;

DROP 


SELECT timeOfDay FROM Appointments WHERE timeOfDay=DATE(2023-08-01) and employeeID='fj34f3443' ORDER BY timeOfDay ASC;
