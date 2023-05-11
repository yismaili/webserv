<?PHP
echo "<HTML>";
echo "<HEAD>";
echo "<TITLE>Test</TITLE>";
echo "</HEAD>";
echo "<BODY>";
if ($_SERVER['REQUEST_METHOD'] == 'GET')
    echo "<H1>hello ". $_GET["first_name"] . " " . $_GET["last_name"] . "</H1>";
else
    echo "<H1>hello ". $_POST["first_name"] . " " . $_POST["last_name"] . "</H1>";
echo "</BODY>";
echo "</HTML>";
?>