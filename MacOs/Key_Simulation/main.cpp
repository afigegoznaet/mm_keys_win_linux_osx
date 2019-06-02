//#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include <IOKit/hidsystem/ev_keymap.h>
//#include <AppKit/NSEvent.h>
#include <chrono>
#include <thread>

void pressMMKey(int keyCode);

int main() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  CGEventRef downEvt = CGEventCreateKeyboardEvent(nullptr, 0, true);
  CGEventRef upEvt = CGEventCreateKeyboardEvent(nullptr, 0, false);
  UniChar oneChar = 'A';

  CGEventKeyboardSetUnicodeString(downEvt, 1, &oneChar);
  CGEventKeyboardSetUnicodeString(upEvt, 1, &oneChar);
  CGEventPost(kCGAnnotatedSessionEventTap, downEvt);
  CGEventPost(kCGAnnotatedSessionEventTap, upEvt);

  oneChar = '\n';
  CGEventKeyboardSetUnicodeString(downEvt, 1, &oneChar);
  CGEventKeyboardSetUnicodeString(upEvt, 1, &oneChar);
  CGEventPost(kCGAnnotatedSessionEventTap, downEvt);
  CGEventPost(kCGAnnotatedSessionEventTap, upEvt);

  CFRelease(downEvt);
  CFRelease(upEvt);

  /***
   *
  #define NX_KEYTYPE_SOUND_UP		0
  #define NX_KEYTYPE_SOUND_DOWN		1
  #define NX_KEYTYPE_BRIGHTNESS_UP	2
  #define NX_KEYTYPE_BRIGHTNESS_DOWN	3
  #define NX_KEYTYPE_CAPS_LOCK		4
  #define NX_KEYTYPE_HELP			5
  #define NX_POWER_KEY			6
  #define	NX_KEYTYPE_MUTE			7
  #define NX_UP_ARROW_KEY			8
  #define NX_DOWN_ARROW_KEY		9
  #define NX_KEYTYPE_NUM_LOCK		10

  #define NX_KEYTYPE_CONTRAST_UP		11
  #define NX_KEYTYPE_CONTRAST_DOWN	12
  #define NX_KEYTYPE_LAUNCH_PANEL		13
  #define NX_KEYTYPE_EJECT		14
  #define NX_KEYTYPE_VIDMIRROR		15

  #define NX_KEYTYPE_PLAY			16
  #define NX_KEYTYPE_NEXT			17
  #define NX_KEYTYPE_PREVIOUS		18
  #define NX_KEYTYPE_FAST			19
  #define NX_KEYTYPE_REWIND		20

  #define NX_KEYTYPE_ILLUMINATION_UP	21
  #define NX_KEYTYPE_ILLUMINATION_DOWN	22
  #define NX_KEYTYPE_ILLUMINATION_TOGGLE	23
   */

  pressMMKey(0);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}

void pressMMKey(int keyCode) {

  CGEventRef downEvt = CGEventCreate(nullptr);
  CGEventRef upEvt = CGEventCreate(nullptr);

  CGEventSetType(downEvt, (CGEventType)NX_SYSDEFINED);
  CGEventSetType(upEvt, (CGEventType)NX_SYSDEFINED);

  CGEventSetIntegerValueField(downEvt, (CGEventField)83, 8);
  CGEventSetIntegerValueField(upEvt, (CGEventField)83, 8);

  CGEventSetIntegerValueField(downEvt, (CGEventField)99, 8);
  CGEventSetIntegerValueField(upEvt, (CGEventField)99, 8);

  CGEventSetIntegerValueField(downEvt, (CGEventField)59, 2560);
  CGEventSetIntegerValueField(upEvt, (CGEventField)59, 2816);

  int test = keyCode << 16;

  CGEventSetIntegerValueField(downEvt, (CGEventField)149, test | 2560);
  CGEventSetIntegerValueField(upEvt, (CGEventField)149, test | 2816);

  CGEventSetIntegerValueField(downEvt, (CGEventField)150, 65535);
  CGEventSetIntegerValueField(upEvt, (CGEventField)150, 65535);

  CGEventPost(kCGHIDEventTap, downEvt);
  CGEventPost(kCGHIDEventTap, upEvt);

  CFRelease(downEvt);
  CFRelease(upEvt);
}
