<?php
// Check if the username cookie is set
if(isset($_COOKIE["username"])) {
    $username = $_COOKIE["username"];
    echo "Welcome back, $username!";
} else {
    // Set the username cookie
    $username = "webserver";
    setcookie("username", $username, time() + (86400 * 30)); // Expires in 30 days
    echo "Hello, we've set a cookie for you!";
}
?>