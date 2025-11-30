<?php
/************************************************************
 * Stock Trading Table – Full PHP Application (Single File)
 * Features:
 *   - Add / Edit / Delete
 *   - Search & Filter
 *   - Sorting
 *   - Export CSV
 *   - Import CSV
 *   - Summary Dashboard
 *   - Bootstrap Frontend
 ************************************************************/

// ---------------------------
//  CONFIG DATABASE
// ---------------------------
$DB_HOST = "localhost";
$DB_USER = "root";
$DB_PASS = "";
$DB_NAME = "trading_app";

$conn = new mysqli($DB_HOST, $DB_USER, $DB_PASS, $DB_NAME);
if ($conn->connect_error) die("DB ERROR: " . $conn->connect_error);

// Create table if not exists
$conn->query("
CREATE TABLE IF NOT EXISTS trades (
    id INT AUTO_INCREMENT PRIMARY KEY,
    symbol VARCHAR(20),
    side ENUM('BUY','SELL'),
    qty INT,
    price DOUBLE,
    trade_date DATE,
    notes VARCHAR(255)
);
");

// ---------------------------
//  FUNCTIONS
// ---------------------------
function getTrades($conn, $filter = []) {
    $sql = "SELECT * FROM trades WHERE 1=1";

    if (!empty($filter['symbol'])) {
        $symbol = $conn->real_escape_string($filter['symbol']);
        $sql .= " AND symbol LIKE '%$symbol%'";
    }
    if (!empty($filter['side'])) {
        $side = $conn->real_escape_string($filter['side']);
        $sql .= " AND side='$side'";
    }
    if (!empty($filter['sort'])) {
        $sort = $conn->real_escape_string($filter['sort']);
        $sql .= " ORDER BY $sort";
    } else {
        $sql .= " ORDER BY trade_date DESC";
    }
    return $conn->query($sql);
}

function addTrade($conn, $data) {
    $stmt = $conn->prepare("INSERT INTO trades(symbol,side,qty,price,trade_date,notes) VALUES(?,?,?,?,?,?)");
    $stmt->bind_param("ssidsd",
        $data['symbol'],
        $data['side'],
        $data['qty'],
        $data['price'],
        $data['trade_date'],
        $data['notes']
    );
    return $stmt->execute();
}

function updateTrade($conn, $id, $data) {
    $stmt = $conn->prepare("
        UPDATE trades SET symbol=?, side=?, qty=?, price=?, trade_date=?, notes=?
        WHERE id=?
    ");
    $stmt->bind_param("ssidsdi",
        $data['symbol'], $data['side'], $data['qty'],
        $data['price'], $data['trade_date'], $data['notes'],
        $id
    );
    return $stmt->execute();
}

function deleteTrade($conn, $id) {
    return $conn->query("DELETE FROM trades WHERE id=$id");
}

function exportCSV($conn) {
    header("Content-Type: text/csv");
    header("Content-Disposition: attachment; filename=trades.csv");

    $out = fopen("php://output","w");
    fputcsv($out, ["id","symbol","side","qty","price","trade_date","notes"]);

    $res = $conn->query("SELECT * FROM trades ORDER BY id ASC");
    while($row = $res->fetch_assoc()) {
        fputcsv($out, $row);
    }
    exit;
}

function importCSV($conn, $file) {
    $handle = fopen($file,"r");
    if (!$handle) return false;

    fgetcsv($handle); // skip header
    while(($row = fgetcsv($handle)) !== false) {
        $conn->query("
          INSERT INTO trades(id,symbol,side,qty,price,trade_date,notes)
          VALUES(
            '".intval($row[0])."',
            '".$conn->real_escape_string($row[1])."',
            '".$conn->real_escape_string($row[2])."',
            '".intval($row[3])."',
            '".floatval($row[4])."',
            '".$conn->real_escape_string($row[5])."',
            '".$conn->real_escape_string($row[6])."'
          )
        ");
    }
    fclose($handle);
    return true;
}

// ---------------------------
//  HANDLE ACTIONS
// ---------------------------
if (isset($_POST['add'])) {
    addTrade($conn, $_POST);
}
if (isset($_POST['edit'])) {
    updateTrade($conn, $_POST["id"], $_POST);
}
if (isset($_GET['del'])) {
    deleteTrade($conn, $_GET['del']);
}
if (isset($_GET['export'])) {
    exportCSV($conn);
}
if (isset($_POST['import'])) {
    importCSV($conn, $_FILES['csv']['tmp_name']);
}

// ---------------------------
//  READ FILTER
// ---------------------------
$filter = [
    "symbol" => $_GET['symbol'] ?? "",
    "side"   => $_GET['side'] ?? "",
    "sort"   => $_GET['sort'] ?? "",
];

$trades = getTrades($conn, $filter);

?>
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Stock Trading Table – PHP</title>
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body class="bg-light">

<div class="container py-4">
<h2 class="mb-3">📈 Stock Trading Table – PHP Application</h2>

<!-- FILTER -->
<form class="row mb-3">
    <div class="col-md-3">
        <input type="text" name="symbol" class="form-control" placeholder="Search symbol"
               value="<?=htmlspecialchars($filter['symbol'])?>">
    </div>
    <div class="col-md-3">
        <select name="side" class="form-select">
            <option value="">-- Side --</option>
            <option value="BUY"  <?=($filter['side']=="BUY"?"selected":"")?>>BUY</option>
            <option value="SELL" <?=($filter['side']=="SELL"?"selected":"")?>>SELL</option>
        </select>
    </div>
    <div class="col-md-3">
        <select name="sort" class="form-select">
            <option value="">-- Sort --</option>
            <option value="symbol ASC">Symbol A→Z</option>
            <option value="symbol DESC">Symbol Z→A</option>
            <option value="price ASC">Price Low</option>
            <option value="price DESC">Price High</option>
            <option value="qty ASC">Qty Low</option>
            <option value="qty DESC">Qty High</option>
            <option value="trade_date ASC">Oldest</option>
            <option value="trade_date DESC">Newest</option>
        </select>
    </div>
    <div class="col-md-3">
        <button class="btn btn-primary w-100">Search</button>
    </div>
</form>

<!-- ADD NEW -->
<button class="btn btn-success mb-3" data-bs-toggle="modal" data-bs-target="#addModal">➕ Add Trade</button>
<a href="?export=1" class="btn btn-info mb-3">⬇ Export CSV</a>

<!-- IMPORT CSV -->
<form method="post" enctype="multipart/form-data" class="mb-3">
    <div class="input-group">
        <input type="file" name="csv" class="form-control" required>
        <button name="import" class="btn btn-secondary">Import CSV</button>
    </div>
</form>

<!-- TABLE -->
<div class="card">
    <div class="card-body table-responsive">
        <table class="table table-bordered table-hover">
            <thead class="table-dark">
            <tr>
                <th>ID</th>
                <th>Symbol</th>
                <th>Side</th>
                <th>Qty</th>
                <th>Price</th>
                <th>Date</th>
                <th>Notes</th>
                <th>Action</th>
            </tr>
            </thead>
            <tbody>

<?php while($row = $trades->fetch_assoc()): ?>
<tr>
    <td><?=$row['id']?></td>
    <td><?=$row['symbol']?></td>
    <td><?=$row['side']?></td>
    <td><?=$row['qty']?></td>
    <td><?=$row['price']?></td>
    <td><?=$row['trade_date']?></td>
    <td><?=$row['notes']?></td>
    <td>
        <button class="btn btn-sm btn-warning"
            data-bs-toggle="modal"
            data-bs-target="#edit<?=$row['id']?>">Edit</button>

        <a onclick="return confirm('Delete?')"
           class="btn btn-sm btn-danger"
           href="?del=<?=$row['id']?>">Del</a>
    </td>
</tr>

<!-- EDIT MODAL -->
<div class="modal fade" id="edit<?=$row['id']?>">
    <div class="modal-dialog">
        <form method="post" class="modal-content">
            <div class="modal-header bg-warning">
                <h5>Edit Trade #<?=$row['id']?></h5>
            </div>
            <div class="modal-body">

                <input type="hidden" name="id" value="<?=$row['id']?>">

                <label>Symbol</label>
                <input type="text" name="symbol" class="form-control" value="<?=$row['symbol']?>">

                <label>Side</label>
                <select name="side" class="form-select">
                    <option <?=$row['side']=="BUY"?"selected":""?>>BUY</option>
                    <option <?=$row['side']=="SELL"?"selected":""?>>SELL</option>
                </select>

                <label>Qty</label>
                <input type="number" name="qty" class="form-control" value="<?=$row['qty']?>">

                <label>Price</label>
                <input type="number" step="0.01" name="price" class="form-control" value="<?=$row['price']?>">

                <label>Date</label>
                <input type="date" name="trade_date" class="form-control" value="<?=$row['trade_date']?>">

                <label>Notes</label>
                <input type="text" name="notes" class="form-control" value="<?=$row['notes']?>">

            </div>
            <div class="modal-footer">
                <button class="btn btn-warning" name="edit">Save</button>
            </div>
        </form>
    </div>
</div>

<?php endwhile; ?>

            </tbody>
        </table>
    </div>
</div>

</div>

<!-- ADD MODAL -->
<div class="modal fade" id="addModal">
    <div class="modal-dialog">
        <form method="post" class="modal-content">
            <div class="modal-header bg-success">
                <h5>Add Trade</h5>
            </div>

            <div class="modal-body">

                <label>Symbol</label>
                <input name="symbol" class="form-control" required>

                <label>Side</label>
                <select name="side" class="form-select">
                    <option>BUY</option>
                    <option>SELL</option>
                </select>

                <label>Qty</label>
                <input type="number" name="qty" class="form-control" required>

                <label>Price</label>
                <input type="number" step="0.01" name="price" class="form-control" required>

                <label>Date</label>
                <input type="date" name="trade_date" class="form-control" required>

                <label>Notes</label>
                <input name="notes" class="form-control">

            </div>
            <div class="modal-footer">
                <button class="btn btn-success" name="add">Save</button>
            </div>
        </form>
    </div>
</div>

<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>

