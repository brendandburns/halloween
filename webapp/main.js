var detections = 0;
function poll() {
    $.ajax("/api").done((response) => {
        var obj = JSON.parse(response);
        if (obj.detections && obj.detections != detections) {
            detections = obj.detections;
            $("#detections").html("" + obj.detections);
            $("#state").html("" + obj.state);
            var audio = document.getElementById("audio");
            audio.currentTime = 0;
            audio.play();
        }
    });
}
setInterval(poll, 100);