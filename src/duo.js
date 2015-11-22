var lang_codes = ['fr','en','fi','sv','de', 'es', 'ru'];

// Function to get keys of a dictionary
var keys = function(o){
   if (o !== Object(o))
      throw new TypeError('Object.keys called on non-object');
   var ret=[],p;
   for(p in o) if(Object.prototype.hasOwnProperty.call(o,p)) ret.push(p);
   return ret;
};

// Function to send Ajax request
var xhrRequest = function (url, type, params, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this);
  };
  xhr.open(type, url);
  if(params) {
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhr.send(params);
  }
  else {
    xhr.send();
  }
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
  
  console.log("Sending " + words.length + " words to Pebble");
  
  Pebble.sendAppMessage(dictionary,
    function(e) {
      console.log("Data sent to Pebble successfully!");
    },
    function(e) {
      console.log("Error sending data to Pebble!");
    }
  );
}

function login(username, password, then_do) {
  var login_url = 'https://www.duolingo.com/login';
  var params = "login="+username+"&password="+password+"&=Login";
  xhrRequest(login_url,'POST',params, function(response) {
    console.log(response.getAllResponseHeaders());
    if(response.status != 200) {
      console.log('Login status ' + response.statusText);
      console.log("Login reply: " + response.responseText);
    }
    then_do();
  });
}

function switch_lang(lang, then_do) {
  var switch_url = 'https://www.duolingo.com/switch_language';
    var params = 'learning_language='+lang;
    xhrRequest(switch_url,'POST',params, function(response) {
      if(response.status != 200) {
        console.log('Switch status ' + response.statusText);
        console.log("Switch reply: " + response.responseText);
      }
      console.log("Switch reply: " + response.responseText);

      then_do();
    });    
}

function get_words(username, password, lang) {
   if(username === null) {
    send_words([]);
    return;
  }
  
  if(password === '') {
    console.log("Getting words without password " + username);
    get_current_words(username, lang);
  }
  else {
    console.log("Getting words using password " + username);
    login(username, password, function() {
      switch_lang(lang, function() {
        get_current_words(username, lang);
      });
    });
  }
}

function get_current_words(username, lang) {
  console.log('Getting Duolingo words for user ' + username);
     
  var url = "https://www.duolingo.com/users/"+username;

  // List all known words. Use a dictionary to simulate a set
  // May break at any time since the Duolingo API is not really public
  xhrRequest(url,'GET', null, function (response) {  
    var json = JSON.parse(response.responseText);
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
    var words = keys(known_words);
    
    send_words(words);
  });
}


function fetch() {
  if (!(localStorage.language in lang_codes)) {
    console.log("ERROR: invalid lang code " + localStorage.language);
  }
  else {
    get_words(localStorage.duo_username, localStorage.password, lang_codes[localStorage.language]);  
  }
}

Pebble.addEventListener('appmessage',
  function(e) {
    console.log("JS got an appmessage " + e.payload.KEY_SEND_WORDS);
    if(e.payload.KEY_SEND_WORDS !== undefined) {
      console.log("Received request for language " + e.payload.KEY_SEND_WORDS);
      localStorage.language = e.payload.KEY_SEND_WORDS;
      fetch();
    }
});

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
                            localStorage.password = '';

    if(localStorage.language === null) {
      localStorage.language = 0;
    }
    fetch();
    setInterval(fetch, 24*60*60*1000); // Update words every day
  }
);