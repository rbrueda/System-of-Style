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

  

-- YYYY-MM-DD

-- GET AVAILABLE DATES --


SELECT timeOfDay FROM Appointments 
WHERE timeOfDay=DATE(2023-08-01) and employeeID='fj34f3443'
ORDER BY timeOfDay ASC;



SELECT timeOfDay FROM Appointments WHERE timeOfDay=DATE(2023-08-01) and employeeID='fj34f3443' ORDER BY timeOfDay ASC;
