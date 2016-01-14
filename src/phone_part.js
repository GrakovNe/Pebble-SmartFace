var Location;
var Hourly_Vibe;
var BT_Vibe ;
var Info_Updates_Frequency;
var Add_String;
var Language;
var Inverted;
var Hide_Weather;
var Charge_Vibe;
var Hide_Battery;
var Hide_BT;
var Temp_Units;
var Night_Mode;
var Night_Hours_On;
var Night_Hours_Off;
var Night_Mins_On;
var Night_Mins_Off;
var Shake_Updade;
var Night_Invert_Display;
var Night_Silent;
var Night_Offline;
var Custom_Text;
var Text_Size;
var Date_Style;

var Location_key                    = 1;
var Hourly_Vibe_key                 = 2;
var BT_Vibe_key                     = 3;
var Info_Updates_Frequency_key      = 4;
var Add_String_key                  = 5;
var Language_key                    = 6;
var Inverted_key                    = 7;
var Hide_Weather_key                = 8;
var Charge_Vibe_key                 = 9;
var Hide_Battery_key                =10;
var Hide_BT_key                     =11;
var Temp_Units_key                  =12;
var Night_Mode_key                  =13;
var Night_Hours_On_key              =14;
var Night_Hours_Off_key             =15;
var Night_Mins_On_key               =16;
var Night_Mins_Off_key              =17;
var Shake_Update_key                =18;
var Night_Invert_Display_key        =19;
var Night_Silent_key                =20;
var Night_Offline_key               =21;
var Custom_Text_key                 =22;
var Text_Size_key                   =23;
var Date_Style_key                  =24;

var Current_Version = 3;

function HTTPGET(url) {
    var req = new XMLHttpRequest();
	for (var i = 0; i < 2; i++){
		try {
			req.open("GET", url, false);
			req.send(null);
            }
		catch (err) {}
		if(req.status == 200)
			return req.responseText;
	}
	return 0;
}

function CheckVersion(){
	var Notification_Text = [["New version released!", "You can download it from PebbleStore now"], ["Новая версия!", "Вы можете скачать ее из PebbleStore"]];
	var Latest_Version = parseInt(HTTPGET("http://grakovne.org/pebble/SmartFace/latest_version.html") );
	if ( (Latest_Version != 0) && (Latest_Version > Current_Version) )
		Pebble.showSimpleNotificationOnPebble(Notification_Text[Language][0], Notification_Text[Language][1]);
	
}

function ReadSettings(){
	Location = localStorage.getItem(Location_key);
	Hourly_Vibe = localStorage.getItem(Hourly_Vibe_key);
	BT_Vibe = localStorage.getItem(BT_Vibe_key);
	Info_Updates_Frequency = localStorage.getItem(Info_Updates_Frequency_key);
	Add_String = localStorage.getItem(Add_String_key);
	Language = localStorage.getItem(Language_key);
	Inverted = localStorage.getItem(Inverted_key);
	Hide_Weather = localStorage.getItem(Hide_Weather_key);
	Charge_Vibe = localStorage.getItem(Charge_Vibe_key);
	Hide_Battery = localStorage.getItem(Hide_Battery_key);
	Hide_BT = localStorage.getItem(Hide_BT_key);
	Temp_Units = localStorage.getItem(Temp_Units_key);
	Night_Mode = localStorage.getItem(Night_Mode_key);
	Night_Hours_On = localStorage.getItem(Night_Hours_On_key);
	Night_Hours_Off = localStorage.getItem(Night_Hours_Off_key);
	Night_Mins_On = localStorage.getItem(Night_Mins_On_key);
	Night_Mins_Off = localStorage.getItem(Night_Mins_Off_key);
	Shake_Update = localStorage.getItem(Shake_Update_key);
	Night_Silent = localStorage.getItem(Night_Silent_key);
	Night_Invert_Display = localStorage.getItem(Night_Invert_Display_key);
	Night_Offline = localStorage.getItem(Night_Offline_key);
	Custom_Text = localStorage.getItem(Custom_Text_key);
	Text_Size = localStorage.getItem(Text_Size_key);
	Date_Style = localStorage.getItem(Date_Style_key);
	
	if (!Location)
		Location = "London";
	if (!Hourly_Vibe)
		Hourly_Vibe = 1;
	if (!BT_Vibe)
		BT_Vibe = 1;
	if (!Info_Updates_Frequency)
		Info_Updates_Frequency = 1;
	if (!Add_String)
		Add_String = "Empty";
	if (!Language)
		Language = 0;
	if (!Inverted)
		Inverted = 0;
	if (!Hide_Weather)
		Hide_Weather = 0;
	if (!Charge_Vibe)
		Charge_Vibe = 1;
	if (!Hide_Battery)
		Hide_Battery = 0;
	if (!Hide_BT)
		Hide_BT = 0;
	if (!Temp_Units)
		Temp_Units = 0;
	if (!Night_Mode)
		Night_Mode = 0;
	if (!Night_Hours_On)
		Night_Hours_On = 0;
	if (!Night_Hours_Off)
		Night_Hours_Off = 0;
	if (!Night_Mins_On)
		Night_Mins_On = 0;
	if (!Night_Mins_Off)
		Night_Mins_Off = 0;	
	if (!Shake_Update)
		Shake_Update = 1;	
	if (!Night_Silent)
		Night_Silent = 1;
	if (!Night_Invert_Display)
		Night_Invert_Display = 1;
	if (!Night_Offline)
		Night_Offline = 1;
	if (!Custom_Text)
		Custom_Text = "Hello!";
	if (!Text_Size)
		Text_Size = 0;
	if (!Date_Style)
		Date_Style = 0;
}

