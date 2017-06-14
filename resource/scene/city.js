function generateScene() {
    var file = "";
    for (var i = 0; i < 20; i++) {
        for (var j = 0; j < 20; j++) {
            
            var factor = Math.pow(i - 9.5, 2) + Math.pow(j - 9.5, 2);
            var y = 5.0 / factor * Math.random();

            file += "pushtransf\n";
            file += "translate " + (i * 0.2 - 2) + " 0 " + (j * 0.2 - 2) + "\n";
            file += "scale " + 0.1 * Math.random() + " " + y + " " + 0.1 * Math.random() + "\n";
            file += "translate 0 " + y / 2 + " 0\n"
            file += "object cube.obj\n";
            file += "poptransf\n";
        }
    }
    return file;
}

var fs = require("fs");
fs.writeFileSync("city.scene", generateScene());

