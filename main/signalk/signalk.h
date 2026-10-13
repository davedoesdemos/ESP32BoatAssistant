#ifndef SIGNALK_H
#define SIGNALK_H

#include "queue_reader.h"

// ws://<cerbo-ip>:3000/signalk/v1/stream?subscribe=none
#define CERBO_WS_ADDRESS "ws://192.168.1.2:3000"
//don't subscribe to anything
#define CERBO_WS_URI CERBO_WS_ADDRESS "/signalk/v1/stream?subscribe=none"
// Define the Signal K subscription JSON payload using clean multiline literal strings
static const char *SIGNALK_SUBSCRIPTION_PLAN = 
    "{"
    "  \"context\": \"vessels.self\","
    "  \"subscribe\": ["
    "    {"
    "      \"path\": \"navigation.headingMagnetic\","
    "      \"period\": 500"
    "    },"
    "    {"
    "      \"path\": \"navigation.position\","
    "      \"period\": 500"
    "    },"
    "    {"
    "      \"path\": \"navigation.speedOverGround\","
    "      \"period\": 500"
    "    },"
    "    {"
    "      \"path\": \"navigation.courseOverGroundTrue\","
    "      \"period\": 500"
    "    },"
    "    {"
    "      \"path\": \"navigation.speedThroughWater\","
    "      \"period\": 500"
    "    },"
    "    {"
    "      \"path\": \"environment.wind.speedApparent\","
    "      \"period\": 500"
    "    },"
    "    {"
    "      \"path\": \"environment.wind.angleApparent\","
    "      \"period\": 500"
    "    },"
    "    {"
    "      \"path\": \"environment.depth.belowTransducer\","
    "      \"period\": 500"
    "    }"
    "  ]"
    "}";


typedef enum {
// -----SignalK-----
    //Vessel
    SIGNALK_TOPIC_NAVIGATION_HEADING,
    SIGNALK_TOPIC_NAVIGATION_POSITION,
    SIGNALK_TOPIC_NAVIGATION_SOG,
    SIGNALK_TOPIC_NAVIGATION_COG,
    SIGNALK_TOPIC_NAVIGATION_STW,
    SIGNALK_TOPIC_ENVIRONMENT_AWS,
    SIGNALK_TOPIC_ENVIRONMENT_AWD,
    SIGNALK_TOPIC_ENVIRONMENT_DEPTH,
    //add topics here, keep the last one
    SIGNALK_TOPIC_COUNT             // (Total count)
} signalk_topic_id_t;

// Hardcode the complete strings directly into your registry array
static const char *signalk_topic_registry[SIGNALK_TOPIC_COUNT] = {
    // Vessel
        [SIGNALK_TOPIC_NAVIGATION_HEADING] = "navigation.headingMagnetic",
        [SIGNALK_TOPIC_NAVIGATION_POSITION] = "navigation.position",
        [SIGNALK_TOPIC_NAVIGATION_SOG] = "navigation.speedOverGround",
        [SIGNALK_TOPIC_NAVIGATION_COG] = "navigation.courseOverGroundTrue",
        [SIGNALK_TOPIC_NAVIGATION_STW] = "navigation.speedThroughWater",
        [SIGNALK_TOPIC_ENVIRONMENT_AWS] = "environment.wind.speedApparent",
        [SIGNALK_TOPIC_ENVIRONMENT_AWD] = "environment.wind.angleApparent",
        [SIGNALK_TOPIC_ENVIRONMENT_DEPTH] = "environment.depth.belowTransducer",
};

void signalk_app_start(void);

#endif //SIGNALK_H