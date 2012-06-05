/**
 * FreeRDP: A Remote Desktop Protocol Client
 * iOS Client
 *
 * Copyright 2011 Nevo <snowgeekx@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __IOSFREERDP_H
#define __IOSFREERDP_H

#include <freerdp/freerdp.h>
#include <freerdp/graphics.h>
#include <freerdp/gdi/gdi.h>
#include <freerdp/codec/color.h>
#include <freerdp/channels/channels.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct ios_info iosInfo;

struct ios_context
{
	rdpContext _p;

	iosInfo* iosi;
	rdpSettings* settings;
};
typedef struct ios_context iosContext;

struct ios_pointer
{
	rdpPointer pointer;
	uint32 xhot;
	uint32 yhot;
};
typedef struct ios_pointer iosPointer;

struct ios_info
{
	int read_fds;
	HCLRCONV clrconv;
};


#endif /* __IOSFREERDP_H */
