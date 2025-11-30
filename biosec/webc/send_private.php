<?php
session_start();
include "../db.php";

$from = $_SESSION['uid'];
$to   = $_POST['to_id'];
$msg  = $_POST['msg'];

$stmt = $pdo->prepare("INSERT INTO chat_private (sender_id, receiver_id, message)
                       VALUES (?,?,?)");
$stmt->execute([$from, $to, $msg]);

echo "OK";

