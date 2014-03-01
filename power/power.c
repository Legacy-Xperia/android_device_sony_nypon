/*
 * Copyright (C) ST-Ericsson SA 2012. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <pthread.h>

#define LOG_TAG "Ux500 PowerHAL"
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

/* Paths to device nodes */
#define APE_OPP "/dev/ape_opp"
#define DDR_OPP "/dev/ddr_opp"
#define ARM_KHZ "/dev/arm_khz"

/* QoS levels*/
#define DISABLE_QOS	0
#define QOS_25		25
#define QOS_50		50
#define QOS_100		100

/* Default delay until deferred work kicks in */
#define DELAY 100000 /* 100ms should be enough */

struct power_state {
	unsigned int ape_opp;
	unsigned int ddr_opp;
	unsigned int arm_khz;
	int fd_ape;
	int fd_ddr;
	int fd_arm;
	void *pending_completion;
	pthread_t thread;
	struct timeval expire_time;
} state;

pthread_mutex_t state_lock;

static void dump_power_state(void)
{
#ifdef DEBUG
	ALOGD("state: ape_opp: %d\n", state.ape_opp);
	ALOGD("state: ddr_opp: %d\n", state.ddr_opp);
	ALOGD("state: arm_khz: %d\n", state.arm_khz);
	ALOGD("state: fd_ape: 0x%08x\n", state.fd_ape);
	ALOGD("state: fd_ddr: 0x%08x\n", state.fd_ddr);
	ALOGD("state: fd_arm: 0x%08x\n", state.fd_arm);
	ALOGD("state: pending_completion: %p\n", state.pending_completion);
	ALOGD("state: thread: %lu\n", state.thread);
	ALOGD("state: expire_time: sec: %lu\n", state.expire_time.tv_sec);
	ALOGD("state: expire_time: usec: %lu\n", state.expire_time.tv_usec);
#endif
}

static void qos_get(char *path, unsigned int QoS_level)
{
	char buf[80];
	int *fd;
	unsigned int *QoS;

#ifdef DEBUG
	ALOGD("New QoS %d requested for %s\n", QoS_level, path);
#endif

	if (QoS_level == DISABLE_QOS)
		return;

	if (strcmp(path, APE_OPP) == 0) {
		fd = &state.fd_ape;
		QoS = &state.ape_opp;
	} else if (strcmp(path, DDR_OPP) == 0) {
		fd = &state.fd_ddr;
		QoS = &state.ddr_opp;
	} else if (strcmp(path, ARM_KHZ) == 0) {
		fd = &state.fd_arm;
		QoS = &state.arm_khz;
	} else {
		ALOGE("Invalid file path");
		return;
	}

	/*
	 * Check whether device node is already open.
	 * N.B. Android PowerManagerService misbehave and don't call our init
	 * function, thus we have to treat file descriptor zero as invalid.
	 */
	if (*fd <= 0) {
		*fd = open(path, O_WRONLY);
#ifdef DEBUG
		ALOGD("Opened %s, with file descriptor 0x%08x\n", path, *fd);
#endif
	}

	if (*fd < 0) {
		strerror_r(errno, buf, sizeof(buf));
		ALOGE("Error opening %s: %s\n", path, buf);
		return;
	}

	/* Update device node with a QoS level */
	if (write(*fd, &QoS_level, sizeof(QoS_level)) != sizeof(QoS_level)) {
		strerror_r(errno, buf, sizeof(buf));
		ALOGE("Error writing to %s: %s\n", path, buf);
		return;
	}

	*QoS = QoS_level;
}

static void qos_put(char *path)
{
	char buf[80];
	int *fd;
	unsigned int *QoS;

#ifdef DEBUG
	ALOGD("QoS discarded for %s\n", path);
#endif

	if (strcmp(path, APE_OPP) == 0) {
		fd = &state.fd_ape;
		QoS = &state.ape_opp;
	} else if (strcmp(path, DDR_OPP) == 0) {
		fd = &state.fd_ddr;
		QoS = &state.ddr_opp;
	} else if (strcmp(path, ARM_KHZ) == 0) {
		fd = &state.fd_arm;
		QoS = &state.arm_khz;
	} else {
		ALOGE("Invalid file path");
		return;
	}

	if (*fd < 0) {
		ALOGE("Can not close %s, file not open\n", path);
		return;
	}

	if (close(*fd) < 0) {
		strerror_r(errno, buf, sizeof(buf));
		ALOGE("Error closing %s: %s\n", path, buf);
		return;
	}

	*QoS = DISABLE_QOS;

	/* Invalidate filedescriptor */
	*fd = -1;
}

static void * start_work(void * arg)
{
	struct timeval now;

#ifdef DEBUG
	ALOGD("Work thread started with ID: %lu, arg: %p\n",
		pthread_self(), arg);
#endif

	do {
		/*
		 * Timer for disabling this hint...
		 * On average we only oversleep by DELAY/4.
		 */
		usleep(DELAY/2);
		gettimeofday(&now, NULL);
	} while (state.pending_completion ||
		timercmp(&state.expire_time, &now, >) > 0);

#ifdef DEBUG
	ALOGD("deferred work expired at sec %lu, usec %lu\n",
		(long) now.tv_sec, (long) now.tv_usec);
#endif

	pthread_mutex_lock(&state_lock);
	qos_put(APE_OPP);
	qos_put(DDR_OPP);
	state.thread = (pthread_t) NULL;
	pthread_mutex_unlock(&state_lock);

	return arg;
}

