<?php
include 'config.php';
$stmt = $pdo->query("SELECT * FROM ledger");
$ledger = $stmt->fetchAll(PDO::FETCH_ASSOC);
?>
<h2>Ledger</h2>
<table border="1">
<tr><th>ID</th><th>AccountID</th><th>Type</th><th>Amount</th><th>Timestamp</th></tr>
<?php foreach($ledger as $tx): ?>
<tr>
    <td><?= $tx['id'] ?></td>
    <td><?= $tx['account_id'] ?></td>
    <td><?= $tx['type'] ?></td>
    <td><?= $tx['amount'] ?></td>
    <td><?= $tx['created_at'] ?></td>
</tr>
<?php endforeach; ?>
</table>

