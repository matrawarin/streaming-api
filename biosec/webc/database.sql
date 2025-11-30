CREATE DATABASE IF NOT EXISTS fund_db;
USE fund_db;

CREATE TABLE accounts (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100),
    balance DECIMAL(15,2) DEFAULT 0,
    section_id INT,
    sector_id INT,
    code_id INT
);

CREATE TABLE ledger (
    id INT AUTO_INCREMENT PRIMARY KEY,
    account_id INT,
    type ENUM('DEPOSIT','WITHDRAW'),
    amount DECIMAL(15,2),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (account_id) REFERENCES accounts(id)
);

CREATE TABLE kyc (
    account_id INT PRIMARY KEY,
    full_name VARCHAR(200),
    national_id VARCHAR(20),
    address TEXT,
    phone VARCHAR(20),
    email VARCHAR(100),
    FOREIGN KEY(account_id) REFERENCES accounts(id)
);

CREATE TABLE section (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100)
);

CREATE TABLE sector (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100)
);

CREATE TABLE code (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100)
);

