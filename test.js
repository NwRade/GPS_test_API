const { json } = require("body-parser");
const fs = require("fs");
let highestSpeed = 0;

fs.readFile("./csv_Data/updated.json", "utf-8", (err, data) => {
    // console.log(data);
    data = JSON.parse(data);
    // let index = jsonarr.length;
    // for (let i = 0; i < index; i++) {
    //     if (highestSpeed < jsonarr[i].GPS.speed) {
    //         highestSpeed = jsonarr[i].GPS.speed;
    //     }
    //     console.log(jsonarr[i].GPS.speed, ": : :", highestSpeed);
    //     // console.log(jsonarr[i].GPS.speed);
    // }
    jsonarr.foreach((element) => {
        console.log(element.GPS.speed);
    });
    // // console.log(typeof jsonarr);
    // console.log("ammount of data in updated.json", jsonarr.length);
    // console.log("Highest Speed is: ", highestSpeed);
});
