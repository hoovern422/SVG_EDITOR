'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, 
      {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

///////////////////////////////////////////////////////////////////////////////

let sharedLib = ffi.Library('./parser/bin/libsvgparse', {

  'getNumSVGFiles': [ 'string', []],
  'getSVGName': [ 'string', [ 'int' ]],
  'getSVGInfo': [ 'string', [ 'string' ]]
}); 

///////////////////////////////////////////////////////////////////////////////

// Get the number of SVG files in the uploads directory
app.get('/get_num_files', function(req , res){
  let ret = sharedLib.getNumSVGFiles();
  //ret = ret.replace(/\\/g, '');
  res.send ({
    ret
  })
});

///////////////////////////////////////////////////////////////////////////////

// Get the name of a particular SVG file
app.get('/get_file_name', function(req , res){
  let ret = sharedLib.getSVGName(req.query.index);
  res.send ({
    foo: ret
  })
});

///////////////////////////////////////////////////////////////////////////////

// Get the file log panel info for a particular SVG image.
app.get('/get_file_info', function(req , res){
  let ret = sharedLib.getSVGInfo(req.query.name);
  res.send ({
    foo: ret
  })
});

///////////////////////////////////////////////////////////////////////////////

//Sample endpoint
app.get('/someendpoint', function(req , res){
  let retStr = req.query.name1 + " " + req.query.name2;
  res.send({
    foo: retStr 
  });
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

///////////////////////////////////////////////////////////////////////////////