#!/bin/sh
#
# Copyright (c) 2018 Robert Bosch GmbH and others.
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0
#
# SPDX-License-Identifier: EPL-2.0
# Forever checker for people so poor that they do not even have cron
#
# Contributors: Robert Bosch GmbH


./sensoris-invehicle $VEHICLE_CLOUD_HOST $VEHICLE_CLOUD_PORT $SESSION_ID $VEHICLE_ID $W3C_SERVER_HOST $W3C_SERVER_PORT $W3C_JWT_TOKEN

