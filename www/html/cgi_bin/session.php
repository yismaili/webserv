<?php
session_start(); // start the session

if (isset($_SESSION['visitedaa'])) {
    // the user has visited this page before
    $username = $_SESSION["visitedaa"];
    echo "Welcome back, $username!";
} else {
    // the user is visiting this page for the first time
    echo "first time here!";
    $_SESSION['visitedaa'] = true; // set the 'visited' variable to true
}
?>