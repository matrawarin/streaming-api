<?php
// stock_chart.php
// Full application: DB + API + Frontend (Chart.js)
// Requirements: PHP 7.4+, PDO extension, MySQL

// ---------------- CONFIG ----------------
$db_host = '127.0.0.1';
$db_port = '3306';
$db_name = 'stock_app';
$db_user = 'root';
$db_pass = '';

$dsn = "mysql:host=$db_host;port=$db_port;dbname=$db_name;charset=utf8mb4";
$options = [
    PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
];

try {
    $pdo = new PDO($dsn, $db_user, $db_pass, $options);
} catch (Exception $e) {
    http_response_code(500);
    echo "DB connection error: " . htmlspecialchars($e->getMessage());
    exit;
}

// ---------------- ROUTER / API ----------------
// Using ?action=... to route; if no action -> return UI page
$action = $_GET['action'] ?? null;

if ($action === 'get_prices') {
    // GET parameters: symbol, start (YYYY-MM-DD), end (YYYY-MM-DD)
    $symbol = $_GET['symbol'] ?? '';
    $start = $_GET['start'] ?? null;
    $end = $_GET['end'] ?? null;

    if (!$symbol) {
        echo json_encode(['error' => 'symbol required']);
        exit;
    }

    $sql = "SELECT dt, open, high, low, close, volume FROM prices WHERE symbol = :symbol";
    $params = [':symbol' => $symbol];

    if ($start) {
        $sql .= " AND dt >= :start";
        $params[':start'] = $start . " 00:00:00";
    }
    if ($end) {
        $sql .= " AND dt <= :end";
        $params[':end'] = $end . " 23:59:59";
    }
    $sql .= " ORDER BY dt ASC LIMIT 10000";

    $stmt = $pdo->prepare($sql);
    $stmt->execute($params);
    $rows = $stmt->fetchAll();

    // return JSON array of {dt, open, high, low, close, volume}
    header('Content-Type: application/json');
    echo json_encode($rows);
    exit;
}

if ($action === 'add_price' && $_SERVER['REQUEST_METHOD'] === 'POST') {
    // POST JSON {symbol, dt, open, high, low, close, volume}
    $data = json_decode(file_get_contents('php://input'), true);
    if (!$data || empty($data['symbol'])) {
        http_response_code(400);
        echo json_encode(['error' => 'invalid payload']);
        exit;
    }
    $sql = "INSERT INTO prices(symbol, dt, open, high, low, close, volume)
            VALUES(:symbol, :dt, :open, :high, :low, :close, :volume)
            ON DUPLICATE KEY UPDATE open=VALUES(open), high=VALUES(high), low=VALUES(low), close=VALUES(close), volume=VALUES(volume)";
    $stmt = $pdo->prepare($sql);
    $stmt->execute([
        ':symbol' => $data['symbol'],
        ':dt' => $data['dt'],
        ':open' => (float)$data['open'],
        ':high' => (float)$data['high'],
        ':low' => (float)$data['low'],
        ':close' => (float)$data['close'],
        ':volume' => (int)($data['volume'] ?? 0),
    ]);
    echo json_encode(['ok' => true]);
    exit;
}

if ($action === 'export_csv') {
    // export symbol / date range -> CSV
    $symbol = $_GET['symbol'] ?? '';
    if (!$symbol) { http_response_code(400); echo "symbol required"; exit; }
    $start = $_GET['start'] ?? null;
    $end = $_GET['end'] ?? null;

    $sql = "SELECT dt, open, high, low, close, volume FROM prices WHERE symbol=:symbol";
    $params = [':symbol' => $symbol];
    if ($start) { $sql .= " AND dt >= :start"; $params[':start'] = $start . " 00:00:00"; }
    if ($end) { $sql .= " AND dt <= :end"; $params[':end'] = $end . " 23:59:59"; }
    $sql .= " ORDER BY dt ASC";

    $stmt = $pdo->prepare($sql);
    $stmt->execute($params);

    header('Content-Type: text/csv');
    header('Content-Disposition: attachment; filename="' . $symbol . '_prices.csv"');
    $out = fopen('php://output', 'w');
    fputcsv($out, ['dt','open','high','low','close','volume']);
    while ($r = $stmt->fetch()) {
        fputcsv($out, [$r['dt'], $r['open'], $r['high'], $r['low'], $r['close'], $r['volume']]);
    }
    fclose($out);
    exit;
}

