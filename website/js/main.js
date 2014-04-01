var switchPage = function(from, to) {
    if(from != "#landingpage")
		$(from).fadeOut();
	$(to).fadeIn();
}

var makelinks = function(from, to) {
    $("#bullseye_" + to).click(function() {
        switchPage("#" + from, "#" + to);
    });

    $("#back_" + to).click(function() {
        switchPage("#" + to, "#" + from);
    });
	
	$(document).keyup(function(e) {
		if (e.keyCode == 27) { 
			switchPage("#" + to, "#" + from);
		}
	});
}

makelinks("landingpage", "software");
makelinks("landingpage", "hardware");
makelinks("landingpage", "video");
makelinks("landingpage", "probleme");
makelinks("landingpage", "planung");
makelinks("landingpage", "team");
makelinks("landingpage", "downloads");
