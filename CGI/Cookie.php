<?php
ob_start(); // Start output buffering
$username = "snoffuae";
setcookie("username", $username, time()+3600);
ob_end_flush(); // End output buffering and send output to browser
?>

<!DOCTYPE html>
<html>
<head>
	<title>Cookie Example</title>
</head>
<body>
	<?php
	if(isset($_COOKIE["username"])) {
	    // The "username" cookie is already set
	    $username = $_COOKIE["username"];
	    echo "<h1>Welcome back, $username!</h1>";
	} else {
	    // The "username" cookie is not set
	    echo "<h1>Hello, we've set a cookie for you!</h1>";
	}
	?>
</body>
</html>

