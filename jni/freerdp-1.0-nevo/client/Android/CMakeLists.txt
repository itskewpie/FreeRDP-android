# FreeRDP: A Remote Desktop Protocol Client
# FreeRDP Android Client
#
# Copyright 2011 Nevo (snowgeekx@gmail.com)
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

add_library(freerdp_android
	    android_graphics.c
	    android_graphics.h
	    androidfreerdp.c
	    androidfreerdp.h)

#target_link_libraries(freerdp_android freerdp-core)
#target_link_libraries(freerdp_android freerdp-gdi)
#target_link_libraries(freerdp_android freerdp-kbd)
#target_link_libraries(freerdp_android freerdp-channels)
#target_link_libraries(freerdp_android freerdp-utils)

install(TARGETS freerdp_android DESTINATION ${CMAKE_INSTALL_LIBDIR})
