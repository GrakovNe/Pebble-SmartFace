#include <pebble.h>
#include "window.h"

#define MILLS_IN_HOUR               3600000
	
#define NON_INVERTED_WINDOW         0
#define INVERTED_WINDOW             1
	
#define ENGLISH_LANG                0
#define RUSSIAN_LANG                1

#define RECEIVING_LATENCY           60000	
	
#define Hourly_Vibe_key             1
#define BT_Vibe_key                 2
#define Info_Updates_Frequency_key  3
#define Add_String_key              4
#define Language_key                5
#define Inverted_key                6
#define Charge_Vibe_key             7
#define Hide_Battery_key            8
#define Hide_BT_key                 9
#define Weather_Text_key            10
#define AddInfo_Text_key            11
#define Hide_Weather_key            12
#define Night_Mode_key              13
#define Night_Start_key             14
#define Night_Finish_key            15
#define Shake_Update_key            16
#define Night_Silent_key            17
#define Night_Invert_Display_key    18
#define Night_Offline_key           19
#define Text_Size_key               20
#define Date_Style_key              21
	
Window *MainWindow;
TextLayer *Time_Text;
TextLayer *Week_Text;
TextLayer *Date_Text;
TextLayer *CWeather_Text;
TextLayer *Battery_Text;
TextLayer *Connection_Text;
TextLayer *AddInfo_Text;

BitmapLayer *BT_Image;
BitmapLayer *BAT_Image;

GBitmap *BT;
GBitmap *BAT;

GFont Bar_Font;
GFont Time_Font;
GFont Date_Font;
GFont CWeather_Font;

static bool JustRun_Flag       = 1;
static bool IsConnected_Flag   = 1;
static bool IsNight_Flag       = 0;

time_t now;
struct tm *current_time;

AppTimer* IsReceiving;

static char Time[] = "00:00";
static char Date[32];


static inline void Process_Received_Data(DictionaryIterator *iter, void *context);
static inline void send_int(uint8_t key, uint8_t cmd);
static inline void UpdateWeather();
static inline void UpdateConnection(bool Connected);
static inline void UpdateBattery(BatteryChargeState State);
static inline void UpdateTime(struct tm* CurrentTime, TimeUnits units_changed);
static inline void UpdateDate(struct tm* CurrentTime, TimeUnits units_changed);
static inline void Init_Display(void);
static inline void ReadSettings(void);
static inline void Shake_Handler(AccelAxisType axis, int32_t direction);
static inline void Answer_Error();

static const uint32_t Battery_Icons[] = {
	RESOURCE_ID_BAT_ICON_0, 
	RESOURCE_ID_BAT_ICON_10, 
	RESOURCE_ID_BAT_ICON_20, 
	RESOURCE_ID_BAT_ICON_30, 
	RESOURCE_ID_BAT_ICON_40, 
	RESOURCE_ID_BAT_ICON_50, 
	RESOURCE_ID_BAT_ICON_60, 
	RESOURCE_ID_BAT_ICON_70, 
	RESOURCE_ID_BAT_ICON_80, 
	RESOURCE_ID_BAT_ICON_90, 
	RESOURCE_ID_BAT_ICON_100
	};

static const uint32_t BT_Icons[] = {
	RESOURCE_ID_NO_BT_ICON,
	RESOURCE_ID_BT_ICON
};

static const char DayNames[2][7][24] = {
	{
		"SUNDAY",
		"MONDAY",
		"TUESDAY",
		"WEDNESDAY",
		"THURSDAY",
		"FRIDAY",
		"SATURDAY"
	},
	{
		"BOCKPECEHЬE",
		"ПOHEДEЛЬHИK",
		"BTOPHИK",
		"CPEДA",
		"ЧETBЕPГ",
		"ПЯTHИЦA",
		"CYББOTA"
	}
};

static const char MonthNames[2][12][24] = {
	{
		"JANUARY",
		"FEBRUARY",
		"MARCH",
		"APRIL",
		"MAY",
		"JUNE",
		"JULE",
		"AUGUST",
		"SEPTEMBER",
		"OCTOBER",
		"NOVEMBER",
		"DECEMBER"
	},
	{
		"ЯНВАРЯ",
		"ФЕВРАЛЯ",
		"МАРТА",
		"АПРЕЛЯ",
		"МАЯ",
		"ИЮНЯ",
		"ИЮЛЯ",
		"АВГУСТА",
		"СЕНТЯБРЯ",
		"ОКТЯБРЯ",
		"НОЯБРЯ",
		"ДЕКАБРЯ"
		
	}
};

