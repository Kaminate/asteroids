/*!
\author Nathan Park
\brief
*/

#include "TacCS260AsteroidsClientError.h"
#include "TacCore.h"
#include "TacMathUtil.h"

namespace Tac
{
  namespace Asteroids
  {

    void TW_CALL OnServerConnect(void * data)
    {
      Client* base= (Client*) data;
      base->Connect();
    }

    ClientError::ClientError( 
      StateMachine<Client>*sm,
      const std::string & errMsg)
      : State<Client>(sm)
      , mErrMsg(errMsg)
    {
      int x = rand() % 400;
      int y = rand() % 300;
      mPos.x = (float) x;
      mPos.y = (float) y;

      x = rand() % 100 - 50;
      y = rand() % 100 - 50;
      mVel.x = (float) x;
      mVel.y = (float) y;

      mErrorBar = Get()->GetOwner()->GetCore()->GetTweak().Create();
      mErrorBar->Init("error");
      mErrorBar->AddComment("error", mErrMsg);
      mErrorBar->AddButton("button", "Try Again?", OnServerConnect, Get());
      mErrorBar->SetPosition((int) mPos.x, (int) mPos.y);
    }


    void ClientError::Update( float dt )
    {
      mPos += mVel * dt;
      if (mPos.x > 400 || mPos.x < 0)
        mVel.x *= -1;
      if (mPos.y > 300 || mPos.y < 0)
        mVel.y *= -1;
      Clamp(mPos.x, 0.0f, 400.0f);
      Clamp(mPos.y, 0.0f, 300.0f);

      mErrorBar->SetPosition((int) mPos.x, (int) mPos.y);

    }

  }
} // Tac
