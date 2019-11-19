CREATE DATABASE IF NOT EXISTS AliceDB;
USE AliceDB;

CREATE TABLE IF NOT EXISTS subs_table(
	user_id INT(10) AUTO_INCREMENT NOT NULL,
	username VARCHAR(30) NOT NULL,
	sub_start_date DATE NOT NULL,
	sub_end_date DATE NOT NULL,
	PRIMARY KEY(user_id)
);

CREATE USER IF NOT EXISTS 'db_user' IDENTIFIED BY 'user_password';
GRANT ALL ON AliceDB.* TO 'db_user'@localhost IDENTIFIED BY 'user_password';
