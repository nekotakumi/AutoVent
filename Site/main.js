var outTemp = 0;
var inTemp = 0;
var setTemp = 0;
var windowStatus = 0;
// Update this URL to the thingspeak server
var feedURL = "https://api.thingspeak.com/channels/985815/feeds.json?results=2";

function getFromServer() {
    fetch(feedURL)
        .then((response) => {
            return response.json();
        })
        .then((myJson) => {
            console.log("feed");
            console.log(myJson);

            var feedStream = myJson.feeds[0];

            outTemp = feedStream.field1;
            inTemp = feedStream.field2;
            setTemp = feedStream.field3;
            windowStatus = feedStream.field4;
        });
    update();
}

function update() {
    document.getElementById("outTemp").innerHTML = "Outside Temp: " + outTemp + "F";
    document.getElementById("roomTemp").innerHTML = "Inside Temp: " + inTemp + "F";
    document.getElementById("setTemp").innerHTML = "Target Room Temp: " + setTemp + "F";
    document.getElementById("windowStatus").innerHTML = "Window Status: " + (windowStatus == 1 ? "Open" : "Closed");
}

// Increments all values and toggles window status
function test() {
    outTemp++;
    inTemp++;
    setTemp++;
    if(windowStatus == -1){
        windowStatus = 1;
    }
    else if(windowStatus == 0){
        windowStatus = 1;
    }
    else if(windowStatus == 1){
        windowStatus = 0;
    }
    else {
        console.log("Invalid value in windowStatus, resetting to 0.");
        windowStatus = 0;
    }
    update();
}