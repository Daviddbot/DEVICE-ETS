<!DOCTYPE html>
<html>
<head>
	<title>IoT Project</title>
	<meta http-equiv="refresh" content="60"> 
	<style>
		table {
			border-collapse: collapse;
			width: 100%;
			margin-top: 20px;
			align-items:center;
		}

		th, td {
			text-align: center;
			padding: 8px;
			border: 1px solid black;
		}

		th {
			background-color: #4CAF50;
			color: white;
		}
	</style>
</head>
<body>
	<header>
		<h1>IoT Project</h1>
	</header>
	<main>
		<form action="insert.php" method="get">

			<br>
			
			
			<br>
			
		</form>

		<?php

			$dbname = 'example';
			$dbuser = 'root';  
			$dbpass = ''; 
			$dbhost = 'localhost'; 

			$connect = @mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

			if(!$connect){
				echo "Error: " . mysqli_connect_error();
				exit();
			}

			$temperature = $_GET["temperature"];
			$humidity = $_GET["humidity"]; 

			$query = "INSERT INTO iot_project (temperature, humidity) VALUES ('$temperature', '$humidity')";
			$result = mysqli_query($connect,$query);

			if ($result) {
				echo "<p>Insertion Success!</p>";
			}

			$query = "SELECT * FROM iot_project";
			$result = mysqli_query($connect,$query);

			if (mysqli_num_rows($result) > 0) {
				echo "<table>";
				echo "<tr><th>Timestamp</th><th>Temperature</th><th>Humidity</th></tr>";
				while($row = mysqli_fetch_assoc($result)) {
					echo "<tr><td>".$row["created_at"]."</td><td>".$row["temperature"]."</td><td>".$row["humidity"]."</td></tr>";
				}
				echo "</table>";
			} else {
				echo "<p>No data available</p>";
			}
//u9!Mt}FF7xo#Rtvf
			mysqli_close($connect);
		?>
	</main>
</body>
</html>
