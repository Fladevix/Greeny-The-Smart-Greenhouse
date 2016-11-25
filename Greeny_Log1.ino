/*Created by Khoirul Iman December 2015
  Smart Greenhouse Using Intel Galileo Gen 2 */

#include <SPI.h>
#include <DHT.h>
#include<Ethernet.h>
#include <SD.h>
#include <RTClib.h>
#include <Wire.h>

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

#define DHTIN 2
#define DHTOUT 3
#define DHTTYPE DHT22
DHT dht(DHTIN, DHTOUT, DHTTYPE);

long log_time_ms = 5000;
long prev_log_time = 0;

const int chipSelect = 4;
const int TEC = 5;
const int Pump = 6;
const int led = 8;
const int Heat = 7;
const int Soil = A0;
const int ldr = A1;

byte mac[] = { 0x98, 0x4F, 0xEE, 0x05, 0x65, 0x2B };
IPAddress ip(192, 168, 0, 105);
IPAddress subnet(255, 255, 255, 0);
EthernetServer server(80);


void setup()
{
  Serial.begin(9600);
  dht.begin();
  delay(100);
  pinMode(TEC,OUTPUT);
  pinMode(Heat,OUTPUT);
  pinMode(Pump,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(Soil,INPUT);
  pinMode(10,OUTPUT);

  digitalWrite(TEC,HIGH);
  digitalWrite(Heat,HIGH);
  digitalWrite(Pump,HIGH);
  digitalWrite(led,LOW);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

  delay(3000);
  Ethernet.begin(mac, ip);
  server.begin();
  }
void loop()
{
  unsigned long current_time;
  current_time = millis();
  DateTime now = rtc.now();
  
  int l = analogRead(ldr);
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float m = analogRead(Soil);
  m = m / 10 ;
  delay(100);

  if ((current_time - prev_log_time) > log_time_ms) {
    prev_log_time = current_time;
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print(now.year(), DEC);
      dataFile.print('/');
      dataFile.print(now.month(), DEC);
      dataFile.print('/');
      dataFile.print(now.day(), DEC);
      dataFile.print(" (");
      dataFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
      dataFile.print(") ");
      dataFile.print(now.hour(), DEC);
      dataFile.print(':');
      dataFile.print(now.minute(), DEC);
      dataFile.print(':');
      dataFile.print(now.second(), DEC);
      dataFile.println();
      dataFile.print("  Temperatur : ");
      dataFile.print(t);
      dataFile.print(" C");
      dataFile.print("  Humidity : ");
      dataFile.print(h);
      dataFile.print(" %");
      dataFile.print("  Soil Moisture : ");
      dataFile.print(m);
      dataFile.println(" %");
      dataFile.println("================");
      dataFile.close();
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.println(now.second(), DEC);
      Serial.println(t);
      Serial.println(h);
      Serial.println(m);
      Serial.println(l);
      Serial.println("=============");
    }
    else {
      Serial.println("error opening datalog.txt");
    }
  }
  if (m < 30)
  {
    digitalWrite(Pump, LOW);
    delay(10000);
  }
  else 
  {
    digitalWrite(Pump, HIGH);
    delay(100);
  }
  if (t > 27)
  {
    digitalWrite(TEC, LOW);
    digitalWrite(Heat, HIGH);
    delay(100);
  }
  else if (t < 20 && t > 0)
  {
    digitalWrite(Heat, LOW);
    digitalWrite(TEC, HIGH);
    digitalWrite(led,LOW);
    delay(100);
  }
  else if (l < 500 && Heat, HIGH )
  {
    digitalWrite(led, HIGH);
    delay(100);
  }

  else
  {
    digitalWrite(TEC, HIGH);
    digitalWrite(Heat, HIGH);
    digitalWrite (led, HIGH);
    delay(100);
  }
  
  EthernetClient client = server.available();

  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<meta http-equiv=\"refresh\" content=\"60\">");
          client.println("<html lang=\"en\">");
          client.println("<head>");
          client.println("<title>SMART GREENHOUSE</title>");
          client.println("<meta charset=\"utf-8\">");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          client.println("<link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\">");
          client.println("<link rel=\"stylesheet\" href=\"https://googledrive.com/host/0B8W2vcGc0bVqY0ZIcGRNS3NFejg\">");
          client.println("<link href=\"http://fonts.googleapis.com/css?family=Lato\" rel=\"stylesheet\" type=\"text/css\">");
          client.println("<link href=\"http://fonts.googleapis.com/css?family=Montserrat\" rel=\"stylesheet\" type=\"text/css\">");
          client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js\"></script>");
          client.println("<script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js\"></script>");
          client.println("<script src=\"http://cdnjs.cloudflare.com/ajax/libs/moment.js/2.0.0/moment.min.js\"></script>");
          client.println("<script src=\"https://googledrive.com/host/0B8W2vcGc0bVqdjlZeUg5OXRMT0U\"></script>");
          client.println("<link rel=\"icon\"");
          client.println("type=\"image/png\"");
          client.println("href=\"https://googledrive.com/host/0B8W2vcGc0bVqTjg2SkQ2TS1reHM\"/>");
          /*Created by Khoirul Iman December 2015
           Smart Greenhouse Using Intel Galileo Gen 2 */
          client.println("</head>");
          client.println("<body id=\"myPage\" data-spy=\"scroll\" data-target=\".navbar\" data-offset=\"50\">");
          client.println("<nav class=\"navbar navbar-default navbar-fixed-top\">");
          client.println("<div class=\"container-fluid\">");
          client.println("<div class=\"navbar-header\">");
          client.println("<button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#myNavbar\">");
          client.println("<span class=\"icon-bar\"></span>");
          client.println("<span class=\"icon-bar\"></span>");
          client.println("<span class=\"icon-bar\"></span>");
          client.println("</button>");
          client.println("<a class=\"navbar-brand\" href=\"#myPage\"><img src=\"https://googledrive.com/host/0B8W2vcGc0bVqRWpFRkdTY181dFE\" class=\"img-thumbnail\"width=\"75\" height=\"50\"></a>");
          client.println("</div>");
          client.println("<div class=\"collapse navbar-collapse\" id=\"myNavbar\">");
          client.println("<ul class=\"nav navbar-nav navbar-right\">");
          client.println("<li><a href=\"#myPage\">HOME</a></li>");
          client.println("<li><a href=\"#sensor\">ENVIRONTMENT MONITORING</a></li>");
          client.println("<li><a href=\"#cctv\">CCTV PREVIEW</a></li>");
          client.println("<li><a href=\"#contact\">About Me</a></li>");
          client.println("</ul>");
          client.println("<ul class=\"nav navbar-nav navbar-left\">");
          client.println("<li><a>\"Greeny The Smart Greenhouse\"</a></li>");
          client.println("</ul>");
          client.println("</div>");
          client.println("</div>");
          client.println("</nav>");
          /*Created by Khoirul Iman December 2015
          Smart Greenhouse Using Intel Galileo Gen 2 */
          client.println("<div id=\"myCarousel\" class=\"carousel slide\" data-ride=\"carousel\">");
          //<!-- Indicators -->
          client.println("<ol class=\"carousel-indicators\">");
          client.println("<li data-target=\"#myCarousel\" data-slide-to=\"0\" class=\"active\"></li>");
          client.println("<li data-target=\"#myCarousel\" data-slide-to=\"1\"></li>");
          client.println("<li data-target=\"#myCarousel\" data-slide-to=\"2\"></li>");
          client.println("</ol>");

          //<!-- Wrapper for slides -->
          client.println("<div class=\"carousel-inner\" role=\"listbox\">");
          client.println("<div class=\"item active\">");
          client.println("<img src=\"https://googledrive.com/host/0B8W2vcGc0bVqb1dhVWdSbXk5M0U\" alt=\"GH\" width=\"1200\" height=\"700\">");
          client.println("<div class=\"carousel-caption\">");
          client.println("<h3>Smart Greenhouse</h3>");
          client.println("<p>Think SMART and Grow It</p>");
          client.println("</div>");
          client.println("</div>");

          client.println("<div class=\"item\">");
          client.println("<img src=\"https://googledrive.com/host/0B8W2vcGc0bVqQ3MzMEszRUlRZEE\" alt=\"intel\" width=\"1200\" height=\"700\">");
          client.println("<div class=\"carousel-caption\">");
          client.println("<h3>Intel Galileo Gen 2</h3>");
          client.println("<p>The Intel® Galileo Gen 2 development board is a microcontroller board based on the Intel® Quark™ SoC X1000 application processor, a 32-bit Intel® Pentium® brand system on a chip (SoC). It is the first board based on Intel® architecture designed to be hardware and software pin-compatible with shields designed for the Arduino Uno R3.</p>");
          client.println("</div>");
          client.println("</div>");

          client.println("<div class=\"item\">");
          client.println("<img src=\"https://googledrive.com/host/0B8W2vcGc0bVqRDM0YXQwTW5tZ2M\" alt=\"roofgarden\">");
          client.println("<div class=\"carousel-caption\">");
          client.println("<h3>Roof Garden</h3>");
          client.println("<p>A roof garden is a garden on the roof of a building. Besides the decorative benefit, roof plantings may provide food, temperature control, hydrological benefits, architectural enhancement, habitats or corridors for wildlife, recreational opportunities, and in large scale it may even have ecological benefits. The practice of cultivating food on the rooftop of buildings is sometimes referred to as rooftop farming. Rooftop farming is usually done using green roof, hydroponics, aeroponics or air-dynaponics systems or container gardens.</p>");
          client.println("</div>");
          client.println("</div>");
          client.println("</div>");

          //<!-- Left and right controls -->
          client.println("<a class=\"left carousel-control\" href=\"#myCarousel\" role=\"button\" data-slide=\"prev\">");
          client.println("<span class=\"glyphicon glyphicon-chevron-left\" aria-hidden=\"true\"></span>");
          client.println("<span class=\"sr-only\">Previous</span>");
          client.println("</a>");
          client.println("<a class=\"right carousel-control\" href=\"#myCarousel\" role=\"button\" data-slide=\"next\">");
          client.println("<span class=\"glyphicon glyphicon-chevron-right\" aria-hidden=\"true\"></span>");
          client.println("<span class=\"sr-only\">Next</span>");
          client.println("</a>");
          client.println("</div>");

          //<!-- Container (Sensor Section) -->
          client.println("<div id=\"sensor\" class=\"container text-center\">");
          client.println("<h3>ENVIRONMENT MONITORING</h3>");
          client.println("<div id=\"clock\" class=\"dark\">");
          client.println("<div class=\"display\">");
          client.println("<div class=\"weekdays\"></div>");
          client.println("<div class=\"ampm\"></div>");
          client.println("<div class=\"alarm\"></div>");
          client.println("<div class=\"digits\"></div>");
          client.println("</div>");
          client.println("</div>");
          client.println("<br>");
          client.println("<p>Greenhouse is a structure with walls and roof made chiefly of transparent material, such as glass, in which plants requiring regulated climatic conditions are grown. These structures range in size from small sheds to industrial-sized buildings. A miniature greenhouse is known as a cold frame. The interior of a greenhouse exposed to sunlight becomes significantly warmer than the external ambient temperature, protecting its contents in cold weather. Here we want to monitor Environment of Greenhouse with 3 parameter. Temperature , Humidity and Soil Moisture . We Use DHT11 sensor for monitoring Temperature and Humidity and Soil Moisture Sensor for monitoring Soil Moisture.</p>");
          client.println("<div id=\"senso\" class=\"light\">");
          client.println("<div class=\"display\">");
          client.println("<br>");
          client.println("<p>Temperature    :   ");
          client.print(t);
          client.println("C");
          client.println("</p>");
          client.println("<br>");
          client.println("<p>Humidity       :   ");
          client.print(h);
          client.println("%");
          client.println("</p>");
          client.println("<br>");
          client.println("<p>Soil Moisture  :    ");
          client.print(m);
          client.println("%");
          client.println("<p>");
          client.println("<br>");
          client.println("</div>");
          client.println("</div>");
          client.println("</div>");

          //<!-- Container (CCTV Section) -->
          client.println("<div id=\"cctv\" class=\"bg-2\">");
          client.println("<div class=\"container\">");
          client.println("<h3 class=\"text-center\">CCTV Preview</h3>");
          client.println("<h2 class=\"text-center\">CCTV use to real time monitor your Plant <br> This CCTV also have Motion Sensor who can take picture when it got a motion in Greenhouse and will sent it tou your mail.<br><br><iframe src=\"http://192.168.0.106/index.html\" width=\"1100\" height=\"700\" align=\"middle\">");
          client.println("</iframe></h2>");
          client.println("</div>");
          client.println("</div>");

          //<!-- Container (About me) -->
          client.println("<div id=\"contact\" class=\"container\">");
          client.println("<h3 class=\"text-center\">About Me</h3>");
          client.println("<p class=\"text-center\"><em>Internet Of Things !</em>");
          client.println("<br>");
          client.println("<div class=\"row\">");
          client.println("<div class=\"col-md-4\">");
          client.println("<img src=\"https://googledrive.com/host/0B8W2vcGc0bVqdzZoc1c3TzN4VlE\" class=\"img-thumbnail\" alt=\"imn\" width=\"150\" height=\"125\">");
          client.println("<br>");
          client.println("<p><span class=\"glyphicon glyphicon-user\"></span>Khoirul Iman</p>");
          client.println("<p><span class=\"glyphicon glyphicon-map-marker\"></span>Bogor, Indonesia</p>");
          client.println("<p><span class=\"glyphicon glyphicon-phone\"></span>Phone: +62 89639419450</p>");
          client.println("<p><span class=\"glyphicon glyphicon-envelope\"></span>Email: khiziman99@gmail.com</p>");
          client.println("<p><span class=\"glyphicon glyphicon-education\"></span>Computer Science ,Gunadarma University</p>");
          client.println("</div>");
          client.println("</div>");
          client.println("</div>");
          client.println("<br>");

          client.println("<div id=\"googleMap\"></div>");

          //<!-- Add Google Maps -->
          client.println("<script src=\"http://maps.googleapis.com/maps/api/js\"></script>");
          client.println("<script>");
          client.println("var myCenter = new google.maps.LatLng(-6.5929456, 106.7600269);");

          client.println("function initialize() {");
          client.println("var mapProp = {");
          client.println("center:myCenter,");
          client.println("zoom:12,");
          client.println("scrollwheel:false,");
          client.println("draggable:false,");
          client.println("mapTypeId:google.maps.MapTypeId.ROADMAP");
          client.println("};");

          client.println("var map = new google.maps.Map(document.getElementById(\"googleMap\"),mapProp);");

          client.println("var marker = new google.maps.Marker({");
          client.println("position:myCenter,");
          client.println("});");

          client.println("marker.setMap(map);");
          client.println("}");

          client.println("google.maps.event.addDomListener(window, 'load', initialize);");
          client.println("</script>");

          //<!-- Footer -->
          client.println("<footer class=\"text-center\">");
          client.println("<a class=\"up-arrow\" href=\"#myPage\" data-toggle=\"tooltip\" title=\"TO TOP\">");
          client.println("<span class=\"glyphicon glyphicon-chevron-up\"></span>");
          client.println("</a><br>");
          client.println("<p>Created By Khoirul Iman</p>");
          client.println("<p>Gunadarma University 2016</a></p>");
          client.println("<img src = \"https://googledrive.com/host/0B8W2vcGc0bVqVzFGVlZpZzJFUDQ\" width=\"75\" height=\"75\" >");
          client.println("</footer>");

          client.println("<script>");
          client.println("$(document).ready(function(){");
          // Initialize Tooltip
          client.println("$('[data-toggle=\"tooltip\"]').tooltip();");

          // Add smooth scrolling to all links in navbar + footer link
          client.println("$(\".navbar a, footer a[href='#myPage']\").on('click', function(event) {");

          // Prevent default anchor click behavior
          client.println("event.preventDefault();");

          // Store hash
          client.println("var hash = this.hash;");

          // Using jQuery's animate() method to add smooth page scroll
          // The optional number (900) specifies the number of milliseconds it takes to scroll to the specified area
          client.println("$('html, body').animate({");
          client.println("scrollTop: $(hash).offset().top");
          client.println("}, 900, function(){");

          // Add hash (#) to URL when done scrolling (default click behavior)
          client.println("window.location.hash = hash;");
          client.println("});");
          client.println("});");
          client.println("})");
          client.println("</script>");
          client.println("</body>");
          client.println("</html>");

          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("client disonnected");
  }
}

