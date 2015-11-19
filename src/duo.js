var lang_codes = ['fr','en','fi','sv','de'];

// Function to get keys of a dictionary
var keys = function(o){
   if (o !== Object(o))
      throw new TypeError('Object.keys called on non-object');
   var ret=[],p;
   for(p in o) if(Object.prototype.hasOwnProperty.call(o,p)) ret.push(p);
   return ret;
};

// Function to send Ajax request
var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function send_words(words) {
  // Send data to Pebble
  var language;
  if(isNaN(1*localStorage.language)) { // Force to integer
    language = 0; // Fallback
  }
  else {
    language = 1*localStorage.language;
  }
  var dictionary = {
    'KEY_KNOWN_WORDS': words.join('|'),
    'KEY_NUM_KNOWN_WORDS': words.length,
    'KEY_LANGUAGE': language 
  };
    
  Pebble.sendAppMessage(dictionary,
    function(e) {
      console.log("Data sent to Pebble successfully!");
    },
    function(e) {
      console.log("Error sending data to Pebble!");
    }
  );
}

function get_words(username, lang) {
  console.log('Getting Duolingo words for user ' + username);
  if(username === null) {
    //send_words(['unconfigured']);
    send_words([]);
    return;
  }
  var url = "https://www.duolingo.com/users/"+username;

  // List all known words. Use a dictionary to simulate a set
  // May break at any time since the Duolingo API is not really public
  xhrRequest(url,'GET', function (responseText) {  
    var json = JSON.parse(responseText);
    var known_words = {};
  
    if(lang in json.language_data) {
      var skills = json.language_data[lang].skills;
      for(var iskill in skills) {
        var skill = skills[iskill];
        if(skill.learned) {
          for(var iword in skill.words) {
            var word = skill.words[iword];
            known_words[word] = 1;
          }
        }
      }
    }
    else {
      //known_words[''] = 0;
    }
    var words = keys(known_words);
    
    send_words(words);
  });
}

function fetch() {
  get_words(localStorage.duo_username, lang_codes[localStorage.language]);
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    if(localStorage.language === null) {
      localStorage.language = 0;
    }
    fetch();
    setInterval(fetch, 24*60*60*1000); // Update words every day
  }
);