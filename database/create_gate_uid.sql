--
-- Create table gate_uid
--
CREATE TABLE gate_uid (
	gate VARCHAR(128) NOT NULL,
	uid VARCHAR(128) NOT NULL,
	PRIMARY KEY (gate, uid)
);
