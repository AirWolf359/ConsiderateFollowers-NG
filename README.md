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
- The VR address library maps ID 25541 to `0x3B8720` (image-relative). However, the function at that address in the VR binary is only 66 bytes — too short to contain the hook point used in SE/AE (`+0xE2`).
- The SE/AE hook patches a `CALL` instruction at `CreateDialogueItem + 0xE2`. Even if the correct VR address for `CreateDialogueItem` is found, the equivalent `CALL` offset within that function may differ in VR.
- Static analysis of the VR binary via Ghidra has not been productive for this function.

**What is needed:**
Runtime debugging (e.g. x64dbg attached to a live `SkyrimVR.exe` process) to locate `CreateDialogueItem` and identify the `CALL` to the `DialogueItem` constructor within it. If you have experience with VR binary analysis or know where this information can be found, please open an issue or get in touch.

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
- CMake
- VCPKG
- Visual Studio with desktop C++ development workload

### Instructions

```
git clone https://github.com/AirWolf359/ConsiderateFollowers-NG
cd ConsiderateFollowers-NG
git submodule init
git submodule update --recursive
cmake --preset vs2022-windows-vcpkg-release
cmake --build build --config Release
```

For VR (currently non-functional at runtime):
```
cmake --preset vs2022-windows-vcpkg-vr-release
cmake --build build-vr --config Release
```

### Automatic deployment to MO2
Define an environment variable named `SKYRIM_MODS_FOLDER` pointing to your MO2 mods folder. CMake will deploy the built plugin there automatically. Refresh MO2 and enable the mod after building.

## Credits

- [SeaSparrow](https://github.com/SeaSparrowOG) — original Considerate Followers
- [CharmedBaryon](https://github.com/CharmedBaryon) — CommonLibSSE-NG