static const char OfflineNames[2][16] = {
	"OFFLINE",
	"HET CETИ"
};

static const char BTNames[2][2][7] = {
	{
		"LOST",
		"OK"
	},
	{
		"HET",
		"OK"
	}
};

enum {
	CURRENT_WEATHER          = 0,
	HOURLY_VIBE              = 1,
	BT_VIBE                  = 2,
	INFO_UPDATES_FREQUENCY   = 3,
	ADD_INFO                 = 4,
	LANGUAGE                 = 5,
	INVERTED                 = 6,
	CHARGE_VIBE              = 7,
	HIDE_BATTERY             = 8,
	HIDE_BT                  = 9,
	HIDE_WEATHER             = 10,
	NIGHT_MODE               = 11,
	NIGHT_START              = 12,
	NIGHT_FINISH             = 13,
	SHAKE_UPDATE             = 14,
	NIGHT_SILENT             = 15,
	NIGHT_INVERT_DISPLAY     = 16,
	NIGHT_OFFLINE            = 17,
	TEXT_SIZE                = 18,
	DATE_STYLE               = 19,
};

static struct {
	char Info_Updates_Frequency;
	char Weather_Text [24];
	char AddInfo_Text [24];
	bool Hourly_Vibe;
	bool BT_Vibe;
	bool Charge_Vibe;
	bool Language;
	bool Inverted;
	bool Hide_Battery;
	bool Hide_BT;
	bool Hide_Weather;
	bool Night_Mode;
	bool Shake_Update;
	bool Night_Silent;
	bool Night_Invert_Display;
	bool Night_Offline;
	bool Text_Size;
	bool Date_Style;
	int Night_Start;
	int Night_Finish;
} Settings;

void Answer_Error(){
	gbitmap_destroy(BT); 
	BT = gbitmap_create_with_resource(RESOURCE_ID_UPDATING_ERROR_IMAGE); 
	bitmap_layer_set_bitmap(BT_Image, BT);
	layer_mark_dirty((Layer *)BT_Image);
}

