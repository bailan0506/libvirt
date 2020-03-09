/*
 * virdaemon.h: remote/logging/lock management daemon common code
 *
 * Copyright (C) 2006-2020 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "internal.h"

typedef struct _virDaemonLogConfig virDaemonLogConfig;
typedef virDaemonLogConfig *virDaemonLogConfigPtr;

struct _virDaemonLogConfig {
    unsigned int log_level;
    char *log_filters;
    char *log_outputs;
};

int virDaemonSetupLogging(virDaemonLogConfigPtr config,
                          const char *output_fname,
                          bool privileged, bool verbose, bool godaemon);

virDaemonLogConfigPtr
virDaemonLogConfigNew(unsigned int log_level,
                      char *log_filter,
                      char *log_outputs);

void
virDaemonLogConfigFree(virDaemonLogConfigPtr data);



