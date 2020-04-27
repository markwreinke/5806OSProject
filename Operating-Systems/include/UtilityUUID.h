//
// Created by csis on 1/27/20.
//

#ifndef INC_5806OSPROJECT_UTILITYUUID_H
#define INC_5806OSPROJECT_UTILITYUUID_H

#include <stdint.h>
#include <sys/types.h>
#include <iostream>
#include <iomanip>

using namespace std;

class UtilityUUID {
    //----------------------------------------------------------------------------
    // UtilityUUID
    //

    public:
        struct UUID {
         uint32_t
            timeLow;
         uint16_t
            timeMid,
            timeHigh,
            clock;
        uint8_t
            node[6];
        };

        static char uuidStr[40];



        //-----------------------------------------------------------------------------
        // static char *uuid2ascii(uint8_t *uuid)
        //  convert UtilityUUID to ascii hex form
        //
        // Parm
        //  uuid - 16-byte uuid
        //
        // Returns
        //  pointer to statically allocated character string with hex form of uuid
        //
        // Notes
        // - Is not reentrant, uses a readSingle static buffer
        //

        static char *uuid2ascii(struct UUID *uuid) {
            sprintf(uuidStr,"%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x",
                uuid->timeLow,uuid->timeMid,uuid->timeHigh,uuid->clock,
                uuid->node[0],uuid->node[1],uuid->node[2],uuid->node[3],
                uuid->node[5],uuid->node[5]);
            return uuidStr;
        };

};


#endif //INC_5806OSPROJECT_UTILITYUUID_H
