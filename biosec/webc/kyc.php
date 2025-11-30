<?php
include 'config.php';

// Add or Update KYC
if(isset($_POST['submit'])) {
    $account_id = $_POST['account_id'];
    $full_name = $_POST['full_name'];
    $national_id = $_POST['national_id'];
    $address = $_POST['address'];
    $phone = $_POST['phone'];
    $email = $_POST['email'];

    $stmt = $pdo->prepare("INSERT INTO kyc (account_id, full_name, national_id, address, phone, email)
        VALUES (?, ?, ?, ?, ?, ?)
        ON DUPLICATE KEY UPDATE full_name=?, national_id=?, address=?, phone=?, email=?");
    $stmt->execute([$account_id,$full_name,$national_id,$address,$phone,$email,$full_name,$national_id,$address,$phone,$email]);
    echo "KYC updated successfully!";
}

// List KYC
$stmt = $pdo->query("SELECT * FROM kyc");
$kyc_list = $stmt->fetchAll(PDO::FETCH_ASSOC);
?>
<h2>KYC</h2>
<table border="1">
<tr><th>AccountID</th><th>Full Name</th><th>NationalID</th><th>Address</th><th>Phone</th><th>Email</th></tr>
<?php foreach($kyc_list as $k): ?>
<tr>
    <td><?= $k['account_id'] ?></td>
    <td><?= $k['full_name'] ?></td>
    <td><?= $k['national_id'] ?></td>
    <td><?= $k['address'] ?></td>
    <td><?= $k['phone'] ?></td>
    <td><?= $k['email'] ?></td>
</tr>
<?php endforeach; ?>
</table>

