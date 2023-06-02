var express = require('express');
var env = require('dotenv').config()
var ejs = require('ejs');
var path = require('path');
var app = express();
var bodyParser = require('body-parser');
var mongoose = require('mongoose');
var session = require('express-session');
var MongoStore = require('connect-mongo')(session);

const DB_URI = process.env.DB_URI
mongoose.connect(DB_URI,{ useNewUrlParser: true, useUnifiedTopology: true})  
  .then((result) => console.log(result))
  .catch((err) => console.log(err));

  var db = mongoose.connection;
  db.on('error', console.error.bind(console, 'connection error:'));
  db.once('open', function () {
  });
  
  app.use(session({
    secret: 'work hard',
    resave: true,
    saveUninitialized: false,
    store: new MongoStore({
      mongooseConnection: db
    })
  }));
  
  app.set('views', path.join(__dirname, './page/view'));
  app.set('view engine', 'ejs');	
  
  app.use(bodyParser.json());
  app.use(bodyParser.urlencoded({ extended: false }));
  
  app.use(express.static(__dirname + './page/view'));
  
  var index = require('./page/pageroute');
  app.use('/', index);
  
  // catch 404 and forward to error handler
  app.use(function (req, res, next) {
    var err = new Error('File Not Found');
    err.status = 404;
    next(err);
  });
  
  // error handler
  // define as the last app.use callback
  app.use(function (err, req, res, next) {
    res.status(err.status || 500);
    res.send(err.message);
  });
  
  
  const PORT = process.env.PORT || 3000;
  app.listen(PORT, function () {
    console.log('Server is started on port:'+PORT);
  });