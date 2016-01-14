#include <pebble.h>
#include "window.h"

extern Window *MainWindow;
extern TextLayer *Time_Text;
extern TextLayer *Week_Text;
extern TextLayer *Date_Text;
extern TextLayer *Battery_Text;
extern TextLayer *Connection_Text;
extern TextLayer *CWeather_Text;
extern TextLayer *AddInfo_Text;

extern BitmapLayer *BT_Image;
extern BitmapLayer *BAT_Image;

extern GBitmap *BT;
extern GBitmap *BAT;

extern GFont Bar_Font;
extern GFont Time_Font;
extern GFont Date_Font;
extern GFont CWeather_Font;

GColor ForegroundColor; 
GColor BackgroundColor;

void SetColors (bool Invert){
	
	if (Invert){
		ForegroundColor = GColorWhite;
		BackgroundColor = GColorBlack;
		//bitmap_layer_set_compositing_mode(BT_Image, GCompOpSet);
		//bitmap_layer_set_compositing_mode(BAT_Image, GCompOpSet);
	}
	else{
		ForegroundColor = GColorBlack;
		BackgroundColor = GColorWhite;
		//bitmap_layer_set_compositing_mode(BT_Image, GCompOpAnd);
		//bitmap_layer_set_compositing_mode(BAT_Image, GCompOpAnd);
	}
	
	window_set_background_color(MainWindow, BackgroundColor);
	text_layer_set_text_color(Battery_Text, ForegroundColor);
	text_layer_set_text_color(Connection_Text, ForegroundColor);
	text_layer_set_text_color(Week_Text, ForegroundColor);
	text_layer_set_text_color(Time_Text, ForegroundColor);
	text_layer_set_text_color(Date_Text, ForegroundColor);
	text_layer_set_text_color(CWeather_Text, ForegroundColor);
	text_layer_set_text_color(AddInfo_Text, ForegroundColor);
	
}

void BuildWindow(bool Text_Size){
	MainWindow = window_create();
	
	/*BT Icon*/
	BT = gbitmap_create_with_resource(RESOURCE_ID_BT_ICON);
	BT_Image = bitmap_layer_create(GRect(0, 2, 16, 16));
	bitmap_layer_set_bitmap(BT_Image, BT);
  	layer_add_child(window_get_root_layer(MainWindow), bitmap_layer_get_layer(BT_Image));
	
	/*Battery Icon*/
	BAT = gbitmap_create_with_resource(RESOURCE_ID_BAT_ICON_100);
	BAT_Image = bitmap_layer_create(GRect(127, 2, 16, 16));
	bitmap_layer_set_bitmap(BAT_Image, BAT);
  	layer_add_child(window_get_root_layer(MainWindow), bitmap_layer_get_layer(BAT_Image));
	
	/*Battery percent Text*/
	Bar_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_12));
  	Battery_Text = text_layer_create(GRect(16, 2, 112, 16));
	text_layer_set_text_alignment(Battery_Text, GTextAlignmentRight);
	text_layer_set_font(Battery_Text, Bar_Font);
	text_layer_set_background_color(Battery_Text, GColorClear);
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Battery_Text));
	
	/*Bluetooth connection percent Text*/
  	Connection_Text = text_layer_create(GRect(17, 2, 124, 16));
	text_layer_set_text_alignment(Connection_Text, GTextAlignmentLeft);
	text_layer_set_font(Connection_Text, Bar_Font);
	text_layer_set_background_color(Connection_Text, GColorClear);
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Connection_Text));
	
	/*Week Day Text*/
	Date_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_17));
	if (Text_Size)
		Week_Text = text_layer_create(GRect(0, 29, 145, 30)); //20 40
	else
    	Week_Text = text_layer_create(GRect(0, 34, 145, 30)); //20 40
	text_layer_set_text_alignment(Week_Text, GTextAlignmentCenter);
	text_layer_set_font(Week_Text, Date_Font);
	text_layer_set_background_color(Week_Text, GColorClear);
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Week_Text));
	
	/*Time Text*/
	
	if (Text_Size){	// large font 
		Time_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_BIG_43));
  		Time_Text = text_layer_create(GRect(2, 38, 145, 50)); //34
		text_layer_set_text_alignment(Time_Text, GTextAlignmentCenter);
		text_layer_set_font(Time_Text, Time_Font);
		text_layer_set_background_color(Time_Text, GColorClear);
		layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Time_Text));
	}
	
	else {
		Time_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_37));
  		Time_Text = text_layer_create(GRect(0, 46, 145, 50)); //34
		text_layer_set_text_alignment(Time_Text, GTextAlignmentCenter);
		text_layer_set_font(Time_Text, Time_Font);
		text_layer_set_background_color(Time_Text, GColorClear);
		layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Time_Text));
	}
	/*Date Text*/
	if (Text_Size)
		Date_Text = text_layer_create(GRect(0, 90, 145, 25)); //75  95
	else
		Date_Text = text_layer_create(GRect(0, 88, 145, 25)); //75  95
	text_layer_set_text_alignment(Date_Text, GTextAlignmentCenter);
	text_layer_set_font(Date_Text, Date_Font);
	text_layer_set_background_color(Date_Text, GColorClear);
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Date_Text));
	
	/*Current weather Text*/
	CWeather_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_14));
	CWeather_Text = text_layer_create(GRect(0, 147, 145, 25)); //105
	text_layer_set_text_alignment(CWeather_Text, GTextAlignmentCenter);
	text_layer_set_font(CWeather_Text, CWeather_Font);
	text_layer_set_background_color(CWeather_Text, GColorClear);
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(CWeather_Text));

	/*Addiditonal String*/
	
	AddInfo_Text = text_layer_create(GRect(0, 129, 145, 18));
	text_layer_set_text_alignment(AddInfo_Text, GTextAlignmentCenter);
	text_layer_set_font(AddInfo_Text, CWeather_Font);
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(AddInfo_Text));
	text_layer_set_background_color(AddInfo_Text, GColorClear);
}

void SetBarText(bool Battery, bool BT, bool Weather){
	layer_set_hidden((Layer *)Battery_Text, Battery);
	layer_set_hidden((Layer *)Connection_Text, BT);
	layer_set_hidden((Layer *)CWeather_Text, Weather);
}

void DestroyWindow(){
	text_layer_destroy(Time_Text);
	text_layer_destroy(Week_Text);
	text_layer_destroy(CWeather_Text);
	text_layer_destroy(AddInfo_Text);
	text_layer_destroy(Battery_Text);
	text_layer_destroy(Connection_Text);
	gbitmap_destroy(BT); 
	gbitmap_destroy(BAT); 
	bitmap_layer_destroy(BT_Image);
	bitmap_layer_destroy(BAT_Image);
	window_destroy(MainWindow);
}
					   