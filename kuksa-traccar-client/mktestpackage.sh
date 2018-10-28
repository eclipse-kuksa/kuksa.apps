#!/bin/bash

# Copyright (c) 2018 Eclipse KUKSA project
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0
#
# SPDX-License-Identifier: EPL-2.0

TARGET=/tmp/SIMPLE_traccar.tar.bz2

echo "Making package for Hawkbit"

tar cvjf $TARGET .

echo "Package saved to $TARGET"
