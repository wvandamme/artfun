const express = require('express')
const body_parser = require('body-parser')
const mailer = require('nodemailer');

const app = express();
app.use(body_parser.json());

var db = { };

app.post('/api', function(request, response){
    const body = request.body;
    db[body.name] = body.state;
	for(var key in db) {
		if (db[key] != 'ok')
		{
			response.status(201).end();
			return;
		}
	}
	response.status(200).end();
});

const server = app.listen(8084, function(){ 
	const host = server.address().address;
    const port = server.address().port;
    console.log('running at http://' + host + ':' + port)

	setInterval(function() {
    	console.clear();
    	console.log('running at http://' + host + ':' + port)
    	console.log('-----------------------------------------------------------------------------')
    	for(var key in db) {
    		console.log(key + '\t' + db[key]); 
    	}
    	console.log('-----------------------------------------------------------------------------')
	}, 1000)

});

