var express = require('express');
var app = express();

app.get('/', function (req, res) {
  console.log("REQUEST RECEIVED", Date.now());
  res.send('Hello World!' + Date.now());
});

var server = app.listen(3000, function () {
  var host = server.address().address;
  var port = server.address().port;

  console.log('\n\nExample app listening at http://%s:%s', host, port);
});