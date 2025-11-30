<?php
include 'config.php';

// --- Add or Update KYC ---
if(isset($_POST['submit'])) {
    $account_id = intval($_POST['account_id']);
    $full_name = $_POST['full_name'];
    $national_id = $_POST['national_id'];
    $address = $_POST['address'];
    $phone = $_POST['phone'];
    $email = $_POST['email'];

    $stmt = $pdo->prepare("
        INSERT INTO kyc (account_id, full_name, national_id, address, phone, email)
        VALUES (?, ?, ?, ?, ?, ?)
        ON DUPLICATE KEY UPDATE full_name=?, national_id=?, address=?, phone=?, email=?
    ");
    $stmt->execute([$account_id, $full_name, $national_id, $address, $phone, $email,
                    $full_name, $national_id, $address, $phone, $email]);

    echo "KYC information saved successfully!";
}

// --- List KYC ---
$stmt = $pdo->query("SELECT * FROM kyc");
$kyc_list = $stmt->fetchAll(PDO::FETCH_ASSOC);
?>

<h2>KYC Management</h2>

<!-- Add/Update Form -->
<form method="post">
    <input type="number" name="account_id" placeholder="Account ID" required>
    <input type="text" name="full_name" placeholder="Full Name" required>
    <input type="text" name="national_id" placeholder="National ID" required>
    <input type="text" name="address" placeholder="Address" required>
    <input type="text" name="phone" placeholder="Phone" required>
    <input type="email" name="email" placeholder="Email" required>
    <button type="submit" name="submit">Save KYC</button>
</form>

<!-- KYC Table -->
<table border="1">
<tr>
    <th>Account ID</th>
    <th>Full Name</th>
    <th>National ID</th>
    <th>Address</th>
    <th>Phone</th>
    <th>Email</th>
</tr>
<?php foreach($kyc_list as $k): ?>
<tr>
    <td><?= $k['account_id'] ?></td>
    <td><?= htmlspecialchars($k['full_name']) ?></td>
    <td><?= htmlspecialchars($k['national_id']) ?></td>
    <td><?= htmlspecialchars($k['address']) ?></td>
    <td><?= htmlspecialchars($k['phone']) ?></td>
    <td><?= htmlspecialchars($k['email']) ?></td>
</tr>
<?php endforeach; ?>
</table>

