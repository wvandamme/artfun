const express = require('express')



const app = express();


app.get('/api', function(request, response) {


	response.send('Hallo.');
})

app.listen(6633, function(){
	console.log('listening to port 6333.');
})