//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IENGINE_H
#define IENGINE_H

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Configurations;
	class IConsoleSystem;

	//---------------------------------------------------------------------//

	class IEngine
	{
	public:
		virtual bool					LoadConfig( const char* FilePath ) = 0;
		virtual bool					SaveConfig( const char* FilePath ) = 0;
		
		virtual void					RunSimulation() = 0;
		virtual void					StopSimulation() = 0;
		virtual void					SetConfig( const Configurations& Configurations ) = 0;
		
		virtual bool					IsRunSimulation() const = 0;
		virtual IConsoleSystem*			GetConsoleSystem() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IENGINE_H