static void Process_Received_Data(DictionaryIterator *iter, void *context){
	
	Tuple *t = dict_read_first(iter);
	
	gbitmap_destroy(BT); 
	BT = gbitmap_create_with_resource(RESOURCE_ID_UPDATING_IMAGE); 
	bitmap_layer_set_bitmap(BT_Image, BT);
	layer_mark_dirty((Layer *)BT_Image);
	
	 while(t != NULL){
		char key = t->key;
        int value = t->value->int32;
		static char string_value[24];
		strcpy(string_value, t->value->cstring);	 
		 switch (key){
			 case CURRENT_WEATHER:
			 	
			 		if (strcmp(string_value, Settings.Weather_Text)){
			 			strcpy(Settings.Weather_Text, string_value);
			 			persist_write_string(Weather_Text_key, string_value);
 						text_layer_set_text(CWeather_Text, Settings.Weather_Text); 
					}
			 
			 	break; 
			 
			 case ADD_INFO:
			 
			 	if (strcmp(string_value, Settings.AddInfo_Text)){
			 			strcpy(Settings.AddInfo_Text, string_value);
			 			persist_write_string(AddInfo_Text_key, string_value);
 						text_layer_set_text(AddInfo_Text, Settings.AddInfo_Text); 
				}
			 
			 	break;
			
			 case HOURLY_VIBE:
			 	
			 	Settings.Hourly_Vibe = value;
			 	persist_write_bool(Hourly_Vibe_key, Settings.Hourly_Vibe);
			 	
			 	break;
			 
			 case BT_VIBE:
			 	
			 	Settings.BT_Vibe = value;
				persist_write_bool(BT_Vibe_key, Settings.BT_Vibe);
			 
				break;
			 
			 case NIGHT_MODE:
			 	
			 	Settings.Night_Mode = value;
			 	persist_write_bool(Night_Mode_key, Settings.Night_Mode);
			 
				break;
			 
			 case NIGHT_SILENT:
			 	
			 	Settings.Night_Silent = value;
			 	persist_write_bool(Night_Silent_key, Settings.Night_Silent);
			 
				break;
			 
			 case NIGHT_OFFLINE:
			 	
			 	Settings.Night_Offline = value;
			 	persist_write_bool(Night_Offline_key, Settings.Night_Offline);
			 
				break;
			 
			 case NIGHT_INVERT_DISPLAY:
			 	
			 	Settings.Night_Invert_Display = value;
			 	persist_write_bool(Night_Invert_Display_key, Settings.Night_Invert_Display);
			 
				break;
			 
			 case NIGHT_START:
			 	
			 	Settings.Night_Start = value;
			 	persist_write_int(Night_Start_key, Settings.Night_Start);
				 
			 	break;
			 
			  case NIGHT_FINISH:
			 	
			 	Settings.Night_Finish = value;
			 	persist_write_int(Night_Finish_key, Settings.Night_Finish);
			 
				break;
			 
			 case TEXT_SIZE:
			 	
			 	if (value != Settings.Text_Size){
			 		Settings.Text_Size = value;
			 		persist_write_int(Text_Size_key, Settings.Text_Size);
			 		window_stack_remove(MainWindow, false);
					DestroyWindow();
					BuildWindow(Settings.Text_Size);
					UpdateBattery(battery_state_service_peek());
					text_layer_set_text(Connection_Text, BTNames[Settings.Language][IsConnected_Flag]);
					window_stack_push(MainWindow, true);
				}
			 	
			 	break;
			 
			 case SHAKE_UPDATE:
			 	
			 	Settings.Shake_Update = value;
			 	persist_write_int(Shake_Update_key, Settings.Shake_Update);
			 	
			 	if (Settings.Shake_Update == 1)
					accel_tap_service_subscribe(Shake_Handler);
			 	else
					accel_tap_service_unsubscribe();
			 	
				break;

			 case LANGUAGE:
			 	
			 	if (value < 2){
				 	Settings.Language = value;
					persist_write_bool(Language_key, Settings.Language);
				}
			 	Init_Display();
			 
			 	break;
				
			 case INVERTED:
			 	
			 	if (value < 2){
					Settings.Inverted = value;
					persist_write_bool(Inverted_key, Settings.Inverted);
				}
			 
			 	if ( (IsNight_Flag) && (Settings.Night_Invert_Display) )
					SetColors(!Settings.Inverted);
			 	else
					SetColors(Settings.Inverted);
			 
			 	Init_Display();
			 
			 	break;
			 
			 case CHARGE_VIBE:
			 	
			 	if (value < 2){
					Settings.Charge_Vibe = value;
					persist_write_bool(Charge_Vibe_key, Settings.Charge_Vibe);
				}
			 
			 	break;
			 
			 case DATE_STYLE:
			 	
			 	if (value < 2){
					Settings.Date_Style = value;
					persist_write_bool(Date_Style_key, Settings.Date_Style);
				}
			 
			 	break;
			 
			 case HIDE_BATTERY:
			 	if (value < 2){
					Settings.Hide_Battery = value;
					persist_write_bool(Hide_Battery_key, Settings.Hide_Battery);
					SetBarText(Settings.Hide_Battery, Settings.Hide_BT, Settings.Hide_Weather);
				}
			 	
			 	break;
			 
			 case HIDE_BT:
			 	if (value < 2){
					Settings.Hide_BT = value;
					persist_write_bool(Hide_BT_key, Settings.Hide_BT);
					SetBarText(Settings.Hide_Battery, Settings.Hide_BT, Settings.Hide_Weather);
				}
			 	
			 	break;
			 
			 case HIDE_WEATHER:
			 
				if (value < 2){
					Settings.Hide_Weather = value;
					persist_write_bool(Hide_Weather_key, Settings.Hide_Weather);
					SetBarText(Settings.Hide_Battery, Settings.Hide_BT, Settings.Hide_Weather);
				}
			 
			 case INFO_UPDATES_FREQUENCY:
			 	
			 	Settings.Info_Updates_Frequency = value;
			 	persist_write_int(Info_Updates_Frequency_key, Settings.Info_Updates_Frequency);
			 	if (Settings.Info_Updates_Frequency < 10)
					app_timer_register(MILLS_IN_HOUR / Settings.Info_Updates_Frequency, UpdateWeather, 0);
				else app_timer_register(MILLS_IN_HOUR, UpdateWeather, 0);		 
		 }	 
        t = dict_read_next(iter);
    }	
	
	app_timer_cancel(IsReceiving);
	
	gbitmap_destroy(BT); 
	BT = gbitmap_create_with_resource(BT_Icons[IsConnected_Flag]); 
	bitmap_layer_set_bitmap(BT_Image, BT);
	Init_Display();
}

