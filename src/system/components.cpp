#include <Arduino.h>
#include "components.h"


namespace SimpleScheduler {

    //collect results as JSON
    void getResults(String &res) {
        SPS30::results(res);

    }

    //did all API collect data?
    void afterSendData(bool status) {
        SPS30::afterSend(status);
    }

    //collect HTML table with current results
    void getResultsAsHTML(String &res) {
        SPS30::resultsAsHTML(res);
        NetworkWatchdog::resultsAsHTML(res);
    }

    //prepare forms with configuration
    String selectConfigForm(LoopEntryType sensor) {
        String s = F("");
        switch (sensor) {
            case SimpleScheduler::SPS30:
                return SPS30::getConfigHTML();
            case SimpleScheduler::NTW_WTD:
                return NetworkWatchdog::getConfigHTML();
            default:
                return s;
        }
    }

    JsonObject& parseHTTPConfig(LoopEntryType sensor) {

        switch (sensor) {
            case SimpleScheduler::SPS30:
                return SPS30::parseHTTPRequest();
            case SimpleScheduler::NTW_WTD:
                return NetworkWatchdog::parseHTTPRequest();
            default:
                StaticJsonBuffer<16> jsonBuffer;    //empty response
                JsonObject & ret = jsonBuffer.createObject();
                return ret;
        }
    }

    String getConfigJSON(LoopEntryType sensor) {
        String s = F("");
        switch (sensor) {
            case SimpleScheduler::SPS30:
                return SPS30::getConfigJSON();
            case SimpleScheduler::NTW_WTD:
                return NetworkWatchdog::getConfigJSON();
            default:
                return s;
        }
    }

    void readConfigJSON(LoopEntryType sensor, JsonObject &json) {
        switch (sensor) {
            case SimpleScheduler::SPS30:
                SPS30::readConfigJSON(json);
                return;
            case SimpleScheduler::NTW_WTD:
                NetworkWatchdog::readConfigJSON(json);
                return;
            default:
                return;
        }
    }

    void readConfigJSON(JsonObject &json) {
        LoopEntryType i = EMPTY;
        i++;
        for (; i < NAMF_LOOP_SIZE; i++) {
            if (json.containsKey(findSlotKey(i))) {
                JsonObject &item = json[findSlotKey(i)];
                readConfigJSON(i, item);
            }
        }
    }

    void getConfigJSON(String &json) {
        String s, s1;
        json += F("\"sensors\":{");
        LoopEntryType i = EMPTY;
        i++;
        for (; i < NAMF_LOOP_SIZE; i++) {
            s = getConfigJSON(i);
            if (s.length() < 1) continue;    //
            s.remove(s.length() - 1);

            s1 = F("\"{k}\":{{r}},");
            s1.replace(F("{k}"), findSlotKey(i));
            s1.replace(F("{r}"), s);
            json += s1;
        }
        json.remove(json.length() - 1);
        json += F("}");
    }

    const __FlashStringHelper *findSlotKey(LoopEntryType sensor) {
        switch (sensor) {
            case SimpleScheduler::SPS30:
                return FPSTR(SPS30::KEY);
            case SimpleScheduler::NTW_WTD:
                return FPSTR(NetworkWatchdog::KEY);
            default:
                debug_out(F("**** MISSING SENSOR SLOT KEY: "), DEBUG_MIN_INFO, false);
                debug_out(String(sensor), DEBUG_MIN_INFO, true);
                return F("");
        }


    }

    const __FlashStringHelper *findSlotDescription(LoopEntryType sensor) {
        switch (sensor) {
            case SimpleScheduler::SPS30:
                return FPSTR(INTL_SPS30_SENSOR_DESC);
            case SimpleScheduler::NTW_WTD:
                return FPSTR(INTL_NTW_WTD_DESC);
            default:
                return F("");
        }

    }


}