if ($action === 'import_csv' && $_SERVER['REQUEST_METHOD'] === 'POST') {
    // Expect form upload: <input type="file" name="csv"> and symbol param
    if (empty($_FILES['csv']) || empty($_POST['symbol'])) {
        http_response_code(400);
        echo json_encode(['error' => 'file and symbol required']);
        exit;
    }
    $symbol = strtoupper(trim($_POST['symbol']));
    $tmp = $_FILES['csv']['tmp_name'];
    if (!is_uploaded_file($tmp)) { http_response_code(400); echo json_encode(['error'=>'upload failed']); exit; }

    $fh = fopen($tmp, 'r');
    // Accept header or first row as dt,open,high,low,close,volume
    $header = fgetcsv($fh);
    // We try to parse lines
    $inserted = 0;
    $pdo->beginTransaction();
    $sql = "INSERT INTO prices(symbol, dt, open, high, low, close, volume)
            VALUES(:symbol, :dt, :open, :high, :low, :close, :volume)
            ON DUPLICATE KEY UPDATE open=VALUES(open), high=VALUES(high), low=VALUES(low), close=VALUES(close), volume=VALUES(volume)";
    $stmt = $pdo->prepare($sql);
    while (($row = fgetcsv($fh)) !== false) {
        if (count($row) < 5) continue;
        // try to map dynamically: if header present, map by name
        if ($header && in_array('dt', $header)) {
            $map = array_flip($header);
            $dt = $row[$map['dt']];
            $open = $row[$map['open']];
            $high = $row[$map['high']];
            $low = $row[$map['low']];
            $close = $row[$map['close']];
            $volume = $row[$map['volume']] ?? 0;
        } else {
            // assume dt,open,high,low,close,volume order
            $dt = $row[0];
            $open = $row[1];
            $high = $row[2];
            $low = $row[3];
            $close = $row[4];
            $volume = $row[5] ?? 0;
        }
        // normalize dt if date only
        if (preg_match('/^\d{4}-\d{2}-\d{2}$/', $dt)) $dt .= ' 00:00:00';
        try {
            $stmt->execute([
                ':symbol'=>$symbol, ':dt'=>$dt,
                ':open'=>(float)$open, ':high'=>(float)$high, ':low'=>(float)$low, ':close'=>(float)$close,
                ':volume'=>(int)$volume
            ]);
            $inserted++;
        } catch (Exception $e) {
            // skip malformed row
        }
    }
    $pdo->commit();
    fclose($fh);
    echo json_encode(['ok'=>true, 'inserted'=>$inserted]);
    exit;
}

// No API -> show frontend UI (HTML + JS)
?>
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Stock Chart - PHP + PDO + Chart.js</title>
<!-- Chart.js -->
<script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.umd.min.js"></script>
<!-- optional: chartjs-plugin-zoom for zoom/pan -->
<script src="https://cdn.jsdelivr.net/npm/chartjs-plugin-zoom@2.0.1/dist/chartjs-plugin-zoom.min.js"></script>
<style>
body { font-family: Arial, sans-serif; margin: 20px; }
.container { max-width: 1100px; margin: auto; }
.controls { display:flex; gap:10px; align-items:center; margin-bottom:15px; }
.card { border:1px solid #ddd; padding:12px; border-radius:6px; margin-bottom:12px; }
table { width:100%; border-collapse:collapse; }
th,td { border:1px solid #eee; padding:6px; text-align:left; }
</style>
</head>
<body>
<div class="container">
<h1>Stock Chart (PHP + PDO + Chart.js)</h1>

<div class="card">
    <form id="add-stock-form" onsubmit="return false;">
        <div class="controls">
            <label>Symbol: <input type="text" id="symbol" value="AAPL"></label>
            <label>Start: <input type="date" id="start"></label>
            <label>End: <input type="date" id="end"></label>
            <button id="btn-load">Load Data</button>
            <label><input type="checkbox" id="ma10"> MA10</label>
            <label><input type="checkbox" id="ma20"> MA20</label>
            <button id="btn-export">Export CSV</button>
            <label style="margin-left:10px;">Import CSV:
                <input type="file" id="csvfile" accept=".csv">
            </label>
            <button id="btn-import">Upload</button>
        </div>
    </form>
</div>

<div class="card">
    <canvas id="chart" height="120"></canvas>
</div>

<div class="card">
    <h3>Price Table</h3>
    <div id="table-wrap"></div>
</div>

<script>
// Utility: fetch JSON
async function fetchJson(url, opts){ const r = await fetch(url, opts); return r.json(); }

let chart = null;
const ctx = document.getElementById('chart').getContext('2d');

function buildChart(labels, closes, ma10=null, ma20=null){
    if(chart) chart.destroy();
    const datasets = [{
        label: 'Close',
        data: closes,
        borderColor: 'rgb(34, 139, 230)',
        backgroundColor: 'rgba(34,139,230,0.1)',
        tension: 0.1,
        pointRadius: 0,
    }];
    if(ma10){
        datasets.push({
            label: 'MA10',
            data: ma10,
            borderColor: 'rgb(230, 90, 90)',
            borderDash: [6,4],
            pointRadius: 0,
        });
    }
    if(ma20){
        datasets.push({
            label: 'MA20',
            data: ma20,
            borderColor: 'rgb(90, 200, 90)',
            borderDash: [4,4],
            pointRadius: 0,
        });
    }
    chart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: datasets
        },
        options: {
            responsive: true,
            interaction: { mode: 'index', intersect: false },
            plugins: {
                zoom: {
                    pan: { enabled: true, mode: 'x' },
                    zoom: { wheel: { enabled: true }, pinch: { enabled: true }, mode: 'x' }
                },
                tooltip: { mode: 'index', intersect: false }
            },
            scales: {
                x: { display: true, title: { display: true, text: 'Datetime' } },
                y: { display: true, title: { display: true, text: 'Price' } }
            }
        }
    });
}

