/*
 * virdaemon.c: remote/logging/lock management daemon common code
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

#include <config.h>

#include "virdaemon.h"
#include "virlog.h"


/*
 * Set up the logging environment
 * By default if daemonized all errors go to journald/a logfile
 * but if verbose or error debugging is asked for then also output
 * informational and debug messages. Default size if 64 kB.
 */
int
virDaemonSetupLogging(virDaemonLogConfigPtr config,
                      const char *output_fname,
                      bool privileged, bool verbose, bool godaemon)
{
    virLogReset();

    /*
     * Libvirtd's order of precedence is:
     * cmdline > environment > config
     *
     * Given the precedence, we must process the variables in the opposite
     * order, each one overriding the previous.
     */
    if (config->log_level != 0)
        virLogSetDefaultPriority(config->log_level);

    /* In case the config is empty, both filters and outputs will become empty,
     * however we can't start with empty outputs, thus we'll need to define and
     * setup a default one.
     */
    ignore_value(virLogSetFilters(config->log_filters));
    ignore_value(virLogSetOutputs(config->log_outputs));

    /* If there are some environment variables defined, use those instead */
    virLogSetFromEnv();

    /*
     * Command line override for --verbose
     */
    if ((verbose) && (virLogGetDefaultPriority() > VIR_LOG_INFO))
        virLogSetDefaultPriority(VIR_LOG_INFO);

    /* Define the default output. This is only applied if there was no setting
     * from either the config or the environment.
     */
    virLogSetDefaultOutput(output_fname, godaemon, privileged);

    if (virLogGetNbOutputs() == 0)
        virLogSetOutputs(virLogGetDefaultOutput());

    return 0;
}
