<?php
include 'config.php';

// --- Create Sector ---
if(isset($_POST['create'])) {
    $name = $_POST['name'];
    $stmt = $pdo->prepare("INSERT INTO sector (name) VALUES (?)");
    $stmt->execute([$name]);
    echo "Sector created successfully!";
}

// --- Delete Sector ---
if(isset($_GET['delete'])) {
    $id = intval($_GET['delete']);
    $stmt = $pdo->prepare("DELETE FROM sector WHERE id=?");
    $stmt->execute([$id]);
    echo "Sector deleted successfully!";
}

// --- Update Sector ---
if(isset($_POST['update'])) {
    $id = intval($_POST['id']);
    $name = $_POST['name'];
    $stmt = $pdo->prepare("UPDATE sector SET name=? WHERE id=?");
    $stmt->execute([$name, $id]);
    echo "Sector updated successfully!";
}

// --- List Sectors ---
$stmt = $pdo->query("SELECT * FROM sector");
$sectors = $stmt->fetchAll(PDO::FETCH_ASSOC);
?>

<h2>Sectors</h2>

<!-- Create Form -->
<form method="post">
    <input type="text" name="name" placeholder="Sector Name" required>
    <button type="submit" name="create">Create Sector</button>
</form>

<!-- Sectors Table -->
<table border="1">
<tr><th>ID</th><th>Name</th><th>Actions</th></tr>
<?php foreach($sectors as $sec): ?>
<tr>
    <td><?= $sec['id'] ?></td>
    <td><?= htmlspecialchars($sec['name']) ?></td>
    <td>
        <!-- Edit Form -->
        <form method="post" style="display:inline;">
            <input type="hidden" name="id" value="<?= $sec['id'] ?>">
            <input type="text" name="name" value="<?= htmlspecialchars($sec['name']) ?>" required>
            <button type="submit" name="update">Update</button>
        </form>
        <!-- Delete Link -->
        <a href="?delete=<?= $sec['id'] ?>" onclick="return confirm('Delete this sector?');">Delete</a>
    </td>
</tr>
<?php endforeach; ?>
</table>

