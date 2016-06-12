Pebble.addEventListener('showConfiguration', function() {
  var back = window.localStorage.getItem('BACKGROUND_COLOR') ? window.localStorage.getItem('BACKGROUND_COLOR') : 0;
  var hour1 = window.localStorage.getItem('HOUR_1_COLOR') ? window.localStorage.getItem('HOUR_1_COLOR') : 65280;
  var hour2 = window.localStorage.getItem('HOUR_2_COLOR') ? window.localStorage.getItem('HOUR_2_COLOR') : 65280;
  var min1 = window.localStorage.getItem('MIN_1_COLOR') ? window.localStorage.getItem('MIN_1_COLOR') : 65280;
  var min2 = window.localStorage.getItem('MIN_2_COLOR') ? window.localStorage.getItem('MIN_2_COLOR') : 65280;
  var date = window.localStorage.getItem('DATE_COLOR') ? window.localStorage.getItem('DATE_COLOR') : 11184810;  
  var batt_b = window.localStorage.getItem('BATT_BACK_COLOR') ? window.localStorage.getItem('BATT_BACK_COLOR') : 5592405;
  var batt_f = window.localStorage.getItem('BATT_FORE_COLOR') ? window.localStorage.getItem('BATT_FORE_COLOR') : 11184810;
  var show_batt = window.localStorage.getItem('SHOW_BATT') ? window.localStorage.getItem('SHOW_BATT') : 1;
  var show_date = window.localStorage.getItem('SHOW_DATE') ? window.localStorage.getItem('SHOW_DATE') : 2;
  
 var config = [  
  {
    type: "title",
    label: "Orbit Watchface"
  },
  {
    type: "option",
    key: "batt_visibility",
    label: "Battery Visibility",
    default: show_batt,
    options: [ "Always Show", "Never Show", "Show on Shake"]
  },
  {
    type: "option",
    key: "date_visibility",
    label: "Date Visibility",
    default: show_date,
    options: [ "Always Show", "Never Show", "Show on Shake"]
  },
  {
    type: "section",
    label: "Time Colors"
  },
  {
    type: "color",
    key: "background_color",
    label: "Background",
    default: back
  },
  {
    type: "color",
    key: "hour_1_color",
    label: "Hours: First Digit",
    default: hour1
  },
  {
    type: "color",
    key: "hour_2_color",
    label: "Hours: Second Digit",
    default: hour2
  },
  {
    type: "color",
    key: "min_1_color",
    label: "Minutes: First Digit",
    default: min1
  },
  {
    type: "color",
    key: "min_2_color",
    label: "Minutes: Second Digit",
    default: min2
  },
  {
    type: "section",
    label: "Indicator Colors"
  },
   {
    type: "color",
    key: "date_color",
    label: "Date Text",
    default: date
  },
  {
    type: "color",
    key: "battery_foreground_color",
    label: "Battery Foreground",
    default: batt_f
  },
  {
    type: "color",
    key: "battery_background_color",
    label: "Battery Background",
    default: batt_b
  }
];

  var url = 'http://gjthoman.github.io/pebble_auto_config/1.0.2?config=' + encodeURIComponent(JSON.stringify(config));  
  
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
    'AppKeyMin2Color': configData.min_2_color,
    'AppKeyBattBackColor': configData.battery_background_color,
    'AppKeyBattForeColor': configData.battery_foreground_color,
    'AppKeyShowBatt': parseInt(configData.batt_visibility),
    'AppKeyShowDate': parseInt(configData.date_visibility),
    'AppKeyDateColor': configData.date_color
  };

  window.localStorage.setItem('BACKGROUND_COLOR', configData.background_color);
  window.localStorage.setItem('HOUR_1_COLOR', configData.hour_1_color);
  window.localStorage.setItem('HOUR_2_COLOR', configData.hour_2_color);
  window.localStorage.setItem('MIN_1_COLOR', configData.min_1_color);
  window.localStorage.setItem('MIN_2_COLOR', configData.min_2_color);
  window.localStorage.setItem('DATE_COLOR', configData.date_color);
  window.localStorage.setItem('BATT_BACK_COLOR', configData.battery_background_color);
  window.localStorage.setItem('BATT_FORE_COLOR', configData.battery_foreground_color);
  window.localStorage.setItem('SHOW_BATT', parseInt(configData.batt_visibility));
  window.localStorage.setItem('SHOW_DATE', parseInt(configData.date_visibility));
  
  // Send to the watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Config data sent successfully!');
  }, function(e) {
    console.log(JSON.stringify(e));
    console.log('Error sending config data!');
  });
});