<?php
if(isset($_POST['submit'])){
    $target_dir = "uploads/";
    $target_file = $target_dir . basename($_FILES["face"]["name"]);
    move_uploaded_file($_FILES["face"]["tmp_name"], $target_file);

    // ใช้ Python OpenCV สำหรับตรวจใบหน้า (ง่ายกว่า PHP)
    $cmd = "python3 face_detect.py " . escapeshellarg($target_file);
    $output = shell_exec($cmd);
    echo nl2br($output);
}
?>

<form method="post" enctype="multipart/form-data">
<input type="file" name="face" required>
<button name="submit">Upload & Detect</button>
</form>

