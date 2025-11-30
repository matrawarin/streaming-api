CREATE DATABASE stock_app CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE stock_app;

CREATE TABLE prices (
  id INT AUTO_INCREMENT PRIMARY KEY,
  symbol VARCHAR(16) NOT NULL,
  dt DATETIME NOT NULL,
  open DOUBLE NOT NULL,
  high DOUBLE NOT NULL,
  low DOUBLE NOT NULL,
  close DOUBLE NOT NULL,
  volume BIGINT DEFAULT 0,
  UNIQUE(symbol, dt),
  INDEX(idx_symbol_dt) (symbol, dt)
);

-- Optional sample data
INSERT INTO prices(symbol, dt, open, high, low, close, volume) VALUES
('AAPL','2025-11-01 09:30:00',150,152,149,151,100000),
('AAPL','2025-11-01 10:30:00',151,153,150,152,80000),
('AAPL','2025-11-01 11:30:00',152,154,151,153,90000);

