// Nathan Park
#include "TacSystemTransform.h"
#include "TacCore.h"
#include "TacComponentTransform.h"
namespace Tac
{
  TacDLLAPI TransformSystem::TransformSystem()
  {
    
  }

  TacDLLAPI  void TransformSystem::Update( float dt )
  {
    for (SPComp comp : GetComponentSet(Comp::Type::eCompTransform))
    {
      TransformComp * transform = (TransformComp*) comp.get();
      transform->mFakePos = transform->mPos;
      transform->mFakeRot = transform->mRot;
    }
  }

  TacDLLAPI  void TransformSystem::UpdateFakeTime( float dt )
  {
    for (SPComp comp : GetComponentSet(Comp::Type::eCompTransform))
    {
      TransformComp * transform = (TransformComp*) comp.get();
      transform->mFakeTransform = Matrix4::Transform(
        transform->mSca, 
        transform->mFakeRot, 
        transform->mFakePos);
    }
  }

} // Tac