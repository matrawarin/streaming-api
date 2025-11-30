<?php
session_start();
if (!isset($_SESSION['uid'])) die("Login required");
?>

<!DOCTYPE html>
<html>
<head>
    <title>Group Chat</title>
    <script src="https://code.jquery.com/jquery-3.7.1.min.js"></script>
</head>

<body>
<h2>Group Chat (ห้องรวม)</h2>
<div id="chat_box" style="width:400px;height:300px;border:1px solid #aaa;overflow:auto;"></div>

<input id="msg" placeholder="พิมพ์ข้อความ..." style="width:300px;">
<button id="send">ส่ง</button>

<script src="js/chat.js"></script>
<script>
startGroupChat(<?=$_SESSION['uid']?>);
</script>
</body>
</html>

