var switchPage = function(from, to) {
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
}

makelinks("landingpage", "software");
makelinks("landingpage", "hardware");
makelinks("landingpage", "video");
