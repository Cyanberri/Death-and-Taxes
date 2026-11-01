const express = require('express');
const { setDefaultAutoSelectFamily } = require('net');
const app = express();
const PORT = 3000;

app.use(express.static('public'));

app.listen(PORT, () => {
  console.log(`Website is running at http://localhost:${PORT}`);
});



