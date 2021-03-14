const fs = require("fs")


fs.readFile("./csv_Data/fullDay-f.json",'utf-8',(err, data)=>{
    var jsonarr = JSON.parse(data)
    console.log("ammount of data in fullday-f.json",jsonarr.length);
    
    

})
