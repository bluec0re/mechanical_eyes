#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_

#include "config.h"

INIReader& getConfig() {
    static INIReader reader(SETTINGS_FILE);

    return reader;
}

#endif
