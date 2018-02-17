/*!
\author Nathan Park
*/
#include "TacTweak.h"
#include "TacComponent.h"
#include <TacErrorIf.h>
#include "TacVector2.h"
#include "TacVector3.h"
#include "TacVector4.h"
#include <TacVeryUtil.h>

#include <vector>

namespace Tac
{
  void TW_CALL CopyStdStringToClient(
    std::string& destinationClientString, 
    const std::string& sourceLibraryString)
  {
    destinationClientString = sourceLibraryString;
  } 

  void TW_CALL CopyCDStringToClient(char **destPtr, const char *src)
  {

    size_t srcLen = (src!=nullptr) ? strlen(src) : 0;
    size_t destLen = (*destPtr!=nullptr) ? strlen(*destPtr) : 0;

    // alloc or realloc dest memory block if needed
    if( *destPtr==nullptr)
      *destPtr = (char *)malloc(srcLen+1);
    else if( srcLen>destLen )
      *destPtr = (char *)realloc(*destPtr, srcLen+1);

    // copy src
    if( srcLen>0 )
      strncpy_s(*destPtr, destLen, src, srcLen);
    (*destPtr)[srcLen] = '\0'; // null-terminated string
  } 

  bool Tweak::Init(unsigned w, unsigned h)
  {
    if (!TwInit(TW_OPENGL, nullptr)) 
      return false;
    TwWindowSize(w, h);
    TwHandleErrors(TweakErrorHandler);

    RegisterVec2();
    RegisterVec3();
    RegisterVec4();
    RegesterCompoent();

    TwCopyStdStringToClientFunc(&CopyStdStringToClient);
    TwCopyCDStringToClientFunc(&CopyCDStringToClient);

    return true;
  }

  void Tweak::Render()
  {
    TwDraw();
  }

  Tweak::~Tweak()
  {
    // returns 0 if failed, 1 if succeeded
    // Seems okay to call no matter what.
    TwTerminate();
  }

  const TwType & Tweak::GetTypeVector2(void) const
  {
    return mTypeVector2;
  }

  const TwType & Tweak::GetTypeVector3(void) const
  {
    return mTypeVector3;
  }

  SPTweakBar Tweak::Create()
  {
    TweakBar * newBar = new TweakBar();
    newBar->SetTweak(this);
    return SPTweakBar(newBar);
  }

  void Tweak::RegesterCompoent()
  {

    std::vector<TwEnumVal> mValues;
    for (unsigned i = 0; i < Comp::Type::eCompCount; ++i)
    {
      Comp::Type iType = (Comp::Type) i;
      TwEnumVal val;
      val.Value = iType;
      val.Label = Comp::GetTypeString(iType);
      mValues.push_back(val);
    }
    // Defining season enum type
    mTypeComponent = TwDefineEnum(
      "Component Type", 
      &mValues.front(), 
      Comp::Type::eCompCount);
  }

  void Tweak::RegisterVec3()
  {
    const char * def = "precision=2 step=0.01";

    TwStructMember Vec3Members[] = {
      { "x", TW_TYPE_FLOAT, offsetof(Vector3, x), def},
      { "y", TW_TYPE_FLOAT, offsetof(Vector3, y), def},
      { "z", TW_TYPE_FLOAT, offsetof(Vector3, z), def}};
    mTypeVector3 = TwDefineStruct(
      "Vector3", Vec3Members, 3, sizeof(Vector3), nullptr, nullptr);


  }

  void Tweak::RegisterVec2()
  {
    const char * def = "precision=2 step=0.01";

    TwStructMember Vec2Members[] = {
      { "x", TW_TYPE_FLOAT, offsetof(Vector2, x), def},
      { "y", TW_TYPE_FLOAT, offsetof(Vector2, y), def}};
    mTypeVector2 = TwDefineStruct(
      "Vector2", Vec2Members, 2, sizeof(Vector2), nullptr, nullptr);


  }

