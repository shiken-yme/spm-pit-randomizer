#include "patch.h"
#include "tplpatch.h"

#include <common.h>
#include <spm/filemgr.h>
#include <spm/icondrv.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/tpl.h>
#include <msl/string.h>

namespace mod::tplpatch
{
  /*
  patchTpl and TextureWork were created by L5050.
  The iconpatch framework was created by Yme.
  Reach out to the respective authors if you need help with using any part of this library!
  */

  using namespace spm;

  // patchTpl takes a TextureWork pointer (args in tplpatch.h) and patches whatever TPL in the game you'd like.
  // Keep in mind that you *may* need to patch a TPL more than once depending on how often the game loads it into memory; your changes will be overwritten each time it's reloaded.
  void patchTpl(TextureWork *textureWork)
  {
    // Loads the tpl if not already loaded by the stated filePath
    if (textureWork->srcTpl == nullptr)
    {
      spm::filemgr::FileEntry *srcFile = spm::filemgr::fileAllocf(4, textureWork->filePath);
      s32 tplSize = srcFile->length;
      textureWork->srcTpl = (wii::tpl::TPLHeader *)spm::memory::__memAlloc(textureWork->heapType, tplSize);
      msl::string::memcpy(textureWork->srcTpl, srcFile->sp->data, tplSize);
      spm::filemgr::fileFree(srcFile);
    }

    // Patches the destination tpl with the one given by the mod.rel
    textureWork->destTpl->imageTable[textureWork->destId] = textureWork->srcTpl->imageTable[textureWork->srcId];

    // Free the memory of the tpl loaded from mod.rel to prevent a leak
    if (textureWork->free)
    {
      spm::memory::__memFree(textureWork->heapType, textureWork->srcTpl);
    }
    return;
  }

  /*
  The iconpatch framework directly overrides normal uses of wicon.tpl (used for all items and many other icons) and points them to your very own custom TPL on the game's root directory.
  To make this library recognize your custom TPL, make sure to initialize tplpatch::iconPatch("filename") in void main() with the filename of your TPL sans .tpl.
  iconpatch automatically redirects all calls to wicon.tpl after a certain offset to your custom TPL.

  EXAMPLE:
  item_data::itemDataTable[45].iconId = 8 + TPLPATCH_ICON_REDIRECT; // Targets item id 45 (Chapter 7 peach texture), icon is set to texture index 8 in the custom TPL (starts from 0!)

  Keep in mind that this library currently does not support adding animated icons, but it can replace existing animated icons (e.g. Mega Star).
  Have fun!!!!
  */

  char *TPLPatchIconTPLName = nullptr;           // This corresponds to the name of your custom TPL! i.e. 
  filemgr::FileEntry *TPLPatchIconTPL = nullptr; // Initializes the custom TPL pointer

  // These hook into vanilla icondrv functions right before they run.
  // These patches are meant to allocate the custom TPL to memory, make it accessible at any time, and help make the custom icons display properly.
  void (*iconMainReal)();
  void (*iconEntryReal)(const char *name, s32 iconId);
  void (*iconGXReal)(wii::mtx::Mtx34 *mtx, icondrv::IconEntry *icon);
  static void iconFuncPatch()
  {
    iconMainReal = patch::hookFunction(icondrv::iconMain,
                                       []()
                                       {
                                         if (TPLPatchIconTPL == 0 || TPLPatchIconTPL == nullptr)
                                         {
                                           const char *root = system::getSpmarioDVDRoot();
                                           filemgr::FileEntry *file = filemgr::fileAsyncf(4, 0, "%s/%s.tpl", root, TPLPatchIconTPLName);
                                           if (file != (filemgr::FileEntry *)0x0)
                                           {
                                             file = filemgr::fileAllocf(4, "%s/%s.tpl", root, TPLPatchIconTPLName); // Allocates the custom tpl to ingame memory on game start
                                             TPLPatchIconTPL = file;                                                // Yippee!!!!!!!!
                                           }
                                         }
                                         iconMainReal();
                                       });

    iconEntryReal = patch::hookFunction(icondrv::iconEntry,
                                        [](const char *name, s32 iconId)
                                        {
                                          iconEntryReal(name, iconId); // Calls iconEntry first so that the following runs at the end
                                          icondrv::IconEntry *icon = icondrv::icondrv_wp->entries;
                                          s32 max = icondrv::icondrv_wp->num;
                                          do // iconEntry doesn't return an icon entry ptr, so we have to loop through every entry to assign this data :D
                                          {
                                            if (icon->iconId >= TPLPATCH_ICON_REDIRECT && icon->iconId == iconId)
                                            {
                                              (icon->curStage).texId = iconId;
                                              (icon->curStage).frames = 1;
                                            }
                                            icon = icon + 1;
                                            max = max - 1;
                                          } while (max != 0);
                                          return;
                                        });

    iconGXReal = patch::hookFunction(icondrv::iconGX,
                                     [](wii::mtx::Mtx34 *mtx, icondrv::IconEntry *icon)
                                     {
                                       if (icon->iconId >= TPLPATCH_ICON_REDIRECT)
                                       {
                                         (icon->curStage).texId = icon->iconId;
                                         (icon->curStage).frames = 1;
                                       }
                                       iconGXReal(mtx, icon);
                                       return;
                                     });
  }

  // This function rewrites TPLGetGXTexObjFromPalette to override wicon.tpl if it's called and get an image from the custom tpl instead.
  void TPLGetGXTexObjFromPaletteNew(wii::tpl::TPLHeader *palette, wii::gx::GXTexObj *dest, u32 id)
  {
    wii::tpl::ImageTableEntry *imgTbl = palette->imageTable;
    s32 idx = 0;
    if (palette->imageCount > 400 && imgTbl[0].image->height == 40 && imgTbl[0].image->width == 96 && id >= TPLPATCH_ICON_REDIRECT) // If wicon.tpl & id > TPLPATCH_ICON_REDIRECT
    {
      palette = (wii::tpl::TPLHeader *)TPLPatchIconTPL->sp->data;
      imgTbl = palette->imageTable;
      idx = id % TPLPATCH_ICON_REDIRECT;
    }
    idx = id % palette->imageCount;
    wii::tpl::ImageHeader *img = imgTbl[idx].image;
    wii::gx::GXInitTexObj(dest, img->data, img->width, img->height, img->format, img->wrapS, img->wrapT, (((u32)img->maxLOD - (u32)img->minLOD) | ((u32)img->minLOD - (u32)img->maxLOD)) >> 0x1f);
    img = imgTbl[idx].image;
    wii::gx::GXInitTexObjLOD(img->minLOD, img->maxLOD, img->LODBias, dest, img->minFilter, img->magFilter, 0, (u32)img->edgeLODEnable, 0);
    return;
  }

  void iconPatch(char *iconFileName)
  {
    TPLPatchIconTPLName = iconFileName;

    // Mods/libraries that hook into iconMain, iconEntry, or iconGX will likely conflict with this library. Please reach out to Yme if this is an issue for you.
    iconFuncPatch();

    // Mods/libraries that modify TPLGetGXTexObjFromPalette may conflict with this library. You're free to use/modify our rewritten function however you'd like for personal use, though!
    patch::hookFunction(wii::tpl::TPLGetGXTexObjFromPalette, TPLGetGXTexObjFromPaletteNew);
  }
}