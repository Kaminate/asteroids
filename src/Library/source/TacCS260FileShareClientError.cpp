/*!
\author Nathan Park
\brief
*/

#include "TacCS260FileShareCLientError.h"
#include "TacCore.h"
#include "TacMathUtil.h"

namespace Tac
{
  FileShareClientError::FileShareClientError( 
    const std::string & errMsg)
    : mErrMsg(errMsg)
  {
    int x = rand() % 400;
    int y = rand() % 300;
    mPos.x = (float) x;
    mPos.y = (float) y;

    x = rand() % 100 - 50;
    y = rand() % 100 - 50;
    mVel.x = (float) x;
    mVel.y = (float) y;

  }

  void TW_CALL ClientConnect(void * data)
  {
    FileShareClient * client = (FileShareClient*) data;
    client->Connect();
  }

  void FileShareClientError::Init()
  {
    mErrorBar = Get()->GetOwner()->GetCore()->GetTweak().Create();
    mErrorBar->Init("error");
    mErrorBar->AddComment("error", mErrMsg);
    mErrorBar->AddButton("button", "Try Again?", ClientConnect, Get());
    mErrorBar->SetPosition((int) mPos.x, (int) mPos.y);
  }

  void FileShareClientError::Update( float dt )
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

} // Tac
