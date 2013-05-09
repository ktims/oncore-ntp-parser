#include <stdint.h>
#include <string.h>
#include <endian.h>
#include <unistd.h>
#include <stdlib.h>
#include "oncore_shmem.h"

void parse_oncore_shmem(char *buf, struct gps_status *gps) {
	struct oncore_shmem o;
	memcpy(&o, buf, sizeof(o));
	gps->month = o.month;
	gps->day = o.day;
	gps->year = be16toh(o.year);
	gps->hour = o.hour;
	gps->minute = o.minute;
	gps->second = o.second;
	gps->flags = o.flags;
	gps->visible = o.visible;
	gps->tracking = o.tracking;
	gps->ns = be32toh(o.ns);

	gps->latitude = (int32_t)be32toh(o.latitude) * (1 / 3600000.0);
	gps->longitude = (int32_t)be32toh(o.longitude) * (1 / 3600000.0);
	gps->altitude = (int32_t)be32toh(o.altitude) / 100.0;
	gps->heading = be16toh(o.heading) / 10.0;
	gps->velocity = be16toh(o.velocity) / 100.0;
	gps->dop = be16toh(o.dop) / 10.0;

	gps->ant_uc = ((o.flags & (1<<7)) != 0);
	gps->ant_oc = ((o.flags & (1<<6)) != 0);
	gps->survey = ((o.flags & (1<<5)) != 0);
	gps->dopmode = ((o.flags & 1) != 0) ? 2 : 3;
}
