let selected = null;

function Log(text) {
    let textDiv = document.createElement("div");
    textDiv.appendChild(document.createTextNode(text));

    textDiv.classList.add("log-line");

    document.getElementById("commandLog").appendChild(textDiv);
}

function getFormData(form) {
    let unindexed_array = form.serializeArray();
    let indexed_array = {};

    $.map(unindexed_array, function (n, i) {
        indexed_array[n['name']] = n['value'];
    });

    return indexed_array;
}

$(function () {
    /*
    $('button#gettest').bind('click', function () {
        $.getJSON($SCRIPT_ROOT + '/get-test', {
            a: $('input[name="get-input-1"]').val(),
            b: $('input[name="get-input-2"]').val()
        }, function (data) {
            alert('GET response: ' + data.result);
        });
        return false;
    });
    */

    const forms = ['#triangle-add', '#text-add'];

    for (const formName of forms) {
        let form = $(formName);

        form.submit(function (e) {
            $.ajax({
                type: form.attr('method'),
                url: form.attr('action'),
                data: JSON.stringify(getFormData(form)),
                contentType: 'application/json;charset=UTF-8',
                success: function (data) {
                    Log("POST " + formName + ": " + data['id']);
                    AddElement(data);
                },
                error: function (data) {
                    Log("POST error " + formName + ": " + data);
                },
            });

            return false;
        });
    }

    function AddElement(e) {
        // create new element
        let entry = document.createElement("div");
        console.log(e);
        entry.appendChild(document.createTextNode(e["type"] + e["id"]));

        // setup element
        entry.classList.add("log-line");
        entry.embedded = e;
        entry.onclick = function (event) {
            if (selected) {
                selected.classList.remove("selected");
            }
            selected = this;
            this.classList.add("selected");
        }
        entry.onclick();

        // append
        document.getElementById("selector").appendChild(entry);
    }
});

function DeleteSelected() {
    if (selected) {
        $.ajax({
            url: /*$SCRIPT_ROOT +*/ '/delete',
            type: 'PUT',
            contentType: 'application/json',
            dataType: "json",
            processData: false,
            data: JSON.stringify({
                type: '_delete',
                id: selected.embedded["id"]
            }),
            success: function (data) {
                Log('delete: ' + data.result);
                selected.parentNode.removeChild(selected);
                selected = null;
            },
            error: function (data) {
                Log('delete error: ' + data)
            },
        });
    }
}
