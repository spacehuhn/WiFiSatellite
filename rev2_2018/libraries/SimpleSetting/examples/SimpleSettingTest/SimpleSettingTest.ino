#include <iostream> // cout

#include "SimpleSetting.h"

using namespace std;
using namespace simplesetting;

SimpleSetting settings;

void setup() {
    settings.add(new SettingFlag("TestFlag"));
    settings.add(new SettingMemory("TestMemory", 1));
    settings.add(new SettingMode("TestMode", { "Eins", "Zwei", "Drei", "Vier" }));
    settings.add(new SettingStatic<string>("TestStatic", "SomeStaticValue"));
    settings.add(new SettingValue<int16_t>("TestValue", 16000, 1, 0, INT16_MAX));
    settings.add(new SettingTime("TestTime", 100000));
    settings.add(new SettingString("TestString", "Hallo"));

    Setting* s = settings.get("TestMemory");
    if (s) s->set_value("1GB 100MB 10KB 1b");

    s = settings.get("TestMode");
    if (s) s->up();
}

void loop() {
    cout << settings;
    delay(3000);
}