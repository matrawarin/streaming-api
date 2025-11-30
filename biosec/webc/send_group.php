<?php
session_start();
include "../db.php";

$msg = $_POST['msg'];
$uid = $_SESSION['uid'];

$stmt = $pdo->prepare("INSERT INTO chat_group (user_id, message) VALUES (?,?)");
$stmt->execute([$uid, $msg]);

echo "OK";

