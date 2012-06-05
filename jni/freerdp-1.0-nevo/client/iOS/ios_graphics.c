/**
 * FreeRDP: A Remote Desktop Protocol Client
 * iOS Graphical Objects
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

#include <freerdp/utils/memory.h>

#include "ios_graphics.h"

/* Pointer Class */

void ios_Pointer_New(rdpContext* context, rdpPointer* pointer)
{
    //TODO
}

void ios_Pointer_Free(rdpContext* context, rdpPointer* pointer)
{
    //TODO
}

void ios_Pointer_Set(rdpContext* context, rdpPointer* pointer)
{
    //TODO
}

void ios_Pointer_SetDefault(rdpContext *context)
{
    //TODO
}

/* Graphics Module */

void ios_register_graphics(rdpGraphics* graphics)
{
	rdpPointer* pointer;

	pointer = xnew(rdpPointer);
	pointer->size = sizeof(iosPointer);

	pointer->New = ios_Pointer_New;
	pointer->Free = ios_Pointer_Free;
	pointer->Set = ios_Pointer_Set;
        pointer->SetDefault = ios_Pointer_SetDefault;

	graphics_register_pointer(graphics, pointer);
	xfree(pointer);
}

