#!/bin/bash
name=$1
zip ${name}.zip public/SocketPlugins/${name}.so public/DIPlugins/${name}.so public/AIPlugins/${name}.so public/SPPlugins/${name}.so application/helpers/device/${name}.php application/views/portal/DevicePage/${name}*.php application/helpers/ini/${name}.yaml application/helpers/ini/${name}.ini public/portal/js/rt_data/rt_data-${name}.js
