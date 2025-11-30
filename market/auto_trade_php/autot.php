<?php
session_start();

// -------------------
// DATABASE CONFIG
// -------------------
$DB_HOST = 'localhost';
$DB_USER = 'root';
$DB_PASS = '';
$DB_NAME = 'stock_auto';

$conn = new mysqli($DB_HOST, $DB_USER, $DB_PASS, $DB_NAME);
if ($conn->connect_error) die("DB Connection Failed: " . $conn->connect_error);

// CREATE TABLES IF NOT EXISTS
$conn->query("
CREATE TABLE IF NOT EXISTS stocks (
    symbol VARCHAR(10) PRIMARY KEY,
    name VARCHAR(50),
    price DOUBLE
)");
$conn->query("
CREATE TABLE IF NOT EXISTS portfolio (
    symbol VARCHAR(10) PRIMARY KEY,
    shares INT,
    avg_price DOUBLE
)");
$conn->query("
CREATE TABLE IF NOT EXISTS trades (
    id INT AUTO_INCREMENT PRIMARY KEY,
    symbol VARCHAR(10),
    type ENUM('BUY','SELL'),
    shares INT,
    price DOUBLE,
    total DOUBLE,
    trade_time DATETIME DEFAULT CURRENT_TIMESTAMP
)");
    
// -------------------
// HELPER FUNCTIONS
// -------------------
function getStocks($conn){
    $res = $conn->query("SELECT * FROM stocks");
    return $res->fetch_all(MYSQLI_ASSOC);
}

function getPortfolio($conn){
    $res = $conn->query("SELECT * FROM portfolio");
    return $res->fetch_all(MYSQLI_ASSOC);
}

function addTrade($conn, $symbol, $type, $shares){
    $stock = $conn->query("SELECT * FROM stocks WHERE symbol='$symbol'")->fetch_assoc();
    if(!$stock) return false;
    $price = $stock['price'];
    $total = $price * $shares;

    $conn->query("INSERT INTO trades(symbol,type,shares,price,total) VALUES('$symbol','$type',$shares,$price,$total)");

    $portfolio = $conn->query("SELECT * FROM portfolio WHERE symbol='$symbol'")->fetch_assoc();
    if($type == 'BUY'){
        if($portfolio){
            $newShares = $portfolio['shares'] + $shares;
            $newAvg = (($portfolio['shares']*$portfolio['avg_price']) + ($shares*$price)) / $newShares;
            $conn->query("UPDATE portfolio SET shares=$newShares, avg_price=$newAvg WHERE symbol='$symbol'");
        } else {
            $conn->query("INSERT INTO portfolio(symbol,shares,avg_price) VALUES('$symbol',$shares,$price)");
        }
    } else { // SELL
        if($portfolio && $portfolio['shares'] >= $shares){
            $newShares = $portfolio['shares'] - $shares;
            $conn->query("UPDATE portfolio SET shares=$newShares WHERE symbol='$symbol'");
        }
    }
    return true;
}

// Auto Trade Strategy Example
function autoTrade($conn){
    $stocks = getStocks($conn);
    foreach($stocks as $s){
        if($s['price'] < 50){ // Buy if price < 50
            addTrade($conn, $s['symbol'], 'BUY', 10);
        } elseif($s['price'] > 100){ // Sell all if price > 100
            $portfolio = $conn->query("SELECT * FROM portfolio WHERE symbol='{$s['symbol']}'")->fetch_assoc();
            if($portfolio && $portfolio['shares']>0){
                addTrade($conn, $s['symbol'], 'SELL', $portfolio['shares']);
            }
        }
    }
}

// Simulate Stock Price Update
function updateStockPrices($conn){
    $stocks = getStocks($conn);
    foreach($stocks as $s){
        $change = rand(-50,50)/10; // -5 to +5
        $newPrice = $s['price'] + $change;
        if($newPrice<1) $newPrice=1;
        $conn->query("UPDATE stocks SET price=$newPrice WHERE symbol='{$s['symbol']}'");
    }
}

// -------------------
// HANDLE ACTIONS
// -------------------
if(isset($_POST['add_stock'])){
    $symbol = strtoupper($_POST['symbol']);
    $name = $_POST['name'];
    $price = floatval($_POST['price']);
    $conn->query("INSERT INTO stocks(symbol,name,price) VALUES('$symbol','$name',$price) ON DUPLICATE KEY UPDATE name='$name',price=$price");
}

if(isset($_POST['manual_trade'])){
    addTrade($conn,$_POST['symbol'],$_POST['type'],intval($_POST['shares']));
}

if(isset($_POST['auto_trade'])){
    autoTrade($conn);
}

if(isset($_POST['update_prices'])){
    updateStockPrices($conn);
}

$stocks = getStocks($conn);
$portfolio = getPortfolio($conn);
?>

<!DOCTYPE html>
<html>
<head>
<title>Automated Stock Trading System</title>
<style>
table {border-collapse: collapse; width: 80%;}
th, td {border:1px solid #000; padding:5px; text-align:center;}
</style>
</head>
<body>

<h2>Stocks</h2>
<table>
<tr><th>Symbol</th><th>Name</th><th>Price</th></tr>
<?php foreach($stocks as $s){ ?>
<tr><td><?= $s['symbol'] ?></td><td><?= $s['name'] ?></td><td><?= number_format($s['price'],2) ?></td></tr>
<?php } ?>
</table>

<h3>Add Stock</h3>
<form method="POST">
Symbol: <input name="symbol" required>
Name: <input name="name" required>
Price: <input name="price" type="number" step="0.01" required>
<button name="add_stock">Add / Update</button>
</form>

<h3>Manual Trade</h3>
<form method="POST">
Symbol: <input name="symbol" required>
Type: <select name="type"><option>BUY</option><option>SELL</option></select>
Shares: <input name="shares" type="number" required>
<button name="manual_trade">Execute</button>
</form>

<h3>Auto Trade & Price Update</h3>
<form method="POST">
<button name="update_prices">Update Prices</button>
<button name="auto_trade">Run Auto Trade</button>
</form>

<h2>Portfolio</h2>
<table>
<tr><th>Symbol</th><th>Shares</th><th>Avg Buy Price</th><th>Current Price</th><th>Value</th></tr>
<?php foreach($portfolio as $p){ 
    $current = $conn->query("SELECT price FROM stocks WHERE symbol='{$p['symbol']}'")->fetch_assoc()['price'];
    $value = $current * $p['shares'];
?>
<tr>
<td><?= $p['symbol'] ?></td>
<td><?= $p['shares'] ?></td>
<td><?= number_format($p['avg_price'],2) ?></td>
<td><?= number_format($current,2) ?></td>
<td><?= number_format($value,2) ?></td>
</tr>
<?php } ?>
</table>

</body>
</html>

