# MMRecompRando

## Setup

### Archipelago Setup
1. Know [Archipelago](https://archipelago.gg/tutorial/Archipelago/setup/en).
2. Download the [apworld](https://github.com/RecompRando/MMRecompRando/releases/latest).
3. Place the apworld into `Archipelago/custom_worlds/`.
4. Generate a multiworld, and host the game either locally or on the website.

### Mod Setup
1. Download the latest [Visual Studio C++ Redist](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version).
2. Download the (for now) required build of [Zelda64Recomp](https://github.com/user-attachments/files/19744271/Zelda64Recomp-Release-Mods-Old.zip).
    - Optionally download a texture pack from [this PR](https://github.com/Zelda64Recomp/Zelda64Recomp/pull/499).
        1. Download both `.7z` files from one of the `.rtz` links farther down in the PR.
        2. Unzip the file ending in .001, and place the resulting `.rtz` in your mods folder.
3. Download the [mods](https://github.com/RecompRando/MMRecompRando/releases/latest), as well as `apconnect.txt`.
4. Place the mods within the zip into the Zelda64Recomp mods folder, as well as any texture packs you've downloaded.
    1. If on Windows, this is located at `%LOCALAPPDATA%\Zelda64Recompiled\mods`.
    2. If on Linux, this is located at `~/.config/Zelda64Recompiled/mods`.
5. You can also download [poptracker](https://github.com/black-sliver/PopTracker/releases/latest) along with Seto's [pack](https://github.com/seto10987/Majoras-Mask-AP-PopTracker-Pack/releases/latest)
6. Place `apconnect.txt` next to `Zelda64Recompiled.exe`, and fill it out with your server connection information.
    - MAKE SURE it is NOT named `apconnect.txt.txt`.
7. Open `Zelda64Recompiled.exe`, select your ROM, and hit Start Game.
8. Go nuts.

## FAQ

### When I press "Start Game" the whole thing crashes! What gives?

Make sure you have a file named "apconnect.txt" (without the quotes) in the same folder as the exe, and that it contains the correct connection information for your Archipelago server slot.

### I got Letter to Kafei and now I can't use the Pendant of Memories! Is it just gone?

Try moving your cursor to the Letter to Kafei and pressing the N64's L button. That should cycle through all quest items in that slot. That's true for the Moon's Tear item slot and the Room Key item slot as well.

### I opened a chest/got a check and it always shows a grey Archipelago item, what's wrong with you?

The phantom AP item means the check is not yet implemented. Please use a text client or Seto's amazing poptracker pack from step 6 to see checks that you can definitely get (though some implemented checks may be missing until the tracker is updated).

Otherwise, here is a list of unimplemented checks:

- Tingle
- Oceanside Gold Skulltulas
- Shop Items
- Moon Children Trials Heart Pieces
- Snowhead/Great Bay/Ikana

## Credit for additional mods included

Thank you to the original authors of some of the other recomp mods used!

- danielryb (Better SoDT, Z-Target Fix)
- ThatHypedPerson (Color Mods)

## Writing mods
See [this document](https://hackmd.io/fMDiGEJ9TBSjomuZZOgzNg) for an explanation of the modding framework, including how to write function patches and perform interop between different mods.

## Tools
You'll need to install `clang` and `make` to build rando.
* On Windows, using [chocolatey](https://chocolatey.org/) to install both is recommended. The packages are `llvm` and `make` respectively.
* On MacOS, these can both be installed using Homebrew, as well as `zip`. Apple clang won't work, as you need a mips target for building the mod code.
* On Linux, install the above packages, and some additional ones: `clang`, `make`, `llvm`, `lld`, `zip`, using your distro's package manager.

You'll also need to build [N64Recomp](https://github.com/N64Recomp/N64Recomp) for the `RecompModTool` and `OfflineModRecomp` utilities, and copy them into your clone of rando.

## Building
* First, make sure you've cloned the repo recursively `git clone https://github.com/LittleCube-hax/MMRecompRando --recurse-submodules` or you can update the submodules afterwards `git submodule update --init --recursive`.
* Then, run `make offline` (with an optional job count) to build the mod code itself.
