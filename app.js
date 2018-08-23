var sys = require('util');
var net = require('net');
var mqtt = require('mqtt');
require('plotly')(sophiarivera, d3rWGnj1Q2wwXonkrJ88);

var io  = require('socket.io').listen(5000);
var client = mqtt.connect('mqtt://10.200.180.7');

var express = require('express'); 
var path = require('path');
var app = express();
app.use(express.static(path.join('/static/styles.css', 'static')));

client.on('message', function(topic, message) {
  console.log(message);
  sys.puts(topic+'='+message);
  io.sockets.in(topic).emit('mqtt',{'topic': String(topic), 'payload':String(message)});
});

io.sockets.on('connection', function (socket) {
  socket.on('subscribe', function (data) {
    console.log('Subscribing to '+data.topic);
    socket.join(data.topic);
    client.subscribe(data.topic);
  });
});