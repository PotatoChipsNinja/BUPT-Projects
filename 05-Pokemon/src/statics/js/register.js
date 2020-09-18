function submit() {
    var username = $("#input-username").val();
    var password = $("#input-password").val();
    var repeat = $("#input-repeat").val();
    if (username == "") {
        $("#err-text").text("用户名不能为空");
        $("#err-info").removeAttr("hidden");
    } else if (password != repeat) {
        $("#err-text").text("两次输入的密码不一致");
        $("#err-info").removeAttr("hidden");
    } else if (password == "") {
        $("#err-text").text("密码不能为空");
        $("#err-info").removeAttr("hidden");
    } else {
        $.get("/api/register", { userName: username, password: password }, function(data) {
            if (data.result) {
                $("#reg-success").modal("show");
                setTimeout(function() {
                    $("#reg-success").modal("hide");
                    window.location.href = "/login";
                }, 1500);
            } else {
                $("#err-text").text("您输入的用户名已被占用");
                $("#err-info").removeAttr("hidden");
            }
        }, "json");
    }
}

function clearErrInfo() {
    $("#err-info").attr("hidden", "hidden");
    $("#err-text").text("");
}