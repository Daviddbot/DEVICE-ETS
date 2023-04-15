<?php
$host = "localhost";
$user = "root";
$pass = "";
$db = "example2";




     //  Connect to MySQL server
$conn = new mysqli($host, $user, $pass, $db);
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

        // Retrieve parameters
$tagId = $_GET['tagId'];
$latitude = $_GET['latitude'];
$longitude = $_GET['longitude'];

        // Insert data into MySQL database
$sql = "INSERT INTO data (tagId, latitude, longitude) VALUES ('$tagId', '$latitude', '$longitude')";
if ($conn->query($sql) === TRUE) {
  echo "New record created successfully";
} else {
  echo "Error: " . $sql . "<br>" . $conn->error;
}

     // Close MySQL connection
$conn->close();
?>
<!-- w0B<tX|S>^xHXAU&  sandi dbhost -->

