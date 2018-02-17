#include "TacCS260AsteroidsClientConnect.h"
#include "TacCS260AsteroidsClientRunning.h"
#include "TacCS260AsteroidsCommon.h"
#include "TacMathUtil.h"
#include "TacCore.h"

namespace Tac
{
  namespace Asteroids
  {
    ClientConnect::ClientConnect(StateMachine<Client>*sm)
      : State<Client>(sm)
      , mMaxSecondsToWait(5)
      , mSecondsRemaining(mMaxSecondsToWait)
    {
      int x = rand() % 400;
      int y = rand() % 300;
      mPos.x = (float) x;
      mPos.y = (float) y;

      x = rand() % 100 - 50;
      y = rand() % 100 - 50;
      mVel.x = (float) x;
      mVel.y = (float) y;

      std::ifstream ifs(gConfigFilepath);
      if (!ifs.is_open())
        throw Exception("Failed to open " + gConfigFilepath);

      std::string serversip;
      std::string serversport;
      ifs >> serversip;
      ifs >> serversport;

      addrinfo hints;
      ZeroMemory(&hints, sizeof(hints));
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
      mAddrInfo = std::shared_ptr<AddrInfo>( new AddrInfo(
        serversip.c_str(), 
        serversport.c_str(), 
        &hints));

      if (!mAddrInfo->Ok())
        throw Exception(mAddrInfo->GetSpecificError());

      mConnectSocket = SPSocket(new Socket(*mAddrInfo));

      if (!mConnectSocket->Ok())
        throw Exception("Failed to create a socket");

      if (!mConnectSocket->SetBlocking(false))
        throw Exception("Failed to create a socket");

      // lets only try to connect to the first address
      addrinfo * node = mAddrInfo->GetInfo();
      if (SOCKET_ERROR == connect(
        mConnectSocket->mSocket, 
        node->ai_addr, 
        (int) node->ai_addrlen))
      {
        // Connect failed
        mOutputBar = Get()->GetOwner()->GetCore()->GetTweak().Create();
        mOutputBar->Init("Status");
        mOutputBar->AddComment("", "Connecting, please wait...");
        mOutputBar->AddVar("asdf", 
          "label='Time remaining' precision='2'", 
          TW_TYPE_FLOAT, &mSecondsRemaining, TweakBar::ReadOnly);
        mOutputBar->SetPosition((int) mPos.x, (int) mPos.y);
        int iResult = WSAGetLastError();
        switch (iResult)
        {
        case WSAEWOULDBLOCK:
          // it should land here
          break;
        default:
          std::string errStr = GetWSAErrorString(iResult);
          throw Exception(errStr);
        }
      }
    }

    void ClientConnect::Update( float dt )
    {
      mPos += mVel * dt;
      if (mPos.x > 400 || mPos.x < 0)
        mVel.x *= -1;
      if (mPos.y > 300 || mPos.y < 0)
        mVel.y *= -1;
      Clamp(mPos.x, 0.0f, 400.0f);
      Clamp(mPos.y, 0.0f, 300.0f);

      mOutputBar->SetPosition((int) mPos.x, (int) mPos.y);

      // Use the select function to determine the completion of the 
      // connection request by checking to see if the socket is writeable.
      fd_set writable;
      timeval timeSelectCanTake;
      timeSelectCanTake.tv_sec = 0;
      timeSelectCanTake.tv_usec = 4000; // 4 miliseconds
      writable.fd_array[0] = mConnectSocket->mSocket;
      writable.fd_count = 1;
      // waiting on 1 socket to be writable (mConnectSocket!)
      if (select(420, nullptr, &writable, nullptr, &timeSelectCanTake) == 1)
      {
        auto sm = GetStateMachine();
        sm->SetNextState(
          SPClientState(new ClientRunning(sm, mConnectSocket)));
      }

      mSecondsRemaining -= dt;
      if (mSecondsRemaining <= 0)
      {
        Get()->Error("Timed out while attempting to connect to server");
      }
    }


  } // asteroids
} // Tac
