<?php
session_start();
include "../db.php";

$me  = $_SESSION['uid'];
$to  = $_GET['to_id'];

$stmt = $pdo->prepare("
SELECT * FROM chat_private 
WHERE (sender_id=? AND receiver_id=?) 
   OR (sender_id=? AND receiver_id=?)
ORDER BY id DESC LIMIT 50
");

$stmt->execute([$me, $to, $to, $me]);

$data = $stmt->fetchAll(PDO::FETCH_ASSOC);

echo json_encode(array_reverse($data));

