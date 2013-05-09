#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <endian.h>
#include <unistd.h>
#include <stdlib.h>
#include "oncore_shmem.h"

int main(int argc, char *argv[]) {
	struct gps_status gps;
	char buf[sizeof(struct oncore_shmem)];
	char flagbuf[12];

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
	parse_oncore_shmem(buf, &gps);

	opt = 0;
	bzero(flagbuf, sizeof(flagbuf));
	flagbuf[0] = '\0';
	if (gps.ant_uc) {
		flagbuf[opt++] = 'u';
		flagbuf[opt++] = 'c';
	}
	if (gps.ant_oc) {
		if (opt)
			flagbuf[opt++] = '|';
		flagbuf[opt++] = 'o';
		flagbuf[opt++] = 'c';
	}
	if (gps.survey) {
		if (opt)
			flagbuf[opt++] = '|';
		flagbuf[opt++] = 's';
		flagbuf[opt++] = 's';
	}
	if (gps.dopmode) {
		if (opt)
			flagbuf[opt++] = '|';
		flagbuf[opt++] = gps.dopmode + '0';
		flagbuf[opt++] = 'd';
	}


	if (dop)
		printf("%0.1f\n", gps.dop);
	if (tracking)
		printf("%d\n", gps.tracking);
	if (visible)
		printf("%d\n", gps.visible);
	if (all)
		printf("%d-%02d-%02d %02d:%02d:%08.5f pos: %.6f,%.6f vel: %.3f m/s dop: %.1f flags: %#02x [%s] sats: %d/%d\n", gps.year, gps.month, gps.day, gps.hour, gps.minute, gps.second + (gps.ns / 1000000000.0), gps.latitude, gps.longitude, gps.velocity, gps.dop, gps.flags, flagbuf, gps.tracking, gps.visible);

	fclose(fp);
	return(0);
}
