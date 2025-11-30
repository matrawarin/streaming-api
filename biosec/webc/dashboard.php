<?php
// simple gallery with text + image (file or url)
?>
<!doctype html>
<html>
<head><meta charset="utf-8"><title>Upload Text + Image</title>
<style>
.gallery{display:flex;flex-wrap:wrap;gap:10px}
.card{width:200px;border:1px solid #ddd;padding:6px}
.card img{width:100%;height:120px;object-fit:cover}
</style>
</head>
<body>
<h2>เพิ่มรายการ (ข้อความ + รูปภาพ)</h2>
<form id="frm" method="post" action="api.php" enctype="multipart/form-data">
    ข้อความ: <input type="text" name="text" required><br>
    รูปจากไฟล์: <input type="file" name="imgfile"><br>
    หรือ URL: <input type="url" name="imgurl" placeholder="https://..."><br>
    <button type="submit">บันทึก</button>
</form>

<hr>
<h3>รายการ</h3>
<div class="gallery">
<?php
$db = new PDO('sqlite:'.__DIR__.'/data.db');
$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
$db->exec("CREATE TABLE IF NOT EXISTS items (id INTEGER PRIMARY KEY, text TEXT, img TEXT)");
$stmt = $db->query("SELECT * FROM items ORDER BY id DESC");
foreach($stmt->fetchAll(PDO::FETCH_ASSOC) as $r){
    $img = htmlspecialchars($r['img']);
    echo "<div class='card'><img src='". $img ."' onerror=\"this.src='noimg.png'\"><div>".htmlspecialchars($r['text'])."</div></div>";
}
?>
</div>
</body>
</html>

