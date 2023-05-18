<?php
$dir = "uploads/";
$allowed_exts = array("pdf", "mp4", "png", "jpg");

// Loop through the directory and find files with allowed extensions
$files = array();
if (is_dir($dir)) {
  if ($dh = opendir($dir)) {
    while (($file = readdir($dh)) !== false) {
      $ext = pathinfo($file, PATHINFO_EXTENSION);
      if (in_array($ext, $allowed_exts)) {
        $files[] = $file;
      }
    }
    closedir($dh);
  }
}

// Display the files in a table
echo "<table>";
foreach ($files as $file) {
  echo "<tr>";
  echo "<td><a href='$dir$file' target='_blank'>$file</a></td>";
  echo "</tr>";
}
echo "</table>";
?>
