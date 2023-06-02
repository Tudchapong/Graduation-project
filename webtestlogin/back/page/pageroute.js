var express = require('express');
const app = express()
const dotenv = require('dotenv')
var router = express.Router();
const axios = require('axios');
var User = require('../dataSchema/actSchema');
var UserInfo = require('../dataSchema/userdataSchema');
const { config } = require('dotenv');
var personInfo;
var show;
let checkelement = 0
let spo2 = []
let bpm = []
let pi = []
let spo2_a = 0
let bpm_a = 0
let pi_a = 0

router.use(express.json())
router.use(express.urlencoded({ extended: true }))
router.use((req, res, next)=>{
  res.header('Access-Control-Allow-Origin', '*')
  res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept')
  next()
})

function getApi(){
	
	var timestamp = Date.now()
    var endtime = timestamp+60000

	let a_data
	let payload = {
	  username: process.env.username,
	  password: process.env.password
	};

	let axiosConfig = {
	  headers: {
		  'Content-Type': 'application/json',
		  "Access-Control-Allow-Origin": "*",
	  }
	};

	axios.post(process.env.connexthingsAuth, payload, axiosConfig)
	.then((res)=>{
	  a_data = res.data.token
	  let config = {
		headers: { 'Authorization' : `Bearer ${a_data}`,
					'Content-Type': 'application/json',
					"Access-Control-Allow-Origin": "*",
		}
	  };
	  setInterval(()=>
          axios.get(process.env.connexthingsAPI+timestamp+"&&end="+endtime+"&&interval=3000", config)
          .then((data)=>{
            let spo2_s = 0
            let bpm_s = 0
            let pi_s = 0
            checkelement = data.data.spo2.length
            for(var i=0;i<checkelement;i++){
              spo2[i] = JSON.parse(data.data.spo2[i].value)
              spo2_s += spo2[i]
              bpm[i] = JSON.parse(data.data.bpm[i].value)
              bpm_s += bpm[i]
              pi[i] = JSON.parse(data.data.pi[i].value)/10
              pi_s += pi[i]
            }
            spo2_a = spo2_s/checkelement
            bpm_a = bpm_s/checkelement
            pi_a = pi_s/checkelement
          })
          .catch((err)=>{
            console.log(err)
          })
        ,60000)
    })
	.catch((err)=>{
	  console.log(err)
	})

}

router.get('/', function (req, res, next) {
	return res.render('index.ejs');
});

router.post('/', function(req, res, next) {
	
	personInfo = req.body;
	console.log(personInfo);

	if(!personInfo.ID){
		res.send();
	} 
	else {
		User.findOne({ID:personInfo.ID},(err,data)=>{
			if(data){
				var newValue = {$set:{
					ID: personInfo.ID
				}}
				UserInfo.findOneAndUpdate({ID:personInfo.ID},newValue,(err,data)=>{
					if(data){
						res.send({"Success":"You are regestered,You can login now."});
					}
					if(err){
						var newData = new UserInfo({
							ID: personInfo.ID
						});
						newData.save();
						res.send({"Success":"You are regestered,You can login now."});
					}
				})
			}
			else{
				var newPerson = new User({
					ID:personInfo.ID
				});
				newPerson.save();

				var newData = new UserInfo({
					ID:personInfo.ID
				});
				newData.save();

				res.send({"Success":"You are regestered,You can login now."});
			}
		})
	}
});

router.get('/prep', function (req, res, next){
	User.findOne({ID:personInfo.ID},function(err,data){
		console.log("data");
		console.log(data);
		if(!data){
			res.redirect('/');
		}else{
			getApi()
			show = 0
			return res.render('prep.ejs');
		}
	});
})

router.get('/waiting',function (req, res, next) {
	if(show == 0){
		getApi()
		return res.render('wait.ejs')
	}
	else{
		res.redirect('/');
	}
})

router.get('/wait', function (req, res, next){
	getApi()
	UserInfo.findOne({ID:personInfo.ID},(err,data)=>
	{
		if(!data){
			var newPerson = new UserInfo({
				ID: personInfo.ID,
				bpm: bpm_a.toFixed(2),
				spo2: spo2_a.toFixed(2),
				pi: pi_a.toFixed(2)
			});
			newPerson.save(); 
			res.redirect('/profile');
		}else{
			var newPerson = new UserInfo({
				ID: personInfo.ID,
				bpm: bpm_a.toFixed(2),
				spo2: spo2_a.toFixed(2),
				pi: pi_a.toFixed(2)
			});
			newPerson.save();
			res.redirect('/profile');
		}
	});
})

router.get('/profile', function (req, res, next) {
	console.log("profile");
	UserInfo.findOne({ID:personInfo.ID}).sort([['createdAt',-1]]).exec(function(err,data){
		console.log("data");
		console.log(data);
		if(!data){
			res.redirect('/');
		}else{
			//console.log("found");
			var newValue = {$set:{
				ID: personInfo.ID,
				bpm: data.bpm,
				spo2: data.spo2,
				pi: data.pi
			}}
			User.findOneAndUpdate({ID:personInfo.ID},newValue, {new: true},(err,res)=>{
				if(err){
					var newData = new UserInfo({
						ID: personInfo.ID,
						bpm: data.bpm,
						spo2: data.spo2,
						pi: data.pi
					});
					newData.save();
				}
			})
			return res.render('data.ejs', {"spo2":data.spo2,"pr":data.bpm,"pi":data.pi});
		}
	});
});

router.get('/logout', function (req, res, next) {
	console.log("logout")
	if (req.session) {
    // delete session object
    req.session.destroy(function (err) {
    	if (err) {
    		return next(err);
    	} else {
    		return res.redirect('/');
    	}
    });
}
});

router.get('/loginadmin', function (req, res, next) {
	return res.render('adminlogin.ejs');
});

router.post('/loginadmin', function(req, res, next){
	if(req.body.username == 'admin' && req.body.password == '123456789'){
		res.send({"Success":"You can login now."});
	}
	else{
		return res.redirect('/');
	}
});

router.get('/admin', function(req, res, next) {
	User.find({}).exec(function(err,data){
		if(data){
			console.log(data)

			return res.render('admin.ejs', {data});
		}
		else{
			throw err;
		}
	})
});


router.get('/admin1/:id',function(req,res,next){
	UserInfo.find({ID: req.params.id}).sort([['createdAt',-1]]).exec(function(err,data){
		if(data){
			console.log(data)
			return res.render('admin1.ejs', {data});
		}
		else{
			throw err
		}
	})
})

module.exports = router;