function computeMA(values, period){
    const res = [];
    for(let i=0;i<values.length;i++){
        if(i+1 < period) { res.push(null); continue; }
        let sum=0;
        for(let j=i+1-period;j<=i;j++) sum += values[j];
        res.push(sum/period);
    }
    return res;
}

async function loadData(){
    const symbol = document.getElementById('symbol').value.trim();
    if(!symbol) return alert('Enter symbol');
    const start = document.getElementById('start').value;
    const end = document.getElementById('end').value;
    const q = new URLSearchParams({ action:'get_prices', symbol: symbol });
    if(start) q.set('start', start);
    if(end) q.set('end', end);
    const url = window.location.pathname + '?' + q.toString();
    const data = await fetch(url).then(r=>r.json());
    if(data.error) return alert(data.error);

    const labels = data.map(r => r.dt);
    const closes = data.map(r => parseFloat(r.close));
    const ma10 = document.getElementById('ma10').checked ? computeMA(closes, 10) : null;
    const ma20 = document.getElementById('ma20').checked ? computeMA(closes, 20) : null;
    buildChart(labels, closes, ma10, ma20);

    // table
    const tableWrap = document.getElementById('table-wrap');
    let html = '<table><thead><tr><th>dt</th><th>open</th><th>high</th><th>low</th><th>close</th><th>volume</th></tr></thead><tbody>';
    for(const r of data){
        html += `<tr><td>${r.dt}</td><td>${r.open}</td><td>${r.high}</td><td>${r.low}</td><td>${r.close}</td><td>${r.volume}</td></tr>`;
    }
    html += '</tbody></table>';
    tableWrap.innerHTML = html;
}

document.getElementById('btn-load').addEventListener('click', (e) => { e.preventDefault(); loadData(); });
document.getElementById('ma10').addEventListener('change', loadData);
document.getElementById('ma20').addEventListener('change', loadData);

// Export CSV
document.getElementById('btn-export').addEventListener('click', (e) => {
    e.preventDefault();
    const symbol = document.getElementById('symbol').value.trim();
    if(!symbol) return alert('Enter symbol');
    const start = document.getElementById('start').value;
    const end = document.getElementById('end').value;
    const params = new URLSearchParams({ action:'export_csv', symbol: symbol });
    if(start) params.set('start', start); if(end) params.set('end', end);
    window.location = window.location.pathname + '?' + params.toString();
});

// Import CSV
document.getElementById('btn-import').addEventListener('click', async (e) => {
    e.preventDefault();
    const fileEl = document.getElementById('csvfile');
    const symbol = document.getElementById('symbol').value.trim();
    if(!fileEl.files.length) return alert('Choose CSV file');
    if(!symbol) return alert('Enter symbol to import into');
    const form = new FormData();
    form.append('csv', fileEl.files[0]);
    form.append('symbol', symbol);
    const res = await fetch(window.location.pathname + '?action=import_csv', { method:'POST', body: form });
    const json = await res.json();
    alert('Import: ' + JSON.stringify(json));
    loadData();
});

// initial load with default symbol
window.addEventListener('load', () => {
    const yesterday = new Date(Date.now() - 7*24*3600*1000);
    document.getElementById('start').valueAsDate = yesterday;
    loadData();
});
</script>
</div>
</body>
</html>

