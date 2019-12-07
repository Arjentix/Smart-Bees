CREATE DATABASE IF NOT EXISTS AliceDB;
USE AliceDB;

CREATE TABLE IF NOT EXISTS subs_table(
	user_id INT(10) AUTO_INCREMENT NOT NULL,
	sub_start_date DATETIME NOT NULL,
	sub_end_date DATETIME NOT NULL,
	PRIMARY KEY(user_id)
);

GRANT ALL ON AliceDB.* TO 'db_user'@'%' IDENTIFIED BY 'user_password';
