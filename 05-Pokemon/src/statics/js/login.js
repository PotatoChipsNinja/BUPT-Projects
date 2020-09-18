function submit() {
    var username = $("#input-username").val();
    var password = $("#input-password").val();
    if (username == "") {
        $("#err-text").text("用户名不能为空");
        $("#err-info").removeAttr("hidden");
    } else if (password == "") {
        $("#err-text").text("密码不能为空");
        $("#err-info").removeAttr("hidden");
    } else {
        $.get("/api/logIn", { userName: username, password: password }, function(data) {
            if (data.result) {
                $("#login-success").modal("show");
                setTimeout(function() {
                    $("#login-success").modal("hide");
                    document.cookie = "username=" + username + "; path=/";
                    window.location.href = "/";
                }, 1500);
            } else {
                $("#err-text").text("用户名或密码错误");
                $("#err-info").removeAttr("hidden");
            }
        }, "json");
    }
}

function clearErrInfo() {
    $("#err-info").attr("hidden", "hidden");
    $("#err-text").text("");
}