static inline void ReadSettings(){

	if (persist_exists(Weather_Text_key))
		persist_read_string(Weather_Text_key, Settings.Weather_Text, sizeof(Settings.Weather_Text));
	else
		strcpy(Settings.Weather_Text, "Set Location!");
		persist_write_string(Weather_Text_key, "string_value");
		
	
	if (persist_exists(AddInfo_Text_key))
		persist_read_string(AddInfo_Text_key, Settings.AddInfo_Text, sizeof(Settings.AddInfo_Text));
	else
		strcpy(Settings.AddInfo_Text, "Add something!");
	persist_write_string(AddInfo_Text_key, "string_value");
	
	if (persist_exists(Hourly_Vibe_key)) 
		Settings.Hourly_Vibe = persist_read_int(Hourly_Vibe_key);
	else
		Settings.Hourly_Vibe = 1;

	if (persist_exists(BT_Vibe_key)) 
		Settings.BT_Vibe = persist_read_int(BT_Vibe_key);
	else
		Settings.BT_Vibe = 1;
	
	if (persist_exists(Info_Updates_Frequency_key)) 
		Settings.Info_Updates_Frequency = persist_read_int(Info_Updates_Frequency_key);
	else
		Settings.Info_Updates_Frequency = 1;
	
	if (persist_exists(Language_key)) 
		Settings.Language = persist_read_int(Language_key);
	else
		Settings.Language = ENGLISH_LANG;
	
	if (persist_exists(Inverted_key)) 
		Settings.Inverted = persist_read_int(Inverted_key);
	else
		Settings.Inverted = NON_INVERTED_WINDOW;
	
	if (persist_exists(Charge_Vibe_key)) 
		Settings.Charge_Vibe = persist_read_int(Charge_Vibe_key);
	else
		Settings.Charge_Vibe = 1;
	
	if (persist_exists(Hide_Battery_key)) 
		Settings.Hide_Battery = persist_read_int(Hide_Battery_key);
	else
		Settings.Hide_Battery = 0;
	
	if (persist_exists(Hide_BT_key)) 
		Settings.Hide_BT = persist_read_int(Hide_BT_key);
	else
		Settings.Hide_BT = 0;
	
	if (persist_exists(Hide_Weather_key)) 
		Settings.Hide_Weather = persist_read_int(Hide_Weather_key);
	else
		Settings.Hide_Weather = 0;
	
	if (persist_exists(Shake_Update_key)) 
		Settings.Shake_Update = persist_read_int(Shake_Update_key);
	else
		Settings.Shake_Update = 1;
	
	if (persist_exists(Night_Mode_key)) 
		Settings.Night_Mode = persist_read_int(Night_Mode_key);
	else
		Settings.Night_Mode = 0;
	
	if (persist_exists(Night_Start_key)) 
		Settings.Night_Start = persist_read_int(Night_Start_key);
	else
		Settings.Night_Start = 0;
	
	if (persist_exists(Night_Finish_key)) 
		Settings.Night_Finish = persist_read_int(Night_Finish_key);
	else
		Settings.Night_Finish = 0;
	
	if (persist_exists(Night_Silent_key)) 
		Settings.Night_Silent = persist_read_int(Night_Silent_key);
	else
		Settings.Night_Silent = 1;
	
	if (persist_exists(Night_Invert_Display_key)) 
		Settings.Night_Invert_Display = persist_read_int(Night_Invert_Display_key);
	else
		Settings.Night_Invert_Display = 1;
	
	if (persist_exists(Night_Offline_key)) 
		Settings.Night_Offline = persist_read_int(Night_Offline_key);
	else
		Settings.Night_Offline = 1;
	
	if (persist_exists(Date_Style_key)) 
		Settings.Date_Style = persist_read_int(Date_Style_key);
	else
		Settings.Date_Style = 0;
	
	if (persist_exists(Text_Size_key)) 
		Settings.Text_Size = persist_read_int(Text_Size_key);
	else
		Settings.Text_Size = 0;
	
}

