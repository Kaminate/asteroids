/*!
  \file TacSingleton.h
  \brief
    Uses CRTP to implement singleton pattern.
    If a class will use singleton, then it should preferably do this:
      private:    
        ClassCtor(); 
        frend Singleton<ClassName>
    so that it can ONLY be created through ClassName::Instance()

    The core is a singleton.

  \author Nathan Park
  \par
    All content (c) 2013-2014 DigiPen (USA) Corporation, all rights reserved. 
      Reproduction or disclosure o this file or this contents without the 
      prior written consent of DigiPen Institute of technology is prohibited.
*/
#pragma once

namespace Tac
{  
  template<typename ClassType>
  class Singleton
  {
  public:
    static ClassType & Instance()
    {
      static ClassType sInstance;
      return sInstance;
    }
  };
}