static void ux500_power_init(struct power_module *module)
{
#ifdef DEBUG
	ALOGD("initializing power module\n");
#endif

	pthread_mutex_lock(&state_lock);
	/* Invalidate all filedescriptors */
	state.fd_ape = -1;
	state.fd_ddr = -1;
	state.fd_arm = -1;
	pthread_mutex_unlock(&state_lock);

	dump_power_state();
}

static void ux500_power_set_interactive(struct power_module *module, int on)
{
#ifdef DEBUG
	ALOGD("setInteractive, on: %s\n", on ? "true" : "false");
#endif
}

static void ux500_power_hint(struct power_module *module, power_hint_t hint,
		void *data)
{
	char buf[80];
	int QoS_level = (int) data;
	struct timeval now, res, delay;

	delay.tv_sec = 0;
	delay.tv_usec = DELAY;

#ifdef DEBUG
	ALOGD("powerHint, hint: 0x%08x, QoS_level: %d\n", hint, QoS_level);
#endif
	dump_power_state();

	switch (hint) {
	case POWER_HINT_VSYNC:
		pthread_mutex_lock(&state_lock);
		switch (QoS_level) {
		case DISABLE_QOS:
			/* update deferred work expire time */
			gettimeofday(&now, NULL);
			timeradd(&now, &delay, &res);

			state.expire_time = res;
			state.pending_completion = data;

#ifdef DEBUG
			ALOGD("updated deferred work expiry time to "
					"sec %lu, usec %lu\n",
					(long) res.tv_sec, (long) res.tv_usec);
#endif

			if (state.thread) {
#ifdef DEBUG
				ALOGD("An existing thread with ID: %lu is "
					"running\n", state.thread);
#endif
			} else {

				/*
				 * Spawn a new thread to create a deferred
				 * work, that way we can avoid re-opening the
				 * dev nodes immediately after closing them.
				 * Android tend to give us a new hint just short
				 * after disabling the previous one.
				 */
				if (pthread_create(&state.thread, NULL,
							start_work, NULL)) {
					strerror_r(errno, buf, sizeof(buf));
					ALOGE("Error creating thread: %s\n",
							buf);
				}

			}
			break;
		case QOS_25:
			if (state.ape_opp < QOS_50 ||
				state.ddr_opp < QOS_25) {

				qos_get(APE_OPP, QOS_50);
				qos_get(DDR_OPP, QOS_25);
			}
			break;

		case QOS_50:
			if (state.ape_opp < QOS_50) {
				qos_get(APE_OPP, QOS_50);
			}
			if (state.ddr_opp < QOS_50) {
				qos_get(DDR_OPP, QOS_50);
			}

			break;
		case QOS_100:
		default:
			if (state.ape_opp < QOS_100 ||
				state.ddr_opp < QOS_100) {

				qos_get(APE_OPP, QOS_100);
				qos_get(DDR_OPP, QOS_100);
			}
			state.pending_completion = data;
			break;
		}
		pthread_mutex_unlock(&state_lock);
		break;
	case POWER_HINT_INTERACTION:

		/* update deferred work expire time */
		gettimeofday(&now, NULL);
		timeradd(&now, &delay, &res);

		pthread_mutex_lock(&state_lock);
		state.expire_time = res;
		pthread_mutex_unlock(&state_lock);

#ifdef DEBUG
		ALOGD("updated deferred work expiry time to "
				"sec %lu, usec %lu\n",
				(long) res.tv_sec, (long) res.tv_usec);
#endif

		if (state.thread) {
#ifdef DEBUG
			ALOGD("An existing thread with ID: %lu is running\n",
				state.thread);
#endif
			return;
		}

		pthread_mutex_lock(&state_lock);
		if (state.ape_opp < QOS_100 || state.ddr_opp < QOS_100) {
			qos_get(APE_OPP, QOS_100);
			qos_get(DDR_OPP, QOS_100);
		}

		/*
		 * Spawn a new thread to create a deferred work. We cannot
		 * sleep in the callers context 'cause it will make the UI
		 * experience horrible.
		 */
		if (pthread_create(&state.thread, NULL, start_work, NULL)) {
			strerror_r(errno, buf, sizeof(buf));
			ALOGE("Error creating thread: %s\n", buf);
		}

		pthread_mutex_unlock(&state_lock);
		break;
	default:
		break;
	}
}

static struct hw_module_methods_t power_module_methods = {
	.open = NULL,
};

struct power_module HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.module_api_version = POWER_MODULE_API_VERSION_0_2,
		.hal_api_version = HARDWARE_HAL_API_VERSION,
		.id = POWER_HARDWARE_MODULE_ID,
		.name = "Ux500 Power HAL",
		.author = "Daniel WILLERUD for ST-Ericsson",
		.methods = &power_module_methods,
	},

	.init = ux500_power_init,
	.setInteractive = ux500_power_set_interactive,
	.powerHint = ux500_power_hint,
};
