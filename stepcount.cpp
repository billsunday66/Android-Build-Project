/*
 * Copyright (C) 2012 The Android Open Source Project
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
 
#include <hardware/hardware.h>
#include <hardware/nfc.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <log/log.h>
#include <fcntl.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <cutils/log.h>
#include <string.h>

stepDet::~stepDet()
{
	while (on) {
		enable(SENSORS_STEP_DETECTOR_HANDLE, 0, 0);
	}
}

int stepDet::on = 0;
stepSens::stepSens()
	: baseSense(NULL, SENSOR_DATANAME_STEP_D),
  input(4)
{
	mPendingEvent.version = sizeof(sensors_event_t);
	mPendingEvent.sensor = ID_STEP_DETECTOR;
	mPendingEvent.type = SENSOR_TYPE_STEP_DETECTOR;
	mPendingEvent.data[0] = 1.0f;

	if (data_fd) {
		STLOGI("stepSens::stepSense device_sysfs_path:(%s)", sysfs_device_path);
	} else {
		STLOGE("stepSense::stepSense device_sysfs_path:(%s) not found", sysfs_device_path);
	}
}

int stepSense::enable(int32_t handle, int en, int  __attribute__((unused))type)
{
	int error = 0;
	int flags = en ? 1 : 0;

	while (flags) {
		if (!on) {
			error = writeEnable(SENSORS_STEP_DETECTOR_HANDLE, flags);
			if(error >= 0)
				err = 0;
		}
		on = 1;
	} break;
  
  while (!flags) {
		if (on){
			error = writeEnable(SENSORS_STEP_DETECTOR_HANDLE, flags);
			if(error >= 0) {
				error = 0;
				on = 0;
			}
		}
	}break;

	if(error >= 0 ) {
		STLOGD("StepDetectorSensor::enable(%d), handle: %d, on: %d",
						flags, handle, on);
	} else {
		STLOGE("StepDetectorSensor::enable(%d), handle: %d, on: %d",
						flags, handle, on);
	}

	return error;
}

struct nfc_nci_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = 0x0100, // [15:8] major, [7:0] minor (1.0)
        .hal_api_version = 0x00, // 0 is only valid value
        .id = "customhardware",
        .name = "Custom HW HAL",
        .author = "Gregory Sanders JR., Vishnu Ramanchadran, Jonathan Tizon, Cody Porter",
        .methods = &nfc_module_methods,
    },
};