static inline void send_int(uint8_t key, uint8_t cmd){
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    Tuplet value = TupletInteger(key, cmd);
    dict_write_tuplet(iter, &value);
 
    app_message_outbox_send();
}

static inline void Shake_Handler(AccelAxisType axis, int32_t direction){
	if (IsConnected_Flag)
		UpdateWeather();
}

static inline void UpdateWeather(){
	
	if (!IsConnected_Flag){
		text_layer_set_text(CWeather_Text, OfflineNames[Settings.Language]);
	}
	
	else if ( (!JustRun_Flag)&&(!((IsNight_Flag) && (Settings.Night_Offline))) ) {
		gbitmap_destroy(BT); 
		BT = gbitmap_create_with_resource(RESOURCE_ID_UPDATING_IMAGE); 
		bitmap_layer_set_bitmap(BT_Image, BT);
		layer_mark_dirty((Layer *)BT_Image);
		
		psleep(1000);
		IsReceiving = app_timer_register(RECEIVING_LATENCY, Answer_Error, 0);
		send_int(CURRENT_WEATHER, 1);
	}
	
	if (Settings.Info_Updates_Frequency)
		app_timer_register(MILLS_IN_HOUR / Settings.Info_Updates_Frequency, UpdateWeather, 0);
	else 
		app_timer_register(MILLS_IN_HOUR, UpdateWeather, 0);
}

static inline void UpdateConnection(bool Connected){
	IsConnected_Flag = Connected;
	
	if ( (!JustRun_Flag)&&(Settings.BT_Vibe) && (!((IsNight_Flag) && (Settings.Night_Silent) ) ) )
		vibes_long_pulse();
	
	if (Connected)
		UpdateWeather();
	else 
		app_timer_cancel(IsReceiving);
	
	gbitmap_destroy(BT); 
	BT = gbitmap_create_with_resource(BT_Icons[Connected]); 
	bitmap_layer_set_bitmap(BT_Image, BT);
	
	text_layer_set_text(Connection_Text, BTNames[Settings.Language][Connected]);
}

static inline void UpdateBattery(BatteryChargeState State){
	static char Percents[] = "100%";
	static bool Vibe_Flag;
	
	if ( (!Vibe_Flag) && (State.is_plugged) && (Settings.Charge_Vibe) && (!JustRun_Flag) && (!((IsNight_Flag) && (Settings.Night_Silent) ) ) ){
		vibes_short_pulse();
		Vibe_Flag = 1;
	}
	
	if ( (Vibe_Flag) && (!State.is_plugged) && (Settings.Charge_Vibe) && (!JustRun_Flag)  && (!((IsNight_Flag) && (Settings.Night_Silent) ) ) ) {
		vibes_short_pulse();
		Vibe_Flag = 0;
	}
	
	snprintf(Percents, sizeof(Percents), "%d%%", State.charge_percent);
	text_layer_set_text(Battery_Text, Percents);
	gbitmap_destroy(BAT); 
	
	if ( (!State.is_charging) && (State.is_plugged) )
		BAT = gbitmap_create_with_resource(RESOURCE_ID_BAT_ICON_FULL); 
	else
		BAT = gbitmap_create_with_resource(Battery_Icons[State.charge_percent / 10]); 
	
	bitmap_layer_set_bitmap(BAT_Image, BAT); 
}

