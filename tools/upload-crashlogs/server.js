var express = require("express");
var multer  = require('multer');
var app     = express();
var nconf   = require('nconf');
var fs      = require('fs');
nconf.argv().env().file( "./config.json" );
var storage = multer.diskStorage({
    destination: function (req, file, callback) {
        callback(null, nconf.get("upload-dir"));
    },
    filename: function (req, file, callback) {
        callback(null, file.fieldname + '-' + Date.now());
    }
});

var upload = multer({
    storage : storage,
    limits: { fileSize: 512 * 1024 }
}).single('crashlog');

app.get('/',function(req,res){
    res.sendFile(__dirname + "/index.html");
});

app.post('/api/upload',function(req,res){
    upload(req,res,function(err) {
        if(err) {
            return res.end("Error uploading file.");
        }
        res.end("File is uploaded");
    });
});

app.listen(nconf.get("upload-port"),function(){
    console.log("Working on port " + nconf.get("upload-port"));
});
