<?php
include 'config.php';

// --- Create Section ---
if(isset($_POST['create'])) {
    $name = $_POST['name'];
    $stmt = $pdo->prepare("INSERT INTO section (name) VALUES (?)");
    $stmt->execute([$name]);
    echo "Section created successfully!";
}

// --- Delete Section ---
if(isset($_GET['delete'])) {
    $id = intval($_GET['delete']);
    $stmt = $pdo->prepare("DELETE FROM section WHERE id=?");
    $stmt->execute([$id]);
    echo "Section deleted successfully!";
}

// --- Update Section ---
if(isset($_POST['update'])) {
    $id = intval($_POST['id']);
    $name = $_POST['name'];
    $stmt = $pdo->prepare("UPDATE section SET name=? WHERE id=?");
    $stmt->execute([$name, $id]);
    echo "Section updated successfully!";
}

// --- List Sections ---
$stmt = $pdo->query("SELECT * FROM section");
$sections = $stmt->fetchAll(PDO::FETCH_ASSOC);
?>

<h2>Sections</h2>

<!-- Create Form -->
<form method="post">
    <input type="text" name="name" placeholder="Section Name" required>
    <button type="submit" name="create">Create Section</button>
</form>

<!-- Sections Table -->
<table border="1">
<tr><th>ID</th><th>Name</th><th>Actions</th></tr>
<?php foreach($sections as $sec): ?>
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
        <a href="?delete=<?= $sec['id'] ?>" onclick="return confirm('Delete this section?');">Delete</a>
    </td>
</tr>
<?php endforeach; ?>
</table>

