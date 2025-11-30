<?php
include 'config.php';

// --- Create Code ---
if(isset($_POST['create'])) {
    $name = $_POST['name'];
    $stmt = $pdo->prepare("INSERT INTO code (name) VALUES (?)");
    $stmt->execute([$name]);
    echo "Code created successfully!";
}

// --- Delete Code ---
if(isset($_GET['delete'])) {
    $id = intval($_GET['delete']);
    $stmt = $pdo->prepare("DELETE FROM code WHERE id=?");
    $stmt->execute([$id]);
    echo "Code deleted successfully!";
}

// --- Update Code ---
if(isset($_POST['update'])) {
    $id = intval($_POST['id']);
    $name = $_POST['name'];
    $stmt = $pdo->prepare("UPDATE code SET name=? WHERE id=?");
    $stmt->execute([$name, $id]);
    echo "Code updated successfully!";
}

// --- List Codes ---
$stmt = $pdo->query("SELECT * FROM code");
$codes = $stmt->fetchAll(PDO::FETCH_ASSOC);
?>

<h2>Codes</h2>

<!-- Create Form -->
<form method="post">
    <input type="text" name="name" placeholder="Code Name" required>
    <button type="submit" name="create">Create Code</button>
</form>

<!-- Codes Table -->
<table border="1">
<tr><th>ID</th><th>Name</th><th>Actions</th></tr>
<?php foreach($codes as $c): ?>
<tr>
    <td><?= $c['id'] ?></td>
    <td><?= htmlspecialchars($c['name']) ?></td>
    <td>
        <!-- Edit Form -->
        <form method="post" style="display:inline;">
            <input type="hidden" name="id" value="<?= $c['id'] ?>">
            <input type="text" name="name" value="<?= htmlspecialchars($c['name']) ?>" required>
            <button type="submit" name="update">Update</button>
        </form>
        <!-- Delete Link -->
        <a href="?delete=<?= $c['id'] ?>" onclick="return confirm('Delete this code?');">Delete</a>
    </td>
</tr>
<?php endforeach; ?>
</table>

