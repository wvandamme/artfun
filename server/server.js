const express = require('express')
const body_parser = require('body-parser')
const mailer = require('nodemailer');

const app = express();
app.use(body_parser.json());

app.get('/api', function(request, response){
	console.log(request);
});

app.post('/api', function(request, response){
	console.log(request.body);
});

const server = app.listen(8084, '192.168.0.190', function(){
    var host = server.address().address;
    var port = server.address().port;
    console.log('running at http://' + host + ':' + port)
});

