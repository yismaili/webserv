<?php
$cookies = array(
    "lang" => "en",
    "USER" => "ADMIN"
);
foreach ($cookies as $name => $value) {
    setcookie($name, $value, time() - (86400 * 30), "/");
}

?>

<html>

<body>
	<?php
	foreach ($cookies as $name => $value) {
		if (!isset($_COOKIE[$name]))
			echo "<h3>Cookie = '" . $name . "' is not  set!</h3><br>";
		else {
			echo "<h3>Cookie = '" . $name . "' is set!</h3><br>";
			echo "<h4>Value is: " . $_COOKIE[$name] . "</h4>";
		}
	}
	?>
</body>

</html>