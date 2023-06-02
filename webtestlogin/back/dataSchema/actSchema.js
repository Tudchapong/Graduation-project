const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const actSchema = new Schema({
    ID: {
        type: String,
        required: true
    },
    bpm: {
        type: String,
        required: false
    },
    spo2: {
        type: String,
        required: false
    },
    pi: {
        type: String,
        required: false
    }
},{timestamps: true});

const IDs = mongoose.model('IDs', actSchema);

module.exports = IDs;

