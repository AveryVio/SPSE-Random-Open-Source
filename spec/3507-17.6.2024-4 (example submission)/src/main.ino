
// ESP32 Weather Display and a LilyGo EPD 4.7" Display, obtains Open Weather Map data, decodes and then displays it.
// This software, the ideas and concepts is Copyright (c) David Bird 2021. All rights to this software are reserved.
#include <Arduino.h>
#include <esp_task_wdt.h>
// In-built
// In built
#include "freertos/FreeRTOS.h" // In built
#include "freertos/task.h"
#include "epd_driver.h" #include "esp_adc_cal.h"
#include <Arduino)son.h>
// In built
// https://github.com/Xinyuan-LilyGo/LilyGo EPD47
// In-built
// https://github.com/bblanchon/Arduino)son
#include <HTTPClient.hy
// In-built
#include <WIFI.h
// In-built
#include <SPI.h>
// In-built
#include <time.h>
// In built
#include "om credentials.h"
#include "forecast record.h"
#include "lang.h"
#define SCREEN WIDTH
EPD WIDTH
#define SCREEN HE TGHT EPD HEIGHT
    String version = "2.5 / 4.71n"; // Programme version, see change log at end //************ED VARIABLES
enum alignment(LEFT, RIGHT, CENTER);
#define White
0xFF
#define Light Grey
    OxBB
#define Grey
    0x88
#define DarkGrey
    0x44
#define Black
    0x00
#define autoscale on true
#define autoscale off false
#define barchart_on
    true
#define barchart off false
    boolean LargeIcon -
    true;
boolean SmallIcon - false;
#define Large 20
#define Small 8
String Time_str = "--:--:--" String Date str = "__ __ ____"
    // For icon drawing
    // For icon drawing
    int wifi signal,
       Current Hour 0, CurrentMin 0, CurrentSec 0, EventCnt = 0, vref = 1100; //********SUR PROGRAM VARIABLES and OBJECTS ###*******************
#define max readings 24                                                       // Limited to 3-days here, but could go to 5-days - 40
Forecast record_type MxConditions[1];
Forecast record type WxForecast[max_readings];
float pressure readings[max_readings] - (0);
float temperature readings[max_readings](0);
float humidity_readings[max_readings] - (0);
float rain_readings[max_readings] - (0);
float snow readings[max_readings] - (0);
Long SleepDuration - 60; // Sleep time in minutes, aligned to the nearest minute boundary, so 14 30 will always update at 00 or 30 past the hour

