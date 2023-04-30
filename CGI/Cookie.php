<?php
// $cookie_name = "user";
// $cookie_value = "John";
$cookie_name = $_GET['name'];
$cookie_value = $_GET['last_name'];
setrawcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/");
// 86400 = 1 day
?>
<html>
<body>

<?php
echo "Cookie is set.";
?>

</body>
</html>
?>

