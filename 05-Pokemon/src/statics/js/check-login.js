if (/username=/.test(document.cookie))
    var userName = /username=([^;]+)/.exec(document.cookie)[1];
else
    window.location.href = "/login";

function logOut() {
    $.get("/api/logOut", { userName: userName }, function(data) {
        if (data.result) {
            document.cookie = "username=; expires=Thu, 01 Jan 1970 00:00:00 GMT";
            window.location.href = "/login";
        }
    }, "json");
}