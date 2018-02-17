// Nathan Park
#include "TacComponentTransform.h"
#include "TacSerializer.h"
#include "TacCore.h"
namespace Tac
{
  void TransformComp::Load( Loader & serializer )
  {
    serializer.Load(mPos);
    serializer.Load(mRot);
    serializer.Load(mSca);
  }

  void TransformComp::Save( Saver & serializer )
  {
    serializer.SaveFormatted("position", mPos);
    serializer.SaveFormatted("rotation", mRot);
    serializer.SaveFormatted("scale", mSca);
  }

  bool TransformComp::Initialize()
  {
    return true;
  }

  TransformComp::TransformComp()
    : mPos(0,0,0)
    , mRot(0,0,0)
    , mSca(1,1,1)
  {

  }

  void TransformComp::AddToTweakBar( TweakBar & bar )
  {
    TwType twvec3 = GetSystem()->GetCore()->GetTweak().GetTypeVector3();
    bar.AddVar("Transform Pos", 
      "group=Tranform "
      "label='Pos '",
      twvec3, &mPos);
    bar.AddVar("Transform Rot", 
      "group=Tranform "
      "label='Rot '",
      twvec3, &mRot);
    bar.AddVar("Transform Scale", 
      "group=Tranform "
      "label='Scale' ",
      twvec3, &mSca);
  }
  void TransformComp::RemoveFromTweakBar( TweakBar & bar )
  {
    bar.RemoveVar("Transform Pos");
    bar.RemoveVar("Transform Rot");
    bar.RemoveVar("Transform Scale");
  }

} // Tac