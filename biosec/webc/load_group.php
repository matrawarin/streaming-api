<?php
include "../db.php";

$stmt = $pdo->query("SELECT chat_group.*, users.username 
                     FROM chat_group 
                     LEFT JOIN users ON users.id = chat_group.user_id
                     ORDER BY chat_group.id DESC 
                     LIMIT 50");

$data = $stmt->fetchAll(PDO::FETCH_ASSOC);

echo json_encode(array_reverse($data));

