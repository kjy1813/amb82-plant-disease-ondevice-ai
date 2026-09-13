// On-device plant disease classification on AMB82-mini
// Course: Embedded Systems Experiment (team project, 2025)
//
// Pipeline: Camera CH3 (224x224 RGB) -> NPU image classification
//           -> ICPostProcess() callback -> Serial log + LED warning.
// Camera CH0 (FHD, H.264) is streamed over RTSP for monitoring.
//
// Portfolio cleanup (2026): the Wi-Fi SSID/password that were hard-coded in the
// submitted sketch were moved to wifi_config.h (git-ignored). No other logic
// was changed.

#include "WiFi.h"
#include "StreamIO.h"
#include "VideoStream.h"
#include "RTSP.h"
#include "NNImageClassification.h"
#include "VideoStreamOverlay.h"
#include "ClassificationClassList.h"
#include "wifi_config.h"   // defines WIFI_SSID / WIFI_PASS (see wifi_config.h.example)

#define IMAGERGB 1

#define CHANNEL   0
#define CHANNELNN 3

#define NNWIDTH  224
#define NNHEIGHT 224

#define LED_PIN LED_BUILTIN

VideoSetting config(VIDEO_FHD, 30, VIDEO_H264, 0);
VideoSetting configNN(NNWIDTH, NNHEIGHT, 10, VIDEO_RGB, 0);
NNImageClassification imgclass;
RTSP rtsp;
StreamIO videoStreamer(1, 1);
StreamIO videoStreamerNN(1, 1);

char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;
int status = WL_IDLE_STATUS;

IPAddress ip;
int rtsp_portnum;

void setup()
{
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);

        delay(2000);
    }
    ip = WiFi.localIP();

    config.setBitrate(2 * 1024 * 1024);
    Camera.configVideoChannel(CHANNEL, config);
    Camera.configVideoChannel(CHANNELNN, configNN);
    Camera.videoInit();

    rtsp.configVideo(config);
    rtsp.begin();
    rtsp_portnum = rtsp.getPort();

    imgclass.configVideo(configNN);
    imgclass.configInputImageColor(IMAGERGB);
    imgclass.setResultCallback(ICPostProcess);
    imgclass.modelSelect(IMAGE_CLASSIFICATION, NA_MODEL, NA_MODEL, NA_MODEL, NA_MODEL, CUSTOMIZED_IMGCLASS);
    imgclass.begin();

    videoStreamer.registerInput(Camera.getStream(CHANNEL));
    videoStreamer.registerOutput(rtsp);
    if (videoStreamer.begin() != 0) {
        Serial.println("StreamIO link start failed");
    }

    Camera.channelBegin(CHANNEL);

    videoStreamerNN.registerInput(Camera.getStream(CHANNELNN));
    videoStreamerNN.setStackSize();
    videoStreamerNN.setTaskPriority();
    videoStreamerNN.registerOutput(imgclass);
    if (videoStreamerNN.begin() != 0) {
        Serial.println("StreamIO link start failed");
    }

    Camera.channelBegin(CHANNELNN);
}

void loop()
{

}


void ICPostProcess(void)
{
    int class_id = imgclass.classID();
    if (imgclassItemList[class_id].filter) {
        float prob = imgclass.score();
        printf("class %d, score: %f, name: %s\r\n", class_id, prob, imgclassItemList[class_id].imgclassName);

        if (class_id != 0) { // plant is not healthy
            digitalWrite(LED_PIN, HIGH); // LED on
            Serial.println("LED ON - It needs immediate treatment!");
        } else { // plant is healthy
            digitalWrite(LED_PIN, LOW);  // LED off
        }
    }
}
