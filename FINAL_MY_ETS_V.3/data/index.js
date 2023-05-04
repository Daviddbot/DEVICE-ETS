    setInterval(function() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("latitude").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/latitude", true);
      xhttp.send();
    }, 1000);

    setInterval(function() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("longitude").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/longitude", true);
      xhttp.send();
    }, 1000);

    setInterval(function() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4) {
          if (this.status == 200) {
            document.getElementById("server-status").innerHTML = "Connected";
          } else {
            document.getElementById("server-status").innerHTML = "Not Connected";
          }
        }
      };
      xhttp.open("GET", "/check-connection", true);
      xhttp.send();
    }, 1000);
    function showIP() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("ip-address").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "/get-ip-address", true);
        xhttp.send();
