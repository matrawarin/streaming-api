CREATE TABLE IF NOT EXISTS kyc (
    account_id INTEGER PRIMARY KEY,
    full_name TEXT,
    national_id TEXT,
    address TEXT,
    phone TEXT,
    email TEXT,
    FOREIGN KEY(account_id) REFERENCES accounts(id)
);