int WakeupHour int SleepHour long StartTime long SleepTimer long Delta - 8; // Don't wakeup until after 07:00 to save battery power -23; // Sleep after 23:00 to save battery power
- 0;
- 0;
- 30; // ESP32 rtc speed compensation, prevents display at xx:59:yy and then xx:00:yy (one minute later) to save power
// fonts
#include "opensans8b.h"
#include "opensans10b.h"
#include "opens ans12b.h"
#include "opensans18b.h"
#include "opensans24b.h"
GFXfont currentFont;
uint8_t *framebuffer;
void Beginsleep() ( epd_poweroff_all(); UpdateLocalTime();
SleepTimer (SleepDuration 60 ((CurrentMin % SleepDuration) 60+ Current Sec)) + Delta; //Some ESP32 have a RTC that is too fast to maintain accurate time, so add an offset esp_sleep_enable_timer_wakeup (SleepTimer 1000000LL); // in Secs, 1000009LL converts to Secs as unit 1usec
}
Serial.println("Awake for: " String((m1111s() StartTime) / 1099.0, 3) + "-secs");
Serial.println("Entering " String(SleepTimer) " (secs) of sleep time");
Serial.println("Starting deep-sleep period...");
esp_deep_sleep start(); // Sleep for e.g. 30 minutes
boolean SetupTime()
{
    configTime(gmtoffset_sec, daylightoffset sec, ntpServer, "time.nist.gov"); //(gmtoffset sec, daylightoffset_sec, ntpServer)
    setenv("TZ", Timezone, 1);                                                 // setenv()adds the "TZ" variable to the environment with a value TimeZone, only used if set to 1, 0 means no change
    tzset();                                                                   // Set the TZ environment variable
    delay(100);
    return UpdateLocalTime();
    uint8_t StartWiFi()
    {
        Serial.println("\r\nConnecting to: " String(ssid));
        IPAddress dns(8, 8, 8, 8); // Use Google DNS WiFi.disconnect();
        WIF1.mode(WIFI_STA);       // switch off AP
        WiFi.setAutoConnect(true);
        WiFi.setAutoReconnect(true);
        WIFI.begin(ssid, password);
        if (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
        }
        Serial.printf("STA: Failed!\n");
        WiFi.disconnect(false);
        delay(500);
        WiFi.begin(ssid, password);
        if (WiFi.status() == WL_CONNECTED)
        {
            wifi signal WiFi.RSSI(); // Get Wifi Signal strength now, because the WiFi will be turned off to save power!
        }
    }
    Serial.println("WiFi connected at: " + WIF1.localIP().toString());
    else Serial.println("WiFi connection *** FAILED ***");
    return WiFi.status();
    void StopWiFi()
    {
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
    }
}
Serial.println("WiFi switched off");
void InitialiseSystem()
{
    StartTime - millis();
    Serial.begin(115200);
    while (Serial)
        ;
    Serial.println(String(_FILE_) + "\nStarting...");
    epd init();
    • framebuffer(uint8_t *) ps calloc(sizeof(uint8_t), EPD_WIDTH EPD_HEIGHT / 2);
    if (Iframebuffer)
        Serial.println("Memory alloc failed!");
    set(framebuffer, 0xFF, EPD WIDTH EPD HEIGHT / 2);
    void loop()
    {
        J
            // Nothing to do here
            void
            setup()
        {
            Initialisesystem();
            if (Startwif1() WL CONECTED & SetupTime() bool Makeup false;
                14(Makeuphour > Sleephour) true)
            {
                Wakey = (CurrentHour ye MakeupHour || CurrentHour < -SleepHour);
                6354 Makeup(Current Hour - MakeupHour 22 CurrentHour < -SleepHour);
14 (Makeup) (
byte Attempts-1
bool Reather
false;
bool forecast WiFiClient client;
# 116((Rather
false;
// wifi client object
false || Rxforecast false) && Attempts < 2)
{ // Try up to 2 time
for
    Weather and Forecast data 1 14(leather * false) Rather 14(86orecast - false) RForecast
        obtainéeatherData(client, "weather");
obtainMeatherData(client, "forecast");
Serial.println("Received all weather data...");
14(ather & Stopwif1();
   epd_poweon();
   epd_clear();
   DisplayWeather();
   epd_update();
   forecast)            // Only 11 received both Weather or Forecast proceed
                        // Reduces power consumption
                        // Switch on EPD display
                        // Clear the screen
                        // Display the weather data
                        // Update the display to show the information
    epd_poweroff_all(); // Switch off all poser to EPD
}
bool Decodedeather(WFICients json, String Type)
{
    Serial.print(F("Creating object...and ")); // allocate the JsonDocument
    Dynamic sonbocument doc(64 1674);
    Deserializationroe error if (error)(
        deserialize) son(dor, json); // Deserialize the JSON document
    // Test if parsing succeeds.
    Serial.print(f("deserialize)son() failed: ")); Serial.print interror.c_str());
    return false;
    // convert it to a sect
    JsonObject root - doc.assorobject > ();
    Serial.println(" Decoding Type data");
    if (Type "weather")
    {
        // All Serial.print in statements are for diagnostic purposes and some are not required,

        remove if not needed with //
                                  // WxConditions[0].1on
            - root["coord"]["lon"].as<float>();
        Serial.println(" Lon: " + String(WxConditions[0].lon));
        // WxConditions[0].lat
        -root["coord"]["lat"].as<float>();
        Serial.println(" Lat: " + String(WxConditions[0].lat));
        Wx Conditions[0].Maine - root["weather"][0]["main"].as<char *>();
        Serial.println("Main: " + String(WxConditions[0].Maine));
        Wx Conditions[0].Forecaste - root["weather"][0]["description"].as<char *>();
        Serial.println("Fore: " + String(WxConditions[0].Forecast0));
        -root["weather"][1]["description"].as<char>();
        // WxConditions[0].Forecast1 Serial.println("For1: " + String (WxConditions[0].Forecast1)); //WxConditions[0].Forecast2 Serial.println("For2: " + String (WxConditions[0].Forecast2)); Wx Conditions[0].Icon
        -root["weather"][2]["description"].as<char>();
        -root["weather"][0]["icon"].as<char *>();
        Serial.println("Icon: " + String(WxConditions[0].Icon));
        Wx Conditions[0].Temperature root["main"]["temp"].as<float>();
        Serial.println("Temp: " + String(WxConditions[0].Temperature));
        Wx Conditions[0].Pressure - root["main"]["pressure"].as<float>();
        Serial.println("Pres: " + String(WxConditions[0].Pressure));
Wx Conditions[0].Humidity -root["main"]["humidity").as<float>(); Serial.println("Humi: " + String (WxConditions[0].Humidity));
Wx Conditions[0].Low
root["main"]["temp_min").as<float>();
Serial.println("TLow: " + String (WxConditions[0].Low));
Wx Conditions[0].High
-root["main"]["temp_max").as<float>();
Serial.println("THig: " + String (WxConditions[0].High));
Wx Conditions[0].Windspeed root["wind"]["speed").as<float>(); Serial.println("WSpd: "+ String (WxConditions[0].Windspeed));
Wx Conditions[0].Winddir
-root["wind"]["deg").as<float>();
Serial.println("WDir: " + String (WxConditions[0].Winddir));
Wx Conditions[0].Cloudcover - root["clouds"]["all").as<int>();
Serial.println("CCov: " + String (WXConditions[0].Cloudcover)); // in % of cloud cover Wx Conditions[0].Visibility - root["visibility").as<int>();
Serial.println("Visi: " + String (Wx Conditions[0].Visibility)); // in metres Wx Conditions[0].Rainfall -root["rain"]["1h"].as<float>(); Serial.println("Rain: " + String (WxConditions[0].Rainfall));
Wx Conditions[0].Snowfall
root["snow"]["1h").as<float>();
Serial.println("Snow: " + String (WxConditions[0].Snowfall));
//WxConditions[0].Country - root["sys"]["country").as<char>();
Serial.println("Ctry: " + String (WxConditions[0].Country));
Wx Conditions[0].Sunrise
root["sys"]["sunrise"].as<int>();
root["sys"]["sunset").as<int>();
Serial.println("SRis: " + String (WxConditions[0].Sunrise));
Wx Conditions[0].Sunset
Serial.println("SSet: " + String (WxConditions[0].Sunset));
Wx Conditions[0].Timezone -root["timezone"].as<int>(); Serial.println("Izon: "+ String (WxConditions[0]. Time zone));
)
if (Type "forecast") {
                            // Serial.println(json);
Serial.print(F("\nReceiving Forecast period
JsonArray list
"));
root["list"];
for (byte = 0; r < max readings; r++) {
                                Serial.println("\nPeriod " + String(F) + "-----");
    WxForecast[r].Dt
            WxForecast[r].Temperature
                    WxForecast[r]
                .Low
        );
    -list[r]["dt"], as<int>();
    Serial.println("Temp: " + String(WxForecast[r].Temperature));
list[r]["main"]["temp").as<float>();
-list[r]["main"]["temp_min").as<float>();
Serial.println("ILow: "+String(WxForecast[r], Low));
WxForecast(r).High
list[r]["main"]["temp_max"].as<float>();
Serial.println("THig: " + String(WxForecast[r].High));
WxForecast[r].Pressure
- list[r]["main"]["pressure").as<float>();
Serial.println("Pres: " + String (WxForecast[r]. Pressure));
WxForecast[r].Humidity
-list[r]["main"]["humidity").as<float>();
Serial.println("Humi: "+ String (WxForecast[r]. Humidity));
//WxForecast[r].Forecasto
list[r]["weather"][0]["main"],as<char>();
Serial.println("Fore: " + String (WxForecast[r].Forecast0));

if (consize LargeIcon) {
                                    scale Large;
                                    Offset - 35;
                                    if (contine.endsk th(""))
                                        addwoon(x, y Offset, scale, IconSize);
                                    addcloud(x 15, y - 22, scale / 2, linesize); // Cloud top right addcloud(x-16, y-18, scale / 2, linesize); // Cloud top left addcloud(x, y, scale, linesize);
                                    // Main cloud
                                    wold Gradraph(int xgos, int y pos, int geldth, int gheight, float YIMIN, float YiMax, String title, float Detarray[], int readings, boolean auto scale, boolean barchart_mode)
                                    {
                                        define auto scale margin // sets the autoscale increment, so axis steps up fter a change of // 5 y-axis division markers
#define y minor axis 5
                                            setFont(Openbans 188);
                                        int maxscale
                                            set anyscale .100,
                                            int last y, last y if (no scale true)
                                        {
                                            for (int 1 - 3; i < readings; i *)
                                                (
                                                    14(Gutarray[1] > maxscale) maxYscale
                                                        DataArray[1];
                                                    14(Day) < mierscale) miscale Datatrray[1];
                                            scale round(mantycale
                                                            no scale margin); // Auto scale the graph and round to the
                                            marest value defined, default was viax
                                                                      Yaround(maxscale + 6.5);
                                            14(scale 1 - 6) einscale round(esoscale auto scale margin); // Auto scale the graph and round to the nearest value defined, default was ViMin
                                            Yround(airscale);
11 Draw the gr
jestys (or constraint array[1], Viin, Vax)) / (ViaxYMin) gheight; wad you, y, t), geign + 7, Grey);
for (int p6, readings; g *)
    (
        *****noth / readings 1)1; // max readings is the global variable that
sets the saxise data that can be plotted
vyys(a contralateral),
    Y,
    14(barchart)
{
} else
X))/(Vax-VII)
1, x2, 121, Black); // Two lines for hi-res display
Gratinet last, lest y, x, y, black);
gheight
bres the Y axis scale
for (int spacing-6; sparing y sin axis; spacing) (
for (int 1-6; 14mber of destes; j++) { // Dras dashed graph grid lines
1 (spacing in axis) da estis ineffe pos) geldth / meter of dashes), yses (paign sparing/ y pince exis), geldin / (2 ember 4 dashes), Grey);
TXT PRESSURE IN) (
drdring gos 16, y pes (float)(Max) / y mine axis
wise
If (VIMIA < 1 66 VMax < 10) (
deign
spacing / y pince axis 5, String((V)Max spacing+ 0.01), 1), RIGHT);

