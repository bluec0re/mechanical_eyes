#ifndef __CONFIG_HEADER__
#define __CONFIG_HEADER__

#include "INIReader.h"

#define SETTINGS_FILE "/etc/mechanical_eyes/settings.ini"

INIReader& getConfig();

#endif
