#!/usr/bin/php-cgi
<?php
$name = $_POST['name'];
$email = $_POST['email'];
echo "Content-Type: text/html\n\n";
echo "<html><body>";
echo "<h1>Hello, $name!</h1>";
echo "<p>Your email is: $email</p>";
echo "</body></html>";
?>