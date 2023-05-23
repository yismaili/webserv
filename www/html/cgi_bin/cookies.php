<?php
// Check if the username cookie is set
if(isset($_COOKIE["username"])) {
    $username = $_COOKIE["username"];
    echo "Welcome back, $username!";
} else {
    // Set the username cookie
    $username = "webserver";
    setcookie("username", $username, time() + 1); 
    echo "Hello, $username we've set a cookie for you!";
}
?>

