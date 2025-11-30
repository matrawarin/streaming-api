<?php
session_start();
if (!isset($_SESSION['uid'])) die("Login required");
?>

<!DOCTYPE html>
<html>
<head>
    <title>Private Chat</title>
    <script src="https://code.jquery.com/jquery-3.7.1.min.js"></script>
</head>

<body>

<h2>Private Chat</h2>

To User ID:
<input id="to_id" value="">
<br><br>

<div id="pchat_box" style="width:400px;height:300px;border:1px solid #aaa;overflow:auto;"></div>

<input id="pmsg" placeholder="พิมพ์ข้อความ..." style="width:300px;">
<button id="psend">ส่ง</button>

<script src="js/chat.js"></script>
<script>
startPrivateChat(<?=$_SESSION['uid']?>);
</script>
</body>
</html>

