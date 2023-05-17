CREATE DATABASE HairSalonBookingManager;

CREATE USER username@localhost IDENTIFIED BY 'password';

GRANT ALL PRIVILEGES ON HairSalonBookingManager.* TO username@localhost IDENTIFIED BY 'password';


FLUSH PRIVILEGES;
