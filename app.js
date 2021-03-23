const express = require("express");
const app = express();
const fs = require("fs");
const bodyParser = require("body-parser");
const converter = require("json-2-csv");

let track = {};

app.use(bodyParser.json());
app.get("/", (req, res) => {
    console.log("Hello");
    res.end();
});

app.get("/jsons", (req, res) => {
    fs.readdir("./csv_Data/", (err, files) => {
        res.send(files);
    });
});

app.get("/jsons/:file", (req, res) => {
    if (req.params.file.includes(".json"))
        fs.readFile(`./csv_Data/${req.params.file}`, "utf8", (err, json) => {
            if (err) res.status(404).send(`no such file, ${err.code}`);
            res.status(200).send(json);
        });
    else res.status(403).send("not a json file");
});

app.get("/jsons/:file/download", (req, res) => {
    if (req.params.file.includes(".json"))
        fs.readFile(`./csv_Data/${req.params.file}`, "utf8", (err, json) => {
            if (err) res.status(404).send(`no such file, ${err.code}`);
            res.download(`./csv_Data/${req.params.file}`, req.params.file);
        });
    else res.status(403).send("not a json file");
});

app.post("/gps", (req, res) => {
    let payloadData = {
        GPS: JSON.parse(req.body.GPS),
        BMP: JSON.parse(req.body.BMP),
        DHT11: JSON.parse(req.body.DHT11),
    };
    // let fileName = req.params.fileName;
    track = payloadData;
    console.log(payloadData);
    fs.readFile(`./csv_Data/updated.json`, "utf8", function (err, data) {
        if (err) console.error(err.code, ":|:", err.message);
        if (data) {
            var obj = JSON.parse(data);
            obj.push(payloadData);
        } else {
            console.log("new file created");
            var obj = [];
            obj.push(payloadData);
        }
        var gpsData = JSON.stringify(obj);
        fs.writeFile("./csv_Data/updated.json", gpsData, function (err) {
            if (err) return console.log(err);
            console.log("data added");
        });
    });
    res.status(200).send("OK");
});

app.get("/gps/live", (req, res) => {
    res.send(track);
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server started at http://localhost:${PORT}....`);
});
