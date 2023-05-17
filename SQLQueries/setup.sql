CREATE DATABASE HairSalonBookingManager;

CREATE USER testOwner_HSBM@localhost IDENTIFIED BY 'largars123';

GRANT ALL PRIVILEGES ON HairSalonBookingManager.* TO 'testOwner_HSBM'@localhost IDENTIFIED BY 'largars123';


FLUSH PRIVILEGES;