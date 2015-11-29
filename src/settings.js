/*
Copyright Stefan Parviainen <pafcu@iki.fi> 2015

This file is part of DuoClock.

DuoClock is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DuoClock is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DuoClock.  If not, see <http://www.gnu.org/licenses/>.
*/

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
