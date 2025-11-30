<?php
include 'config.php';

// Create Account
if(isset($_POST['create'])) {
    $name = $_POST['name'];
    $section_id = $_POST['section_id'];
    $sector_id = $_POST['sector_id'];
    $code_id = $_POST['code_id'];

    $stmt = $pdo->prepare("INSERT INTO accounts (name, section_id, sector_id, code_id) VALUES (?, ?, ?, ?)");
    $stmt->execute([$name, $section_id, $sector_id, $code_id]);
    echo "Account created successfully!";
}

// List Accounts
$stmt = $pdo->query("SELECT * FROM accounts");
$accounts = $stmt->fetchAll(PDO::FETCH_ASSOC);
?>
<h2>Accounts</h2>
<table border="1">
<tr><th>ID</th><th>Name</th><th>Balance</th><th>Section</th><th>Sector</th><th>Code</th></tr>
<?php foreach($accounts as $acc): ?>
<tr>
    <td><?= $acc['id'] ?></td>
    <td><?= $acc['name'] ?></td>
    <td><?= $acc['balance'] ?></td>
    <td><?= $acc['section_id'] ?></td>
    <td><?= $acc['sector_id'] ?></td>
    <td><?= $acc['code_id'] ?></td>
</tr>
<?php endforeach; ?>
</table>

