Pebble.addEventListener('showConfiguration', function() {
  var back = window.localStorage.getItem('BACKGROUND_COLOR') ? window.localStorage.getItem('BACKGROUND_COLOR') : 0;
  var hour1 = window.localStorage.getItem('HOUR_1_COLOR') ? window.localStorage.getItem('HOUR_1_COLOR') : 5592405;
  var hour2 = window.localStorage.getItem('HOUR_2_COLOR') ? window.localStorage.getItem('HOUR_2_COLOR') : 5592405;
  var min1 = window.localStorage.getItem('MIN_1_COLOR') ? window.localStorage.getItem('MIN_1_COLOR') : 5592405;
  var min2 = window.localStorage.getItem('MIN_2_COLOR') ? window.localStorage.getItem('MIN_2_COLOR') : 5592405;
  
  var url = 'http://gjthoman.github.io/orbit/config.html?background_color=' + back + '&hour_1_color=' + hour1 + '&hour_2_color=' + hour2 + '&min_1_color=' + min1 + '&min_2_color=' + min2 

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  // Decode the user's preferences

  var configData = JSON.parse(decodeURIComponent(e.response));
  // Send to the watchapp via AppMessage
  var dict = {
    'AppKeyBackgroundColor': configData.background_color,
    'AppKeyHour1Color': configData.hour_1_color,
    'AppKeyHour2Color': configData.hour_2_color,
    'AppKeyMin1Color': configData.min_1_color,
    'AppKeyMin2Color': configData.min_2_color
  };
  
  window.localStorage.setItem('BACKGROUND_COLOR', configData.background_color);
  window.localStorage.setItem('HOUR_1_COLOR', configData.hour_1_color);
  window.localStorage.setItem('HOUR_1_COLOR', configData.hour_1_color);
  window.localStorage.setItem('MIN_1_COLOR', configData.min_1_color);
  window.localStorage.setItem('MIN_2_COLOR', configData.min_2_color);
  
  // Send to the watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Config data sent successfully!');
  }, function(e) {
    console.log(JSON.stringify(e));
    console.log('Error sending config data!');
  });
});