$(function () {
    $('button#gettest').bind('click', function () {
        $.getJSON($SCRIPT_ROOT + '/get-test', {
            a: $('input[name="get-input-1"]').val(),
            b: $('input[name="get-input-2"]').val()
        }, function (data) {
            alert('GET response: ' + data.result);
        });
        return false;
    });
});

$(function () {
    $('button#puttest').bind('click', function () {
        $.ajax({
            url: $SCRIPT_ROOT + '/put-test',
            type: 'PUT',
            contentType: 'application/json',
            dataType: "json",
            processData: false,
            data: JSON.stringify({
                a: 'test ',
                b: 'string'
            }),
            success: function (data) {
                alert('PUT response: ' + data.result);
            },
            error: function (data) {
                alert('PUT error: ' + data)
            },
        });
        return false;
    });
});

$(function () {
    const forms = ['#posttestform', '#triangleform'];

    for (const formName of forms) {
        let form = $(formName);

        form.submit(function (e) {
            $.ajax({
                type: form.attr('method'),
                url: form.attr('action'),
                data: form.serialize(),
                success: function (data) {
                    alert('POST response: ' + data.result)
                },
                error: function (data) {
                    alert('POST error: ' + data.result)
                },
            });

            return false;
        });
    }
});
