const express = require('express');
const admin = require('firebase-admin');
const http = require('http');
const socketIo = require('socket.io');
const cors = require('cors');
const path = require('path');
require('dotenv').config();

const app = express();
const server = http.createServer(app);
const io = socketIo(server, {
  cors: {
    origin: "*",
    methods: ["GET", "POST"]
  }
});

app.use(cors());
app.use(express.json());
app.use(express.static('public'));

const serviceAccount = require('./firebase-service-account.json');
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: process.env.FIREBASE_DATABASE_URL
});
const db = admin.database();

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.get('/api/data', async (req, res) => {
  try {
    const snapshot = await db.ref('arduino_data').once('value');
    const data = snapshot.val();
    res.json(data);
  } catch (error) {
    res.status(500).json({ error: 'Failed to fetch data' });
  }
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});