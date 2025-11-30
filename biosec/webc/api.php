<?php
// handle upload (file or url) and save record
$db = new PDO('sqlite:'.__DIR__.'/data.db');
$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
$db->exec("CREATE TABLE IF NOT EXISTS items (id INTEGER PRIMARY KEY, text TEXT, img TEXT)");

$text = $_POST['text'] ?? '';
$uploads = __DIR__.'/uploads';
if(!is_dir($uploads)) mkdir($uploads, 0777, true);

// if file uploaded
if(!empty($_FILES['imgfile']['tmp_name'])){
    $tmp = $_FILES['imgfile']['tmp_name'];
    $name = basename($_FILES['imgfile']['name']);
    $dest = $uploads . '/' . time() . '_' . preg_replace('/[^a-zA-Z0-9._-]/','_', $name);
    move_uploaded_file($tmp, $dest);
    $imgpath = 'uploads/' . basename($dest);
} elseif(!empty($_POST['imgurl'])) {
    // store URL directly (you may want to validate)
    $imgpath = $_POST['imgurl'];
} else {
    $imgpath = 'noimg.png';
}

$stmt = $db->prepare("INSERT INTO items (text, img) VALUES (?, ?)");
$stmt->execute([$text, $imgpath]);
header("Location: index.php");

