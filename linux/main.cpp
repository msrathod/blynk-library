/* Comment this out to disable prints and save space */
#define BLYNK_PRINT stdout

#ifdef RASPBERRY
 #include <BlynkApiWiringPi.h>
#else
 #include <BlynkApiLinux.h>
#endif
#include <BlynkSocket.h>
#include <BlynkOptionsParser.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);
#include <BlynkWidgets.h>

#include "roof_client.h"
#include "roof_cmd.h"

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V1
BLYNK_WRITE(V1)
{
  int pinV1 = param.asInt(); // assigning incoming value from pin V1 to a variable
  // process received value
  if(pinV1 > 0){
      client_push_cmd(CMD_OPEN);
  }
}
BLYNK_WRITE(V2)
{
    int pinV2 = param.asInt();
    if(pinV2 > 0){
        client_push_cmd(CMD_CLOSE);
    }
}
BLYNK_WRITE(V3)
{
    int pinV3 = param.asInt();
    if(pinV3 > 0){
        client_push_cmd(CMD_STOP);
    }
}

void setup()
{
  client_setup();
}

void loop()
{
  Blynk.run();
}

int main(int argc, char* argv[])
{
  const char *auth, *serv;
  uint16_t port;
  parse_options(argc, argv, auth, serv, port);
  Blynk.begin(auth, serv, port);

  setup();
  while(true) 
  {
    loop();
  }

  return 0;
}
