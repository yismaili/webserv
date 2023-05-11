<?php
// Set up database connection
$servername = "localhost";
$username = "your_username";
$password = "your_password";
$dbname = "your_database_name";

$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
	die("Connection failed: " . $conn->connect_error);
}

// Get form data
$title = $_POST['title'];
$about = $_POST['about'];
$image = $_FILES['image']['name'];
$temp = $_FILES['image']['tmp_name'];

// Move uploaded image to server
$upload_dir = "uploads/";
$target_file = $upload_dir . basename($image);
move_uploaded_file($temp, $target_file);

// Insert data into database
$sql = "INSERT INTO elements (title, about, image) VALUES ('$title', '$about', '$image')";

if ($conn->query($sql) === TRUE) {
	echo "Element added successfully";
} else {
	echo "Error adding element: " . $conn->error;
}

$conn->close();
?>
