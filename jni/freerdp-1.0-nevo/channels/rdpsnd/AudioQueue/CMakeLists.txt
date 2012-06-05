# FreeRDP: A Remote Desktop Protocol Client
# FreeRDP cmake build script
#
# Copyright 2011 Nevo <snowgeekx@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(RDPSND_AUDIOQUEUE_SRCS
	rdpsnd_aq.c
)

include_directories(..)

add_library(rdpsnd_aq ${RDPSND_AUDIOQUEUE_SRCS})
set_target_properties(rdpsnd_aq PROPERTIES PREFIX "")

target_link_libraries(rdpsnd_aq freerdp-utils)

install(TARGETS rdpsnd_aq DESTINATION ${FREERDP_PLUGIN_PATH})
