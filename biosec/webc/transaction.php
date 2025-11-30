<?php
include 'config.php';

// Deposit or Withdraw
if(isset($_POST['submit'])) {
    $account_id = $_POST['account_id'];
    $type = $_POST['type'];
    $amount = floatval($_POST['amount']);

    // ดึงยอดบัญชี
    $stmt = $pdo->prepare("SELECT balance FROM accounts WHERE id=?");
    $stmt->execute([$account_id]);
    $balance = $stmt->fetchColumn();

    if($type == 'DEPOSIT') {
        $new_balance = $balance + $amount;
    } elseif($type == 'WITHDRAW') {
        if($amount > $balance) { echo "Insufficient balance!"; exit; }
        $new_balance = $balance - $amount;
    }

    // อัปเดต balance
    $stmt = $pdo->prepare("UPDATE accounts SET balance=? WHERE id=?");
    $stmt->execute([$new_balance, $account_id]);

    // เพิ่ม ledger
    $stmt = $pdo->prepare("INSERT INTO ledger (account_id, type, amount) VALUES (?, ?, ?)");
    $stmt->execute([$account_id, $type, $amount]);

    echo "Transaction completed!";
}
?>

