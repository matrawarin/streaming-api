function startGroupChat(uid) {
    $("#send").click(function () {
        $.post("api/send_group.php", { msg: $("#msg").val() }, function () {
            $("#msg").val("");
        });
    });

    setInterval(function () {
        $.getJSON("api/load_group.php", function (data) {
            $("#chat_box").html("");
            $.each(data, function (_, row) {
                $("#chat_box").append("<div><b>" + row.username + ":</b> " + row.message + "</div>");
            });
        });
    }, 1500);
}

function startPrivateChat(uid) {
    $("#psend").click(function () {
        $.post("api/send_private.php", {
            msg: $("#pmsg").val(),
            to_id: $("#to_id").val()
        }, function () {
            $("#pmsg").val("");
        });
    });

    setInterval(function () {
        $.getJSON("api/load_private.php?to_id=" + $("#to_id").val(), function (data) {
            $("#pchat_box").html("");
            $.each(data, function (_, row) {
                $("#pchat_box").append("<div><b>User " + row.sender_id + ":</b> " + row.message + "</div>");
            });
        });
    }, 1500);
}