static inline void UpdateTime(struct tm* CurrentTime, TimeUnits units_changed){
	
	IsNight_Flag = 0;
	static bool Not_Inverted = 1;
	static bool IsVibed_Flag   = 0;
	
	/*Night detection*/
	
	if (Settings.Night_Mode) {
		int Time_In_Mins = CurrentTime -> tm_hour * 60 + CurrentTime -> tm_min;
		
		if (Settings.Night_Start > Settings.Night_Finish){
			if ( (Time_In_Mins >= Settings.Night_Start) || (Time_In_Mins <= Settings.Night_Finish) ) {
				IsNight_Flag = 1;
			}
		}
		
		if (Settings.Night_Start < Settings.Night_Finish){
			if ( (Time_In_Mins >= Settings.Night_Start) && (Time_In_Mins <= Settings.Night_Finish) ){
				IsNight_Flag = 1;
			}
			
		}
	}
	
	if ( (IsNight_Flag) && (Settings.Night_Invert_Display) && (Not_Inverted)){
		SetColors(!Settings.Inverted);
		Not_Inverted = 0;
	}
	
	if ( (!IsNight_Flag) && (!Not_Inverted) ){
		SetColors(Settings.Inverted);
		Not_Inverted = 1;
	}
	
	strftime(Time, sizeof(Time), "%H:%M", CurrentTime);
	text_layer_set_text(Time_Text, Time);
	
	if ( (!(CurrentTime -> tm_hour)) && (!(CurrentTime -> tm_min)) ){
		UpdateDate(CurrentTime, SECOND_UNIT);
	}
	
	if ( (!IsVibed_Flag) && (!(CurrentTime -> tm_min)) && (!JustRun_Flag) && (Settings.Hourly_Vibe)  && (!((IsNight_Flag) && (Settings.Night_Silent) ) ) ){
		vibes_double_pulse();
		IsVibed_Flag = 1;
	}

	if ( (CurrentTime -> tm_min == 1) && (IsVibed_Flag) )
		IsVibed_Flag = 0;
}

static inline void UpdateDate(struct tm* CurrentTime, TimeUnits units_changed){
	if (Settings.Date_Style)
		snprintf(Date, sizeof(Date), "%02d %s", CurrentTime->tm_mday, MonthNames[Settings.Language][CurrentTime->tm_mon]);
	else
		snprintf(Date, sizeof(Date), "%02d.%02d.%d", CurrentTime->tm_mday, CurrentTime->tm_mon+1, CurrentTime->tm_year + 1900);

	text_layer_set_text(Date_Text, Date);
	text_layer_set_text(Week_Text, DayNames[Settings.Language][CurrentTime->tm_wday]);
}

static inline void Init_Display(){
	
	/*Set current time*/
	now = time(NULL);
    current_time = localtime(&now);
    UpdateTime(current_time, SECOND_UNIT);
	UpdateDate(current_time, SECOND_UNIT);
	
	/*Set bottom strings*/
 	text_layer_set_text(AddInfo_Text, Settings.AddInfo_Text); 
 	text_layer_set_text(CWeather_Text, Settings.Weather_Text); 
}

int main(void) {
	/*Interface building*/
	JustRun_Flag = 1;
	ReadSettings();
	BuildWindow(Settings.Text_Size);
	
	/*communication with phone*/
	app_message_register_inbox_received(Process_Received_Data);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	/*Application Settings*/
	SetColors(Settings.Inverted);
	SetBarText(Settings.Hide_Battery, Settings.Hide_BT, Settings.Hide_Weather);
	Init_Display();
	
	/*Checking Battery state & BT - connection*/
	UpdateBattery(battery_state_service_peek());
	UpdateConnection(bluetooth_connection_service_peek());
	if (!IsConnected_Flag)
		text_layer_set_text(CWeather_Text, OfflineNames[Settings.Language]);
	
	/*subcribing to services*/
	tick_timer_service_subscribe(MINUTE_UNIT, &UpdateTime);
	bluetooth_connection_service_subscribe(UpdateConnection);
	battery_state_service_subscribe(UpdateBattery);
	
	if (Settings.Shake_Update == 1)
		accel_tap_service_subscribe(Shake_Handler);
	
	window_stack_push(MainWindow, true);
	JustRun_Flag = 0;
	
  	app_event_loop();
	
	/*unsubcribing from services*/
  	tick_timer_service_unsubscribe(); 
	battery_state_service_unsubscribe();
	bluetooth_connection_service_unsubscribe();
	accel_tap_service_unsubscribe();
	
	/*Closing phone connection*/
	app_message_deregister_callbacks();
	
	/*Inerface destroying*/
 	DestroyWindow();
}