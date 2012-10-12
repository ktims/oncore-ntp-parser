#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <endian.h>
#include <unistd.h>
#include <stdlib.h>

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

void parse_oncore_Ea(char *buf, struct gps_status *gps) {
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

	gps->ant_uc = (o.flags & (1<<7) != 0);
	gps->ant_oc = (o.flags & (1<<6) != 0);
	gps->survey = (o.flags & (1<<5) != 0);
	gps->dopmode = (o.flags & 1 != 0) ? 2 : 3;
}

int main(int argc, char *argv[]) {
	struct gps_status gps;
	char buf[sizeof(struct oncore_shmem)];

	int opt;
	int visible = 0, tracking = 0, all = 0, dop = 0;

	while ((opt = getopt(argc, argv, "adtv")) != -1) {
		switch (opt) {
			case 'a':
				all = 1;
				break;
			case 'd':
				dop = 1;
				break;
			case 'v':
				visible = 1;
				break;
			case 't':
				tracking = 1;
				break;
			default:
				fprintf(stderr, "Usage: %s [-adtv] <file>\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "Expected argument after options\n");
		exit(EXIT_FAILURE);
	}

	FILE *fp = fopen(argv[optind], "r");
	if (fp == NULL) {
		fprintf(stderr,"Error opening file.\n");
		exit(EXIT_FAILURE);
	}
	fseek(fp, 8, SEEK_SET);
	int result = fread(buf, sizeof(buf), 1, fp);
	if (result != 1) {
		fprintf(stderr,"Read error.\n");
		exit(EXIT_FAILURE);
	}
	parse_oncore_Ea(buf, &gps);

	if (dop)
		printf("%0.1d\n", gps.dop);
	if (tracking)
		printf("%d\n", gps.tracking);
	if (visible)
		printf("%d\n", gps.visible);
	if (all)
		printf("%d-%02d-%02d %02d:%02d:%08.5f pos: %.6f,%.6f vel: %.3f m/s dop: %.1f flags: %#02x sats: %d/%d\n", gps.year, gps.month, gps.day, gps.hour, gps.minute, gps.second + (gps.ns / 1000000000.0), gps.latitude, gps.longitude, gps.velocity, gps.dop, gps.flags, gps.tracking, gps.visible);

	fclose(fp);
	return(0);
}
