//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef RECT_H
#define RECT_H

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    template<typename T>
    struct Rect
    {
        Rect( const T& Left = 0, const T& Top = 0, const T& Width = 0, const T& Height = 0) :
            left( Left ),
            top( Top ),
            width( Width ),
            height( Height )
        {}

        T           left;
        T           top;
        T           width;
        T           height;
    };
    
    //---------------------------------------------------------------------//

    typedef Rect<int>           IntRect_t;
    typedef Rect<float>         FloatRect_t;

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !RECT_H