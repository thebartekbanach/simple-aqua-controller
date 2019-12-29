# System
The system consists of 11 non-blocking main threads
modules. Each module manages only its own part of the system.
Various basic implementations of base classes are included
in the file `include/system/SystemModule.hpp`.

# ArduinoMenu
The application uses the `ArduinoMenu` library to create and manage
and displaying the entire menu on the device display.

This is my [fork](https://github.com/thebartekbanach/ArduinoMenu/archive/master.zip)
of this library whose latest version can be found [here](https://github.com/neu-rah/ArduinoMenu).
Thanks for creating this library, [neu-rah](https://github.com/neu-rah)!

## Why fork and why did I have to change the original library?
Well, the original version of this library did not give opportunities
registering the event in the form of an indicator on the class method.
It is now possible with the `actionReceiver` class.
An example of use can be seen in the file
`include/system/ActionReceiver.hpp`.

# SystemModuleSettings
This is a template class as a template that adopts a structure of settings.
The main API consists of four main methods:
- `TSettings & data()`: returns the settings structure saved in memory
- `void saveSettings()`: this method should be called after changing the settings object in order to save them in the EEPROM
- `void resetSettings()`: resets settings to factory defaults,
that are given in the constructor
- `void restoreSettings()`: reads from EEPROM and overwrites
current settings, can be used to restore the edited values

# GlobalEventBus
This is the event bus used to send global events to
each module. Can be used to communicate between two
our modules. The best example of use can be a module
`TimeSetupModule` in the file
`include/modules/timeSetup/Module.hpp`.

It sends two events:
- `DAY_CYCLE_BEGIN` - start of the day
- `NIGHT_CYCLE_BEGIN` - the beginning of the night

# ActionCreator
This is the type of module that completely takes control
of the display and joystick. Can be used for
do something more advanced just like
`include/modules/feedingControl/FeedingActionCreator.hpp`

Before activating the action creator, make sure
that no creator is currently being executed.
An example of such a check can be found in the method
`void startFeedingModeEvent()` of the `FeedingControlModule` class
in the file `include/modules/feedingControl/Module.hpp`.

# TimeGuard
TimeGuard protects the RTC module against interference
and restarting time. When getting time all the time
from the RTC, TimeGuard module checks if the time is not older than
last measurement. If time is older, it means that
the clock restarted due to some external interference.
TimeGuard detects this and sets the RTC clock back to the last
known correct time.

This can cause the RTC clock to "slow down" if the controller is located
in an environment where interference often occurs.
