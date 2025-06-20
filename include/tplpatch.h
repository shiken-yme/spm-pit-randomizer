#pragma once
#include "patch.h"

#include <common.h>
#include <spm/filemgr.h>
#include <spm/icondrv.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/tpl.h>
#include <msl/string.h>

namespace mod::tplpatch
{
  struct TextureWork
  {
    u32 destId = 0;
    u32 srcId = 0;
    wii::tpl::TPLHeader *destTpl = nullptr;
    wii::tpl::TPLHeader *srcTpl = nullptr;
    const char *filePath;
    bool free = false;
    s32 heapType = spm::memory::Heap::HEAP_MAIN;
  };

  void patchTpl(TextureWork *textureWork);

#define TPLPATCH_ICON_REDIRECT 1600

  void iconPatch(char * iconFileName); // Put this in your mod's void main() to enable iconpatch functionality!
}