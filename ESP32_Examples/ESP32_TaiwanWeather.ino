/*
ESP32 Weather of Taiwan
Author: Javier G. Siliacay (USTP-CDO)
Facebook: https://www.facebook.com/siliacayjavier

Credits: Special thanks to my friend, an enthusiast in developing devices like Flipper and similar tools.
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

char wifi_ssid[] = "teacher";
char wifi_pass[] = "87654321";

String Weather0012[8] = {"","","","","","","",""};
String Weather1224[8] = {"","","","","","","",""};
String Weather2436[8] = {"","","","","","","",""};

void setup()
{
  Serial.begin(115200);
  initWiFi();
}

void loop()
{
  // Yilan County, Hualien County, Taitung County, Penghu County, Kinmen County, Lienchiang County, Taipei City, New Taipei City, Taoyuan City, Taichung City, Tainan City, Kaohsiung City, Keelung City, Hsinchu County, Hsinchu City, Miaoli County, Changhua County, Nantou County, Yunlin County, Chiayi County, Chiayi City, Pingtung County
  opendataWeather("高雄市","rdec-key-123-45678-011121314");
  
  //getWeather(period, index)   period=0,1,2  index=0,1,2,3,4,5,6,7
  Serial.println("Location= "+getWeather(0, 0));
  Serial.println("Start Time= "+getWeather(0, 1));
  Serial.println("End Time= "+getWeather(0, 2));
  Serial.println("Weather Phenomenon= "+getWeather(0, 3));
  Serial.println("Probability of Precipitation= "+getWeather(0, 4)+" %");
  Serial.println("Minimum Temperature= "+getWeather(0, 5)+" °C");
  Serial.println("Comfort Level= "+getWeather(0, 6));
  Serial.println("Maximum Temperature= "+getWeather(0, 7)+" °C");            
  Serial.println();
  Serial.println("Location= "+getWeather(1, 0));
  Serial.println("Start Time= "+getWeather(1, 1));
  Serial.println("End Time= "+getWeather(1, 2));
  Serial.println("Weather Phenomenon= "+getWeather(1, 3));
  Serial.println("Probability of Precipitation= "+getWeather(1, 4)+" %");
  Serial.println("Minimum Temperature= "+getWeather(1, 5)+" °C");
  Serial.println("Comfort Level= "+getWeather(1, 6));
  Serial.println("Maximum Temperature= "+getWeather(1, 7)+" °C");            
  Serial.println();
  Serial.println("Location= "+getWeather(2, 0));
  Serial.println("Start Time= "+getWeather(2, 1));
  Serial.println("End Time= "+getWeather(2, 2));
  Serial.println("Weather Phenomenon= "+getWeather(2, 3));
  Serial.println("Probability of Precipitation= "+getWeather(2, 4)+" %");
  Serial.println("Minimum Temperature= "+getWeather(2, 5)+" °C");
  Serial.println("Comfort Level= "+getWeather(2, 6));
  Serial.println("Maximum Temperature= "+getWeather(2, 7)+" °C");            
  Serial.println();    
  delay(60000);
}

void initWiFi() {
  WiFi.mode(WIFI_AP_STA);

  for (int i=0;i<2;i++) {
    WiFi.begin(wifi_ssid, wifi_pass);

    delay(1000);
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);

    long int StartTime=millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        if ((StartTime+5000) < millis()) break;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("STAIP address: ");
      Serial.println(WiFi.localIP());
      Serial.println("");

      break;
    }
  }
}

void opendataWeather(String location, String Authorization) {
  location = urlencode(location);
  WiFiClientSecure client_tcp;
  //client_tcp.setInsecure();  //for 1.0.5 or 1.0.6

  String request = "/api/v1/rest/datastore/F-C0032-001?Authorization="+Authorization+"&locationName="+location;
  if (client_tcp.connect("opendata.cwb.gov.tw", 443)) {
    client_tcp.println("GET " + request + " HTTP/1.1");
    client_tcp.println("Host: opendata.cwb.gov.tw");
    client_tcp.println("Connection: close");
    client_tcp.println();
    String getResponse="",Feedback="";
    boolean state = false;
    int waitTime = 10000;
    long startTime = millis();
    char c;
    String temp = "";
    int i = 0;
    while ((startTime + waitTime) > millis()) {
      while (client_tcp.available()) {
        if (state==true) {
          temp = client_tcp.readStringUntil('\r');
          i++;
          if (i%2==0) {
            Feedback += temp;
          }
        }
        else
          c = client_tcp.read();
        if (c == '\n') {
          if (getResponse.length()==0) state=true;
          getResponse = "";
        }
        else if (c != '\r')
          getResponse += String(c);
        startTime = millis();
      }
      if (Feedback.length()!= 0) break;
    }
    client_tcp.stop();
    //Serial.println(Feedback);
    
    JsonObject obj;
    DynamicJsonDocument doc(4096);
    
    Feedback = Feedback.substring(Feedback.indexOf("records")+9,Feedback.length()-1);
    temp = "";
    for (i=0;i<Feedback.length();i++) {
      c = Feedback[i];
      if (c!='\r'&&c!='\n')
      temp += Feedback[i];
    }
    Feedback = temp;
    
    deserializeJson(doc, Feedback);
    obj = doc.as<JsonObject>();

    //00-12
    Weather0012[0] = obj["location"][0]["locationName"].as<String>();
    Weather0012[1] = obj["location"][0]["weatherElement"][0]["time"][0]["startTime"].as<String>();
    Weather0012[2] = obj["location"][0]["weatherElement"][0]["time"][0]["endTime"].as<String>();
    Weather0012[3] = obj["location"][0]["weatherElement"][0]["time"][0]["parameter"]["parameterName"].as<String>();
    Weather0012[4] = obj["location"][0]["weatherElement"][1]["time"][0]["parameter"]["parameterName"].as<String>();
    Weather0012[5] = obj["location"][0]["weatherElement"][2]["time"][0]["parameter"]["parameterName"].as<String>();
    Weather0012[6] = obj["location"][0]["weatherElement"][3]["time"][0]["parameter"]["parameterName"].as<String>();
    Weather0012[7] = obj["location"][0]["weatherElement"][4]["time"][0]["parameter"]["parameterName"].as<String>();
    //12-24
    Weather1224[0] = obj["location"][0]["locationName"].as<String>();
    Weather1224[1] = obj["location"][0]["weatherElement"][0]["time"][1]["startTime"].as<String>();
    Weather1224[2] = obj["location"][0]["weatherElement"][0]["time"][1]["endTime"].as<String>();   
    Weather1224[3] = obj["location"][0]["weatherElement"][0]["time"][1]["parameter"]["parameterName"].as<String>();
    Weather1224[4] = obj["location"][0]["weatherElement"][1]["time"][1]["parameter"]["parameterName"].as<String>();
    Weather1224[5] = obj["location"][0]["weatherElement"][2]["time"][1]["parameter"]["parameterName"].as<String>();
    Weather1224[6] = obj["location"][0]["weatherElement"][3]["time"][1]["parameter"]["parameterName"].as<String>();
    Weather1224[7] = obj["location"][0]["weatherElement"][4]["time"][1]["parameter"]["parameterName"].as<String>();
    //24-36
    Weather2436[0] = obj["location"][0]["locationName"].as<String>();
    Weather2436[1] = obj["location"][0]["weatherElement"][0]["time"][2]["startTime"].as<String>();   
    Weather2436[2] = obj["location"][0]["weatherElement"][0]["time"][2]["endTime"].as<String>();      
    Weather2436[3] = obj["location"][0]["weatherElement"][0]["time"][2]["parameter"]["parameterName"].as<String>();
    Weather2436[4] = obj["location"][0]["weatherElement"][1]["time"][2]["parameter"]["parameterName"].as<String>();
    Weather2436[5] = obj["location"][0]["weatherElement"][2]["time"][2]["parameter"]["parameterName"].as<String>();
    Weather2436[6] = obj["location"][0]["weatherElement"][3]["time"][2]["parameter"]["parameterName"].as<String>();
    Weather2436[7] = obj["location"][0]["weatherElement"][4]["time"][2]["parameter"]["parameterName"].as<String>();
  }
}

String getWeather(int period,int index) {   //period=0,1,2  index=0,1,2,3,4,5,6,7
  if (period==0)
    return Weather0012[index];
  else if (period==1)
    return Weather1224[index];
  else if (period==2)
    return Weather2436[index];
  return "";
}

//https://www.arduino.cc/reference/en/libraries/urlencode/
String urlencode(String str) {
  const char *msg = str.c_str();
  const char *hex = "0123456789ABCDEF";
  String encodedMsg = "";
  while (*msg != '\0') {
    if (('a' <= *msg && *msg <= 'z') || ('A' <= *msg && *msg <= 'Z') || ('0' <= *msg && *msg <= '9') || *msg == '-' || *msg == '_' || *msg == '.' || *msg == '~') {
      encodedMsg += *msg;
    } else {
      encodedMsg += '%';
      encodedMsg += hex[(unsigned char)*msg >> 4];
      encodedMsg += hex[*msg & 0xf];
    }
    msg++;
  }
  return encodedMsg;
}
