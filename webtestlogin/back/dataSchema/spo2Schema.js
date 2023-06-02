const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const spo2Schema = new Schema({
    bpm: {
        type: String,
        required: true
    },
    spo2: {
        type: String,
        required: true
    },
    pi: {
        type: String,
        required: true
    }
},{timestamps: true});

const spo2 = mongoose.model('spo2', spo2Schema);

module.exports = spo2;