  void Tweak::RegisterVec4()
  {
    const char * def = "precision=2 step=0.01";

    TwStructMember Vec4Members[] = {
      { "x", TW_TYPE_FLOAT, offsetof(Vector4, x), def},
      { "y", TW_TYPE_FLOAT, offsetof(Vector4, y), def},
      { "z", TW_TYPE_FLOAT, offsetof(Vector4, z), def},
      { "w", TW_TYPE_FLOAT, offsetof(Vector4, w), def}};
    mTypeVector4 = TwDefineStruct(
      "Vector4", Vec4Members, 4, sizeof(Vector4), nullptr, nullptr);
  }


  TacDLLAPI const TwType& Tweak::GetTypeComponent() const
  {
    return mTypeComponent;
  }

  TacDLLAPI const TwType& Tweak::GetTypeVector4() const
  {
    return mTypeVector4;
  }

  TweakBar::TweakBar() : mBar(0)//, mName("uninitialized bar")
    , mTweak(nullptr)
  {

  }

  TweakBar::~TweakBar()
  {
    if(mBar)
    {
      TwDeleteBar(mBar);
      mBar = nullptr;
    }
  }

  void TweakBar::Init( const std::string & name )
  {
    mName = name;
    RemoveSpaces(mName);
    mBar = TwNewBar(mName.c_str());
  }

  void TweakBar::ClearBar()
  {
    //if (mBar)
    //{
    TwRemoveAllVars(mBar);
    //}
  }

  void TweakBar::AddVar( 
    const std::string & name, 
    const std::string & params, 
    TwType dataType,
    void * data,
    Type varType)
  {
    ErrorIf(!mBar, "TweakBar::Init() hasn't been called");
    switch (varType)
    {
    case Tac::TweakBar::ReadWrite:
      TwAddVarRW(mBar, name.c_str(), dataType, data, params.c_str());
      break;
    case Tac::TweakBar::ReadOnly:
      TwAddVarRO(mBar, name.c_str(), dataType, data, params.c_str());
      break;
    default:
      ErrorIf(true, "How did you get here");
      break;
    }
  }

  void TweakBar::AddComment( 
    const std::string & name, 
    const std::string & comment )
  {
    ErrorIf(!mBar, "TweakBar::Init() hasn't been called");
    ErrorIf(comment.find("'") != std::string::npos, 
      "You can't have apostrophies in comments");

    std::string def;
    def += " label='";
    def += comment;
    def += "' ";
    TwAddButton(mBar, name.c_str(), nullptr, nullptr, def.c_str());
  }

  void TweakBar::AddButton( 
    const std::string & name, 
    const std::string & description, 
    TwButtonCallback onClick, 
    void * data,
    const std::string & group)
  {
    ErrorIf(!mBar, "TweakBar::Init() hasn't been called");
    std::string def;
    def += " label='";
    def += description;
    def += "' ";
    if (!group.empty())
    {
      def += " group=";
      def += group;
    }
    TwAddButton(mBar, name.c_str(), onClick, data, def.c_str());
  }

  void TweakBar::RemoveVar( const std::string & name )
  {
    TwRemoveVar(mBar, name.c_str());
  }

  void TweakBar::SetPosition( int x, int y )
  {
    std::string def = mName + " position='";
    def += std::to_string(x) + " ";
    def += std::to_string(y) + "' ";
    TwDefine(def.c_str());
  }
  void TweakBar::SetVisible(bool visible)
  {
    std::string def = mName + " visible=";
    if (visible)
      def += "true ";
    else
      def += "false ";
    TwDefine(def.c_str());
  }

  void TweakBar::SetSize( int w, int h )
  {
    std::string def = mName + " size='";
    def += std::to_string(w) + " ";
    def += std::to_string(h) + "' ";
    TwDefine(def.c_str());
  }

  void TweakBar::GetPosition( int & x, int & y )
  {
    // get the current position of a bar
    int pos[2];
    TwGetParam(mBar, NULL, "position", TW_PARAM_INT32, 2, pos);
    x = pos[0];
    y = pos[1];
  }

  void TW_CALL TweakErrorHandler( const char *errorMessage )
  {
    ErrorIf(true, "Ant tweak bar error %s", errorMessage);
  }

} // Tac
