<?php
session_start(); // start the session

if (isset($_SESSION['visited'])) {
    // the user has visited this page before
    echo "Welcome back!";
} else {
    // the user is visiting this page for the first time
    echo "first time here!";
    $_SESSION['visited'] = true; // set the 'visited' variable to true
}

?>