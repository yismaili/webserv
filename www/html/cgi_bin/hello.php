<?php
echo "<html>";
echo "<head>";
echo "<title>Test</title>";
echo "</head>";
echo "<body>";
echo "<div style='text-align: center; margin-top: 100px;'>";
if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    echo "<h1>Hello " . $_GET["first_name"] . " " . $_GET["last_name"] . "</h1>";
} else {
    echo "<h1>Hello " . $_POST["first_name"] . " " . $_POST["last_name"] . "</h1>";
}
echo "<p>Thank you for using our service!</p>";
echo "<a href='/'>Go back</a>";
echo "</div>";
echo "</body>";
echo "</html>";
?>