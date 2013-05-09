#ifndef _ONCORE_H
#define _ONCORE_H

#include <stdint.h>

#pragma pack(push)
#pragma pack(1)

struct oncore_shmem {
	int8_t month, day;
	uint16_t year;
	int8_t hour, minute, second;
	uint32_t ns;
	int32_t latitude, longitude, altitude;
	int8_t padding[4];
	uint16_t velocity, heading, dop;
	uint8_t flags;
	int8_t visible, tracking;
};

#pragma pack(pop)

struct gps_status {
	uint8_t month, day, hour, minute, second, flags, visible, tracking;
	uint16_t year;
	uint32_t ns;
	double latitude, longitude, altitude, heading, velocity, dop;
	uint8_t ant_uc, ant_oc, dopmode, survey;
};

void parse_oncore_shmem(char *buf, struct gps_status *gps);

#endif
