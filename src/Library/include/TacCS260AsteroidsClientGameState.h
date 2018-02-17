/*!
\author Nathan Park
\brief
*/

#pragma once

#ifdef TacDLL_Export
#define TacDLLAPI __declspec(dllexport)
#else
#define TacDLLAPI __declspec(dllimport)
#endif

#include "TacStateMachine.h"
#include "TacMessageUtil.h"
#include "TacCS260AsteroidsClient.h"
#include "TacCS260AsteroidsCommon.h"
#include "TacCS260AsteroidsClientRunning.h"
#include "TacVector2.h"
#include "TacVector3.h"

namespace Tac
{
  namespace Asteroids
  {
    class ClientLobby : public State <ClientRunning>
    {
    public:
      ClientLobby(StateMachine<ClientRunning>*sm);
      virtual void Update(float dt) override;
      Message::Adder mAdder;
      std::vector<Message::SPHandler> mHandlers;
      SPTweakBar moutput;
      SPTweakBar minput;
      SPTweakBar mLobbyBar;
      std::string minputStr;
      static void TW_CALL OnChat(void * data);
      static void TW_CALL OnReady(void * data);
    };

    class ClientGameplay : public State <ClientRunning>
    {
    public:
      // throws
      ClientGameplay(StateMachine<ClientRunning>*sm);
      virtual void Update(float dt) override;

      void RecvGamestate();
      void SendInput();

      SPTweakBar mGameplayBar;
      SPSocket mUDPSock;

      std::string mServerIP;
      std::string mServerUDPPort;

      std::string mUDPIpv4Addr;
      std::string mUDPPort;

      Message::Adder mAdder;
      std::vector<Message::SPHandler> mHandlers;
    };

    class ClientScoreScreen : public State<ClientRunning>
    {
    public:
      ClientScoreScreen(
        StateMachine<ClientRunning>*sm, 
        bool playerVictory);
      virtual void Update(float dt) override;
      SPTweakBar mScoreScreenBar;
      static void TW_CALL OnPlayAgain(void * data);

    };
  } // asteroids
} // Tac
