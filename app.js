const express = require("express");
const app = express();
const fs = require("fs");
const bodyParser = require("body-parser");
const converter = require("json-2-csv");

app.use(bodyParser.json());
app.get("/", (req, res) => {
    console.log("Hello");
    res.end();
});
app.get("/jsons",(req, res)=>{
    fs.readdir("./csv_Data/",(err, files)=>{
        res.send(files)
    })
})
app.get("/jsons/:file",(req, res)=>{
    if(req.params.file.includes(".json"))
    fs.readFile(`./csv_Data/${req.params.file}`,"utf8",(err, json)=>{
        if(err) res.status(404).send(`no such file, ${err.code}`)
        res.status(200).send(json)
    })
    else res.status(403).send("not a json file")
})
app.get("/jsons/:file/download", (req, res)=>{
    if(req.params.file.includes(".json"))
    fs.readFile(`./csv_Data/${req.params.file}`,"utf8",(err, json)=>{
        if(err) res.status(404).send(`no such file, ${err.code}`)
        res.download(`./csv_Data/${req.params.file}`, req.params.file);
    })
    else res.status(403).send("not a json file")
})

app.get("/json2csv/:jsonfile",(req,res)=>{})
app.get("/json2geojson/:jsonfile",(req,res)=>{})



app.post("/gps", (req, res) => {
    console.log(req.body);
    fs.readFile("./csv_Data/day2.json", "utf8", function (err, data) {
        if (err) console.error(err.code, ":|:", err.message);
        if (data) {
            var obj = JSON.parse(data);
            obj.push(req.body);
        } else {
            console.log("new file created");
            var obj = [];
            obj.push(req.body);
        }
        var gpsData = JSON.stringify(obj);
        fs.writeFile("./csv_Data/day2.json", gpsData, function (err) {
            if (err) return console.log(err);
            console.log("data added");
        });
    });
    res.status(200).send("OK");
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server started at http://localhost:${PORT}....`);
});