Serial.printing Fel StringForecastic) Forecast1));
Forecast().Forecast
    Serial.println("Fa7" + StringForecast() + forecast());
cast(r). Icon
*String(forecast[r].l
7/Forecast[r].Description
Serial.println("Desc: " + StringForecast(r).forecast()) Cloudcover
Serial.println("crov" + *String(forecast)
Serial.printing
Serial.println("Wir *String
dorecast().Rainfall
Serial.printing "Rain: Stringe o stir).Sfall
Serial.printing "Snow Stringono
[2][""] asechar>();
"][0]["description"], avechar>();
// In of cloud cover speed").asflest>();
m"), aveftest>();
Serial.println("Per
String(
str). Perted));
Forecast[2] Pressure; // Measure pressure
fiest pressure trend Forecast(e). Pressure
slope between and later
pressure trend ((Est) (pressure trend 10)) / 10.0; // Remove any small variations less
than 0.1
St (pressure trend > 0)
if (pressure trend 0) ce
if (pressure trend
6)
tions/o). Trend jej. Trend
ngs to Imperio1();
If (UNITS -- "T") Convert
return true;
bool obtalatherData(WIFICITent & citent, const String & Request Type) ( const String units
citent.stop(); // close connection before sending a new request
String url/data/2.5/ Request Type "7q-" City + "," + Country + "&APPID=" + apikey +
1 ("Request Type 1 weather")
-- "Bont>"+ String(max readings);
Language;
http.begin(client, server, 80, 1); //http.begin(url, test_root_ca); //HTTPS example connection
int httpCode-hrtp.GET();
If (httpCode HTTP_CODE_OK) {
                                                    (10ecadeeather(http.getstream(), RequestType)) return false;
                                                    client.stop();
                                                    http.end();
                                                    return true;
                                                    else(
                                                        Serial.printf();
                                                        client.stop();
                                                        http.end();
                                                        return false;
                                                        http.end();
                                                        return true;
                                                        "ection failed, error: Es", http.errorToString(httpcode), c_str());
                                                    flost a to Inches(ficut value)
                                                    {

                                                        return 0.0393701 value;
                                                        void Displayforecastsection(int x, int y)
                                                        {
Displayforecastweather (x, y, f),
) while (<max readings),
int 0
do // Pre load temporary arrays with with data because ( parses by reference and remember that [1] has already been converted to I units
if (tts) pressure readings[r] texForecast[r] Pressure 0.02953, else pressure readings[r] Forecast[r] Pressure, If (Units) rain readings[r] Forecast(r) Rainfall,
rain reading[]
0.6391701, else
"1") snow
Forecast[r] Rainfall tod orecast[r] Snowfall
0.0393701, else
snow readings[r]
Forecast[r], Snowfall,
temperature readings[r] humidity readings[r]
Forecast[r] Temperature, Forecast[r]midity;
) while (r<max readings),
int gwidth 175, gheight 100,
Int gx
(SCREEN WIDTH
Int gy (SCREEN HEIGHT
Int gap
gwidth + gx
width gheight
4)/548;
30),
// (x,y,width, height, invalue, Maxvalue, Title, Data Array, Autoscale, ChartMode) DrauGraph(gx+6 gap, gy, gwidth, height, 900, 1050, Units TXT PRESSURE PA TXT PRESSURE IN, pressure readings, max readings, autoscale on, barchart off);
DrauGraph(gx + 1 gap, gv, gwidth, gheight, 10, 30, Units TXT TEMPERATURE C TXT TEMPERATURE F, temperature readings, max readings, autoscale on, barchart off),
DrauGraph(gx + 2 gap, tv, guldth, height, 0, 100, TXT HUMIDITY PERCENT, humidity readings, max readings, autoscale off, barchart off);
If(Sumo Precip(rain readings, max readings) SunOfPrecip(snow readings, max readings)) Dr auGraph(gs + 3 gap + 5, gv, gidth, gheight, 0, 30, units TXT RAINFALL P TNT RAINFALL IN, rain readings, max readings, autoscale on, barchart on),
    else DrauGraph(gx + 3 gap + S, gv, gwidth, gheight, 0, 30, units " TXT SNOWFALL MM: TXT SNOWFALL IN", snow readings, max readings, autoscale on, barchart on);
                                                        }
vold Drawbattery(int x, int y) (
uint8_t percentage
100;
esp_adc_cal_characteristics t adc chars;
esp adc cal value t val type esp adc_cal characterize (ADC UNIT 1, ADC_ATTEN DB 11, ADC WIDTH BIT 12, 1100, &ade_chars);
if (val type ESP ADC CAL VAL EFUSE VREF) (
Serial.printf("efuse Vref:Su mv", adc_chars.vref);
vref adc_chars.vref;
hoat voltage - analogRead(36) / 4096.0 6.566* (vref/ 1060.0);
if (voltage > 1) { // Only display if there is a valid reading Serial.println("\nvoltage" + String(voltage));
                                                            4 percentage - 2836.9625 pow(voltage, 4) 43987.4889 pow(voltage, 3) 255233.8134. pow(voltage, 2) - 656689.7123 voltage 632041.7303;
}
if (voltage >- 4.20) percentage - 100;
if (voltage <- 3.20) percentage = 0; // orig 3.5
drawRect(x + 25, y fillRect(x + 65, y FillRect(x + 27, y
drawstring(x + 85, y
14, 40, 15, Black);
10, 4, 7, Black);
12, 36 percentage / 100.0, 11, 8lack);
14, String(percentage) + " " + String(voltage, 1), LEFT);
void cloudy(int x, int y, bool IconSize, String IconName) (
int scale Small, linesize 5, Offset - 10;

drawstring(x_pos - 3, y_pos+ gheight spacing / y_minor_axis S, String((VIMax (float) (V1Max ViMin) / y_minor_axis spacing + 0.01), 1), RIGHT);
else (
drawstring(x_pos - 7, y_pos+ gheight spacing / y_minor_axis 5, String((VMax (float) (YiMax - VIMin) / y_minor_axis spacing+ 0.01), 0), RIGHT);
for (int i = 0; i < 3; i++){
                                                            {
                                                                drawstring(20 + x_pos + gwidth / 31, y_pos + gheight 10, String(1) "d", LEFT);
                                                                if (12)
                                                                {
                                                                    draufastVLine(x_pos + gwidth / 31 + gwidth / 3, y_pos, gheight, LightGrey);
                                                                }
                                                            }
}