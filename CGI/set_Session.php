<?php
// Start the session for set it in the browser
session_start();

// Check if the session variable is set in the browser
if (isset($_SESSION['username'])) {
  // The session variable is set 
  echo "Welcome back, " . $_SESSION['username'] . "!";
} else {
  // The session variable is not set
  echo "Please log in to access this page.";
}
?>