# Considerate Followers NG

A fork of [Considerate Followers](https://github.com/SeaSparrowOG/ConsiderateFollowers) by SeaSparrow, migrated to [CommonLibSSE-NG](https://github.com/CharmedBaryon/CommonLibSSE-NG) with bug fixes and improved functionality.

## What it does

Prevents followers from speaking idle dialogue when the player is already in conversation with an NPC. Without this mod, followers will talk over the player's active dialogue, which breaks immersion.

This version also suppresses follower idle dialogue when another follower is already speaking — something the original mod intended to support but never fully implemented.

## Differences from the original

### New functionality
- **Follower-to-follower blocking**: When any follower within range is actively speaking, other followers' idle dialogue is suppressed. The original mod tracked only a single "closest speaker" and had a non-functional INI toggle (`bPreventFollowerPileup`) for this feature. This fork removes the toggle and enables the behavior unconditionally.

### Bug fixes
- Fixed a crash on first install when the JSON config folder was not present
- Fixed AE compatibility: the plugin now correctly identifies itself as compatible with post-1.6.629 struct layouts, resolving a version mismatch error on launch

### Library
- Uses CommonLibSSE-NG instead of the original CommonLibSSE fork, enabling a single codebase to target SE, AE, and (eventually) VR

## Compatibility

| Version | Status |
|---|---|
| Skyrim SE 1.5.97 | Not supported (blocked by version check) |
| Skyrim AE 1.6.1130+ | Supported |
| Skyrim VR 1.4.15 | Not yet supported — see below |

## VR Support

VR support is the primary goal of the CommonLibSSE-NG migration, but is currently blocked. The mod hooks into `TESTopic::CreateDialogueItem` (SKSE address library ID 25541) to intercept follower dialogue at the point of creation. This ID is absent from the VR address library CSV, and the correct raw memory offset for this function in `SkyrimVR.exe` has not been determined.

**What is known:**
- Five of the six required VR function addresses have been sourced from the [VR address library CSV](https://github.com/alandtse/skyrim_vr_address_library) and confirmed as valid function starts in Ghidra. One (`AIProcess::ProcessGreet`, ID 39162) maps to a mid-function address and still needs runtime verification.
- `CreateDialogueItem` (ID 25541) is confirmed at VR image-relative offset `0x3B8720`. However, the function at that address is only 66 bytes — far too short to contain the hook point used in SE/AE (`+0xE2`).
- The SE/AE hook patches a `CALL` instruction at `CreateDialogueItem + 0xE2`. In VR, the equivalent `CALL` to the `DialogueItem` constructor either occurs at a different offset or has been compiled away entirely. The VR version of this function appears to be structured very differently from SE/AE.

**What is needed:**
Runtime debugging (e.g. x64dbg attached to a live `SkyrimVR.exe` process) to find where the `DialogueItem` constructor is called in the VR version of `CreateDialogueItem`, and to verify the `AIProcess::ProcessGreet` address. If you have experience with VR binary analysis or know where this information can be found, please open an issue or get in touch.

## INI Settings

Located at `Data\SKSE\Plugins\ConsiderateFollowers.ini`.

| Setting | Default | Description |
|---|---|---|
| `fMaxConversationDistance` | `1200.0` | Maximum distance (in game units) at which a speaking actor suppresses follower idle dialogue. Set to `0` to disable. Values above `2000` are ignored. |

## JSON Whitelist

Mod authors can exempt specific NPCs from suppression by placing a `.json` file in `Data\SKSE\Plugins\ConsiderateFollowers\`. Whitelisted NPCs are always allowed to speak regardless of other conditions.

```json
{
    "Whitelist": ["MyMod.esp|0x123456", "MyFollowerEditorID"]
}
```

Multiple files are supported — each mod can ship its own without conflict.

## Building

### Requirements

- **Visual Studio Build Tools 2022 or later** with the **Desktop development with C++** workload
  - Includes MSVC compiler, Ninja, and CMake (bundled)
  - Available at https://visualstudio.microsoft.com/downloads/ under "Tools for Visual Studio"
- **vcpkg** cloned to `C:\vcpkg` and bootstrapped
  - `git clone https://github.com/microsoft/vcpkg C:\vcpkg`
  - `C:\vcpkg\bootstrap-vcpkg.bat`

> **Note on vcpkg location:** The `C:\vcpkg` path is conventional and keeps package install paths short, avoiding Windows' 260-character path limit. If you have long paths enabled (`LongPathsEnabled = 1` in the registry), you may install vcpkg elsewhere.

### Instructions

Open an **x64 Native Tools Command Prompt for VS** (found in the Start menu after installing Build Tools), then:

```
git clone https://github.com/AirWolf359/ConsiderateFollowers-NG
cd ConsiderateFollowers-NG
git submodule update --init --recursive
cmake --preset build-release-msvc
cmake --build --preset release-msvc
```

vcpkg will download and build all dependencies on first run. This takes several minutes. Subsequent builds are fast.

The output DLL and PDB are placed in `build\release-msvc\`.

### Automatic deployment to MO2

Define a user environment variable named `SKYRIM_MODS_FOLDER` pointing to your MO2 mods folder. CMake will copy the built DLL and PDB there automatically after each build.

### Cleaning up

To stop developing this project and remove build artifacts from your system:

1. **Build output** — delete the `build\` folder in the project directory.
2. **vcpkg package cache** — delete `C:\vcpkg\packages\` and `C:\vcpkg\installed\` to reclaim disk space. The vcpkg toolchain itself (`C:\vcpkg\`) can be deleted entirely if no other projects use it.
3. **vcpkg binary cache** — located at `%LOCALAPPDATA%\vcpkg\archives\` by default. Safe to delete.
4. **Visual Studio Build Tools** — uninstall via the Visual Studio Installer or Windows Settings → Apps.
5. **Environment variables** — remove `VCPKG_ROOT` and `SKYRIM_MODS_FOLDER` from your user environment variables if set.

## Credits

- [SeaSparrow](https://github.com/SeaSparrowOG) — original Considerate Followers
- [alandtse](https://github.com/alandtse) — CommonLibVR (CommonLibSSE-NG fork used by this project)
