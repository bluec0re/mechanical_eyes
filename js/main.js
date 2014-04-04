var switchPage = function(from, to) {
    if(from != "#landingpage")
		$(from).fadeOut();
	$(to).fadeIn();
};

var makelinks = function(from, to) {
    $("#bullseye_" + to).click(function() {
        switchPage("#" + from, "#" + to);
        return false;
    });

    $("#back_" + to).click(function() {
        switchPage("#" + to, "#" + from);
    });

	$(document).keyup(function(e) {
		if (e.keyCode == 27) {
			switchPage("#" + to, "#" + from);
		}
	});
};

makelinks("landingpage", "software");
makelinks("landingpage", "hardware");
makelinks("landingpage", "video");
makelinks("landingpage", "servo");
makelinks("landingpage", "probleme");
makelinks("landingpage", "planung");
makelinks("landingpage", "team");
makelinks("landingpage", "downloads");

$("section.subpage").addClass("tmp_hidden").addClass("subpage_js");
