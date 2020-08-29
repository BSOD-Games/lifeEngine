// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef EVENT_H
#define EVENT_H

namespace le
{
	enum EEventType
	{
		ET_None,
		ET_WindowClose
	};

	struct Event
	{
		EEventType			type;
	};
}

#endif // !EVENT_H