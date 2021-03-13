const fs = require("fs")

fs.readFile("./csv_Data/market.json",'utf-8',(err, data)=>{
    var jsonarr = JSON.parse(data)
    console.log("ammount of data in Day2.json",jsonarr.length);
    
    

})