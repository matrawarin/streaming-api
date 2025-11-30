<?php
$dir = __DIR__ . '/uploads';
$images = array_filter(scandir($dir), function($f){
    return preg_match('/\.(jpe?g|png|gif)$/i', $f);
});
?>
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Image Gallery</title>
<link rel="stylesheet" href="css/style.css">
</head>
<body>
<h1>Image Gallery</h1>
<div class="gallery">
<?php foreach($images as $img): ?>
    <a class="thumb" href="uploads/<?= urlencode($img) ?>" target="_blank">
        <img src="uploads/<?= urlencode($img) ?>" alt="<?= htmlspecialchars($img) ?>">
    </a>
<?php endforeach; ?>
</div>
</body>
</html>

