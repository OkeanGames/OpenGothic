#include "visualfxdefinitions.h"

#include <Tempest/Log>

#include "graphics/visualfx.h"
#include "gothic.h"

using namespace Tempest;

VisualFxDefinitions::VisualFxDefinitions(Gothic& gothic) {
  vm = gothic.createVm(u"VisualFx.dat");
  }

VisualFxDefinitions::~VisualFxDefinitions() {
  vm->clearReferences(Daedalus::IC_Pfx);
  }

const VisualFx *VisualFxDefinitions::get(const char *name) {
  auto it = vfx.find(name);
  if(it!=vfx.end())
    return it->second.get();
  auto def = implGet(name);
  if(def==nullptr)
    return nullptr;

  static const char* keyName[int(SpellFxKey::Count)] = {
    "OPEN",
    "INIT",
    "CAST",
    "INVEST",
    "COLLIDE"
    };

  std::unique_ptr<VisualFx> p{new VisualFx(std::move(*def))};
  for(int i=0;i<int(SpellFxKey::Count);++i) {
    char kname[256]={};
    std::snprintf(kname,sizeof(kname),"%s_KEY_%s",name,keyName[i]);
    auto id = vm->getDATFile().getSymbolIndexByName(kname);
    if(id==size_t(-1))
      continue;
    Daedalus::GEngineClasses::C_ParticleFXEmitKey key;
    vm->initializeInstance(key, id, Daedalus::IC_FXEmitKey);
    vm->clearReferences(Daedalus::IC_FXEmitKey);
    p->key(SpellFxKey(i)) = key;
    }

  auto ret = vfx.insert(std::make_pair<std::string,std::unique_ptr<VisualFx>>(name,std::move(p)));
  return ret.first->second.get();
  }

Daedalus::GEngineClasses::CFx_Base *VisualFxDefinitions::implGet(const char *name) {
  static Daedalus::GEngineClasses::CFx_Base ret={};
  if(!vm || name==nullptr || name[0]=='\0')
    return nullptr;

  auto id = vm->getDATFile().getSymbolIndexByName(name);
  if(id==size_t(-1)) {
    Log::e("invalid visual effect: \"",name,"\"");
    return nullptr;
    }

  vm->initializeInstance(ret, id, Daedalus::IC_Vfx);
  vm->clearReferences(Daedalus::IC_Vfx);
  return &ret;
  }
