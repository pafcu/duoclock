Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  var configUrl = 'https://pafcu.github.io/duoclock/settings_2.html';
  Pebble.openURL(configUrl);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));
  localStorage.duo_username = configData.username;
  localStorage.language = configData.language;
  //localStorage.password = configData.password; // Password does not currently work due to Pebble not setting cookies
  localStorage.password = '';
  fetch();
});