function SendSettings(){
	
	var Night_Start  = parseInt(Night_Hours_On) * 60 + parseInt(Night_Mins_On);
	var Nigth_Finish = parseInt(Night_Hours_Off) * 60 + parseInt(Night_Mins_Off);
	
	Pebble.sendAppMessage({
							'HOURLY_VIBE'             : parseInt(Hourly_Vibe),
							'BT_VIBE'                 : parseInt(BT_Vibe),
							'INFO_UPDATES_FREQUENCY'  : parseInt(Info_Updates_Frequency),
							'LANGUAGE'                : parseInt(Language),
							'INVERTED'                : parseInt (Inverted),
							'CHARGE_VIBE'             : parseInt(Charge_Vibe),
							'HIDE_BATTERY'            : parseInt(Hide_Battery),
							'HIDE_BT'                 : parseInt(Hide_BT),
							'HIDE_WEATHER'            : parseInt(Hide_Weather),
							'NIGHT_MODE'              : parseInt(Night_Mode),
							'NIGHT_START'             : parseInt(Night_Start),
							'NIGHT_FINISH'            : parseInt(Nigth_Finish),
							'SHAKE_UPDATE'            : parseInt(Shake_Update),
							'NIGHT_SILENT'            : parseInt(Night_Silent),
							'NIGHT_INVERT_DISPLAY'    : parseInt(Night_Invert_Display),
							'NIGHT_OFFLINE'           : parseInt(Night_Offline),
							'TEXT_SIZE'               : parseInt(Text_Size),
							'DATE_STYLE'              : parseInt(Date_Style),
							}); 
}

function Update_Info(){
	var CurrentWeather = " ";
	var response = " ";
	if (Hide_Weather == 0){
		response = HTTPGET("http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=ba08e74fcb7da39750070a88a2ab8a9b");
		
		var json = JSON.parse(response);
		var temperature = (json.main.temp - 273.15);
	
		/*Just adding "+" sign for good temperature*/
		var sign = "";
		if (temperature > 0)
			sign = "+";
	
		var Unit = "C";
		if (parseInt(Temp_Units) == 1){
			temperature = temperature * 1.8 + 32;
			Unit = "F";
		}
		
		temperature = temperature.toFixed(1);	
  
		var state = json.weather[0].main;

		switch (state){
			case "Thunderstorm":
				state = "Storm";
				break;
			case "Atmosphere":
				state = "Smoke";
		}
	
		if (Language == 1)
			switch(state){
				case "Storm":
					state = "Гроза";
					break;
				case "Drizzle":
					state = "Морось";
					break;
				case "Rain":
					state = "Дождь";
					break;
				case "Snow":
					state = "Снег";
					break;
				case "Smoke":
					state = "Туман";
					break;
				case "Clouds":
					state = "Облачно";
					break;
				case "Extreme":
					state = "Шторм";
					break;
				case "Clear":
					state = "Ясно";
					break;
				case "Fog":
					state = "Дымка";
					break;
				case "Mist":
					state = "Туман";
					break;
				default:
					state = "Облачно";
			}
		CurrentWeather = state + ', ' + sign + temperature + Unit;
	}
	
	else
		CurrentWeather = " ";
		
	switch (Add_String){
		case "Empty":
			response = " ";
			break;
		case "Custom_string":
			response = Custom_Text;
			break;
		
		case "Location":
			response = Location;
			if (response.length > 10)
				response = response.substr(0, 10) + "...";
			break;
			
		default:
			response = HTTPGET("http://grakovne.org/pebble/SmartFace/CustomStings/" + Add_String + ".php");
	}

	Pebble.sendAppMessage({"CURRENT_WEATHER" : CurrentWeather, "ADD_INFO" : response});
	
	//console.log('SmartFace [phone]: Info updated: ' + CurrentWeather + ' ' + Location + '; ' + response);
}

