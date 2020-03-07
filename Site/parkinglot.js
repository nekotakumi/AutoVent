var full = 0;

function updateSpots() {
    // reset spot counter before reading again
    full = 0;
    var feedURL = "https://api.thingspeak.com/channels/985815/feeds.json?results=2";

    fetch(feedURL)
        .then((response) => {
            return response.json();
        })
        .then((myJson) => {
            console.log("feed");
            console.log(myJson);

            var feedStream = myJson.feeds[0];

            if (feedStream.field1 == 0) {
                // apply green color
                document.getElementById("onePic").style.display = "none";
                full++;
            } else if (feedStream.field1 == 1) {
                // apply red color
                document.getElementById("onePic").style.display = "block";
            } else {
                console.err("Invalid response from server.");
            }

            if (feedStream.field2 == 0) {
                // apply green color
                document.getElementById("twoPic").style.display = "none";
                full++;
            } else if (feedStream.field2 == 1) {
                // apply red color
                document.getElementById("twoPic").style.display = "block";
            } else {
                console.err("Invalid response from server.");
            }

            if (feedStream.field3 == 0) {
                // apply green color
                document.getElementById("threePic").style.display = "none";
                full++;
            } else if (feedStream.field3 == 1) {
                // apply red color
                document.getElementById("threePic").style.display = "block";
            } else {
                console.err("Invalid response from server.");
            }

            updateFull();
        });
}

function updateFull() {
    document.getElementById("spots_left").innerHTML = "There are " + full + " spots available.";
}

function testFunction() {
    document.getElementById("onePic").style.display = "block";
}