Pebble.addEventListener("showConfiguration",
  function(e) {
    try {
		Pebble.openURL("http://grakovne.org/pebble/SmartFace/AppConfig_3_0.php?Location=" + Location + "&Info_Updates_Frequency=" + Info_Updates_Frequency + "&Hourly_Vibe=" + Hourly_Vibe + "&BT_Vibe=" + BT_Vibe + "&Add_String=" + Add_String + "&Language=" + Language + "&Inverted=" + Inverted + "&Hide_Weather=" + Hide_Weather + "&Charge_Vibe=" + Charge_Vibe + "&Hide_Battery=" + Hide_Battery + "&Hide_BT=" + Hide_BT + "&Temp_Units=" + Temp_Units + "&Night_Mode=" + Night_Mode + "&Night_Start=" + Night_Hours_On + ":" + Night_Mins_On + "&Night_Finish=" + Night_Hours_Off + ":" + Night_Mins_Off + "&Shake_update=" + Shake_Update + "&Night_Silent=" + Night_Silent + "&Night_Invert_Display=" + Night_Invert_Display + "&Night_Offline=" + Night_Offline + "&Custom_Text=" + Custom_Text + "&Text_Size=" + Text_Size + "&Date_Style=" + Date_Style);
		}
	catch (err) {
		Pebble.openURL("http://grakovne.org/pebble/SmartFace/AppConfig_3_0.php");
		}
	}
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
	
	localStorage.setItem(Location_key, configuration.Location);
	
	localStorage.setItem(Hourly_Vibe_key, configuration.Hourly_Vibe);
	
	localStorage.setItem(BT_Vibe_key, configuration.BT_Vibe);  
	 
	localStorage.setItem(Info_Updates_Frequency_key, configuration.Info_Updates_Frequency);
  
	localStorage.setItem(Add_String_key, configuration.Add_String); 
	
	localStorage.setItem(Language_key, configuration.Language);
	 
	localStorage.setItem(Inverted_key, configuration.Inverted);
	
	localStorage.setItem(Hide_Weather_key, configuration.Hide_Weather);

	localStorage.setItem(Charge_Vibe_key, configuration.Charge_Vibe);

	localStorage.setItem(Hide_Battery_key, configuration.Hide_Battery);
	
	localStorage.setItem(Hide_BT_key, configuration.Hide_BT);

	localStorage.setItem(Temp_Units_key, configuration.Temp_Units);
	
	localStorage.setItem(Night_Mode_key, configuration.Night_Mode);
	  
	localStorage.setItem(Shake_Update_key, configuration.Shake_update);
	  
	localStorage.setItem(Night_Silent_key, configuration.Night_Silent);
	  
	localStorage.setItem(Night_Offline_key, configuration.Night_Offline);
	
	localStorage.setItem(Custom_Text_key, configuration.Custom_Text);
	  
	localStorage.setItem(Text_Size_key, configuration.Text_Size);
	
	localStorage.setItem(Night_Invert_Display_key, configuration.Night_Invert_Display);
	  
	localStorage.setItem(Date_Style_key, configuration.Date_Style);
	
    localStorage.setItem(Night_Hours_On_key, configuration.Night_Start.split(":")[0]);
	  
	localStorage.setItem(Night_Mins_On_key, configuration.Night_Start.split(":")[1]);
   
	localStorage.setItem(Night_Hours_Off_key, configuration.Night_Finish.split(":")[0]);
	  
  	localStorage.setItem(Night_Mins_Off_key, configuration.Night_Finish.split(":")[1]);
	  
	ReadSettings();
	SendSettings();
	Update_Info();
	
  }
);

Pebble.addEventListener("appmessage",
  function(e) {
	//console.log('SmartFace [phone]: New message Received!');
	 
	if ('CURRENT_WEATHER' in e.payload)
		Update_Info();
  }
);

Pebble.addEventListener("ready",
  function(e) {
	ReadSettings();
	SendSettings();
	Update_Info();  
	CheckVersion();
	//console.log('SmartFace [phone]: App running OK...');
  }
);